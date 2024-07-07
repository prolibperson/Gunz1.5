USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------

IF( NOT (OBJECT_ID('spGetGambleRewardItem')) IS NULL )                      DROP PROC spGetGambleRewardItem
IF( NOT (OBJECT_ID('spChangeGambleItemToRewardNormalItem')) IS NULL )       DROP PROC spChangeGambleItemToRewardNormalItem
IF( NOT (OBJECT_ID('spChangeGambleItemToRewardSpendableItem')) IS NULL )    DROP PROC spChangeGambleItemToRewardSpendableItem
IF( NOT (OBJECT_ID('spBringAccountItem')) IS NULL )                         DROP PROC spBringAccountItem
IF( NOT (OBJECT_ID('spBringAccountItemStackable')) IS NULL )                DROP PROC spBringAccountItemStackable
IF( NOT (OBJECT_ID('spBringBackAccountItem')) IS NULL )                     DROP PROC spBringBackAccountItem
IF( NOT (OBJECT_ID('spBringBackAccountItemStackable')) IS NULL )            DROP PROC spBringBackAccountItemStackable
GO
-------------------------------------------------------------------------------------------------------------------------
  
CREATE PROC dbo.spGetGambleRewardItem
AS    
BEGIN    

    SET NOCOUNT ON;  

    SELECT  GRIID, GIID, ItemIDMale, ItemIDFemale
            , RentHourPeriod, ISNULL(ItemCnt, 1) AS ItemCnt, RatePerThousand
    FROM    dbo.GambleRewardItem(NOLOCK)

END
GO

-------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spChangeGambleItemToRewardNormalItem
	@CID				INT
	, @CIID				INT
	, @GIID				INT
	, @GRIID			INT
	, @RewardItemID		INT
    , @RewardRentPeriod INT
AS BEGIN

	SET NOCOUNT ON;

    -----------------------------------------------------------------------------

    DECLARE @NowDate DATETIME;
	SET @NowDate = GETDATE();	

	DECLARE @GambleItemCount	INT;
			
	SELECT	@GambleItemCount = ISNULL(Cnt, 1)
	FROM	CharacterItem(NOLOCK)
	WHERE	CIID = @CIID
	AND		CID IS NOT NULL;
		
	-----------------------------------------------------------------------------
	
    SET XACT_ABORT ON;

	BEGIN TRAN -----------  
	
		----------------------------------------------------------------------------------------
		
		IF( @GambleItemCount > 1 ) BEGIN
		
			UPDATE	CharacterItem 
			SET		Cnt = Cnt - 1
			WHERE	CID = @CID 
			AND		CIID = @CIID 
			
		END
		ELSE BEGIN
		
			DELETE  CharacterItem 
			WHERE	CID = @CID 
			AND		CIID = @CIID
			
		END
		
		---------------------------------------------------------------------------------------	
		
		INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
		VALUES (@CID, @RewardItemID, @NowDate, @NowDate, @RewardRentPeriod, 1)
		
		DECLARE @OrderCIID INT;			
		SET @OrderCIID = @@IDENTITY;			

	COMMIT TRAN  -----------

    SET XACT_ABORT OFF;
		

    INSERT dbo.ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, ItemID, [Count])
    VALUES (202, @NowDate, @CID, @CIID, @GIID, 1);	

    INSERT dbo.ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, ItemID, [Count])
    VALUES (102, @NowDate, @CID, @OrderCIID, @RewardItemID, 1);

    INSERT INTO LogDB..GambleLog(CID, GIID, RewardItemID, RegDate, RentHourPeriod, ItemCnt)
    VALUES (@CID, @GIID, @RewardItemID, @NowDate, @RewardRentPeriod, 1);

    ----------------------------------------------------------------------------------------
		
	SELECT @OrderCIID AS 'ORDERCIID'
END  
GO

-------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spChangeGambleItemToRewardSpendableItem
-- ALTER PROC dbo.spChangeGambleItemToRewardSpendableItem
	@CID				INT
	, @CIID				INT
	, @GIID				INT
	, @GRIID			INT
	, @RewardItemID		INT
    , @RewardCount      INT
AS BEGIN

	SET NOCOUNT ON;

    -----------------------------------------------------------------------------

    DECLARE @NowDate DATETIME;
	SET @NowDate = GETDATE();	

	DECLARE @GambleItemCount INT;			
	SELECT	@GambleItemCount = ISNULL(Cnt, 1)
	FROM	CharacterItem(NOLOCK)
	WHERE	CIID = @CIID
	AND		CID IS NOT NULL;
		
	-----------------------------------------------------------------------------
	
    SET XACT_ABORT ON;

	BEGIN TRAN -----------  
	
		----------------------------------------------------------------------------------------
		
		IF( @GambleItemCount > 1 ) BEGIN
		
			UPDATE	CharacterItem 
			SET		Cnt = Cnt - 1
			WHERE	CID = @CID 
			AND		CIID = @CIID 
			
		END
		ELSE BEGIN
		
			DELETE  CharacterItem 
			WHERE	CID = @CID 
			AND		CIID = @CIID
			
		END

        ---------------------------------------------------------------------------------------	

        DECLARE @OrderCIID INT;

        SELECT  @OrderCIID = CIID
        FROM    CharacterItem CI(NOLOCK)
        WHERE   CI.CID = @CID
        AND     CI.ItemID = @RewardItemID

        IF( @OrderCIID IS NULL ) BEGIN

            INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
            VALUES (@CID, @RewardItemID, @NowDate, @NowDate, 0, @RewardCount)

            SET @OrderCIID = @@IDENTITY;

        END
        ELSE BEGIN

            UPDATE	CharacterItem 
            SET		Cnt = Cnt + @RewardCount
            WHERE	CID = @CID 
            AND		CIID = @OrderCIID 

        END
		
	COMMIT TRAN  -----------

    SET XACT_ABORT OFF;
	

	---------------------------------------------------------------------------------------	
		
	INSERT dbo.ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, ItemID, [Count])
	VALUES (202, @NowDate, @CID, @CIID, @GIID, 1);	

	INSERT dbo.ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, ItemID, [Count])
	VALUES (102, @NowDate, @CID, @OrderCIID, @RewardItemID, @RewardCount);

	INSERT INTO LogDB..GambleLog(CID, GIID, RewardItemID, RegDate, RentHourPeriod, ItemCnt)
	VALUES (@CID, @GIID, @RewardItemID, @NowDate, 0, @RewardCount);
	
	----------------------------------------------------------------------------------------

	SELECT @OrderCIID AS 'ORDERCIID'
END  
GO

-------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spBringAccountItem  
    @AID    INT,
    @CID    INT,
    @AIID   INT 
AS BEGIN  

SET NOCOUNT ON  

    ------------------------------------------------------------------------------------------------------------  

    DECLARE @ItemID         INT      
    DECLARE @RentDate       DATETIME      
    DECLARE @RentHourPeriod SMALLINT      
    DECLARE @Cnt            SMALLINT  

    SELECT  @ItemID = ItemID, @RentDate = RentDate, 
            @RentHourPeriod = RentHourPeriod, @Cnt = ISNULL(Cnt, 1)  
    FROM    dbo.AccountItem(NOLOCK)  
    WHERE   AIID = @AIID  
    AND     AID = @AID;  

    IF( @ItemID IS NULL ) BEGIN  
        SELECT -1 AS 'Ret'  
        RETURN;  
    END  

    ------------------------------------------------------------------------------------------------------------  

    DECLARE @NowDate DATETIME  
    SELECT @NowDate = GETDATE();  

    DECLARE @OrderCIID INT  

    BEGIN TRAN ----------------  

        DELETE AccountItem   
        WHERE AIID = @AIID AND AID = @AID;  

        IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN   
            ROLLBACK TRAN;
            SELECT -2 AS 'Ret'  
            RETURN;
        END  


        INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)  
        VALUES (@CID, @ItemID, @NowDate, @RentDate, @RentHourPeriod, @Cnt);  

        SET @OrderCIID = @@IDENTITY;  

        IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN        
            ROLLBACK TRAN  
            SELECT -3 AS 'Ret'  
            RETURN       
        END  

    COMMIT TRAN ---------------

    INSERT INTO BringAccountItemLog (ItemID, AID, CID, Date)    
    VALUES (@ItemID, @AID, @CID, GETDATE());

    INSERT INTO ItemChangeLog_AccountItem(ChangeType, ChangeDate, AID, AIID, CID, CIID, ItemID, [Count])  
    VALUES (200, @NowDate, @AID, @AIID, @CID, @OrderCIID, @ItemID, @Cnt);  

    INSERT INTO ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, AID, AIID, ItemID, [Count])  
    VALUES (100, @NowDate, @CID, @OrderCIID, @AID, @AIID, @ItemID, @Cnt);     

    SELECT  0 AS 'Ret',
            @OrderCIID AS ORDERCIID, @ItemID AS ItemID,
            @Cnt AS Cnt,
            @RentHourPeriod as 'RentHourPeriod',
            (@RentHourPeriod*60) - (DateDiff(n, @RentDate, GETDATE())) AS RentPeriodRemainder  
END  
GO

-------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spBringAccountItemStackable
    @AID    INT,  
    @CID    INT,  
    @AIID   INT,    
    @CIID   INT,  
    @Count  INT
AS BEGIN  

    SET NOCOUNT ON  

    ------------------------------------------------------------------------------------------------------------  

    DECLARE @ItemID         INT      
    DECLARE @RentDate       DATETIME      
    DECLARE @RentHourPeriod SMALLINT      
    DECLARE @Cnt            SMALLINT  

    SELECT  @ItemID = ItemID, @RentDate = RentDate, 
            @RentHourPeriod = RentHourPeriod, @Cnt = ISNULL(Cnt, 1)  
    FROM    dbo.AccountItem(NOLOCK)  
    WHERE   AIID = @AIID  
    AND     AID = @AID;  

    IF( @ItemID IS NULL ) BEGIN  
        SELECT -1 AS 'Ret'  
        RETURN;  
    END  

    ------------------------------------------------------------------------------------------------------------  

    DECLARE @NowDate DATETIME  
    SELECT @NowDate = GETDATE();  

    DECLARE @OrderCIID INT  

    BEGIN TRAN ----------------  

        IF ( @Cnt = @Count ) 
        BEGIN  
        
            DELETE AccountItem WHERE AIID = @AIID AND AID = @AID;  

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN   
                ROLLBACK TRAN       
                SELECT -2 AS 'Ret'  
                RETURN      
            END  

        END  
        ELSE 
        BEGIN  

            UPDATE  AccountItem   
            SET     Cnt = @Cnt - @Count   
            WHERE   AIID = @AIID   
            AND     AID = @AID  
            AND     Cnt - @Count > 0;  

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
                ROLLBACK TRAN       
                SELECT -3 AS 'Ret'  
                RETURN   
            END  

        END       

        IF( @CIID > 0 ) BEGIN  

            UPDATE CharacterItem   
            SET  Cnt = Cnt + @Count   
            WHERE CIID = @CIID  
            AND  CID = @CID;  

            SET @OrderCIID = @CIID;  

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN        
                ROLLBACK TRAN  
                SELECT -4 AS 'Ret'    
                RETURN       
            END     

        END   
        ELSE 
        BEGIN    

            INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)  
            VALUES (@CID, @ItemID, @NowDate, @RentDate, @RentHourPeriod, @Count);  

            SET @OrderCIID = @@IDENTITY;  

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN        
                ROLLBACK TRAN  
                SELECT -5 AS 'Ret'  
                RETURN       
            END  

        END   

    COMMIT TRAN ---------------   

    INSERT INTO BringAccountItemLog (ItemID, AID, CID, Date)    
    VALUES (@ItemID, @AID, @CID, GETDATE());  

    INSERT INTO ItemChangeLog_AccountItem(ChangeType, ChangeDate, AID, AIID, CID, CIID, ItemID, [Count])  
    VALUES (200, @NowDate, @AID, @AIID, @CID, @OrderCIID, @ItemID, @Count);  

    INSERT INTO ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, AID, AIID, ItemID, [Count])  
    VALUES (100, @NowDate, @CID, @OrderCIID, @AID, @AIID, @ItemID, @Count);  


    SELECT  0 AS 'Ret',
            @OrderCIID AS ORDERCIID, @ItemID AS ItemID,
            @Count AS Cnt,
            @RentHourPeriod as 'RentHourPeriod',
            (@RentHourPeriod*60) - (DateDiff(n, @RentDate, GETDATE())) AS RentPeriodRemainder
END  
GO

------------------------------------------------------------------------------------------------------------  

CREATE PROC dbo.spBringBackAccountItem  
    @AID   INT,  
    @CID   INT,  
    @CIID  INT  
AS BEGIN

    SET NOCOUNT ON  

    DECLARE @ItemID         INT      
    DECLARE @RentDate       DATETIME      
    DECLARE @RentHourPeriod SMALLINT      
    DECLARE @Cnt            SMALLINT  

    DECLARE @CurDate DATETIME;  
    SET @CurDate = GETDATE();  

    SELECT  @ItemID = ItemID, @RentDate = RentDate, 
            @RentHourPeriod = RentHourPeriod , @Cnt = Cnt
    FROM    CharacterItem   
    WHERE   CIID = @CIID  
    AND     CID = @CID;  

    IF( (@ItemID IS NULL) AND (@ItemID < 400000) ) BEGIN  
        SELECT -1 AS 'Ret';  
        RETURN;  
    END   

    IF( EXISTS(SELECT CID FROM CharacterEquipmentSlot WHERE CIID = @CIID AND CID = @CID) ) BEGIN  
        SELECT -2 AS 'Ret';  
        RETURN;  
    END  

    DECLARE @OrderAIID INT;  

    BEGIN TRAN -------------  

        DELETE  CharacterItem
        WHERE   CIID = @CIID  
        AND     CID = @CID;  

        IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
            ROLLBACK TRAN;  
            SELECT -3 AS 'Ret';  
            RETURN;
        END  

        INSERT INTO AccountItem (AID, ItemID, RentDate, RentHourPeriod, Cnt)     
        VALUES (@AID, @ItemID, @RentDate, @RentHourPeriod, @Cnt);  
    
        SET @OrderAIID = @@IDENTITY;  

        IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
            ROLLBACK TRAN;
            SELECT -4 AS 'Ret';
            RETURN;
        END   

    COMMIT TRAN -----------    

    SELECT 0 AS 'Ret';
END  
GO
------------------------------------------------------------------------------------------------------------  

CREATE PROC dbo.spBringBackAccountItemStackable
    @AID        INT,
    @CID        INT,
    @CIID       INT,
    @ItemCnt    INT
AS BEGIN
 
    SET NOCOUNT ON  

    DECLARE @ItemID         INT
    DECLARE @RentDate       DATETIME
    DECLARE @RentHourPeriod SMALLINT
    DECLARE @Cnt            SMALLINT

    DECLARE @CurDate DATETIME;  
    SET @CurDate = GETDATE();  

    SELECT  @ItemID = ItemID, @RentDate = RentDate, 
            @RentHourPeriod = RentHourPeriod, @Cnt = Cnt
    FROM    CharacterItem   
    WHERE   CIID = @CIID  
    AND     CID = @CID;  

    IF( (@ItemID IS NULL) AND (@ItemID < 400000) ) BEGIN  
        SELECT -1 AS 'Ret';  
        RETURN;  
    END   

    IF( EXISTS(SELECT CID FROM CharacterEquipmentSlot WHERE CIID = @CIID AND CID = @CID) ) BEGIN  
        SELECT -2 AS 'Ret';  
        RETURN;  
    END  

    BEGIN TRAN -------------  
    
        DECLARE @AIID INT;
        SELECT  @AIID = AIID
        FROM    AccountItem(NOLOCK)
        WHERE   AID = @AID
        AND     ItemID = @ItemID;

        IF( @AIID IS NULL ) BEGIN
            -- 아이템 삽입
            INSERT INTO AccountItem (AID, ItemID, RentDate, RentHourPeriod, Cnt)     
            VALUES (@AID, @ItemID, @RentDate, @RentHourPeriod, @ItemCnt); 
 
            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
                ROLLBACK TRAN;  
                SELECT -3 AS 'Ret';  
                RETURN;    
            END   
        END
        ELSE BEGIN
            -- 아이템 수량 증가
            UPDATE  AccountItem
            SET     Cnt = Cnt + @ItemCnt
            WHERE   AID = @AID
            AND     AIID = @AIID;

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
                ROLLBACK TRAN;  
                SELECT -4 AS 'Ret';  
                RETURN;    
            END   
        END

        IF( @Cnt = @ItemCnt ) BEGIN
            -- 아이템 삭제
            DELETE  CharacterItem
            WHERE   CIID = @CIID  
            AND     CID = @CID;  

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
                ROLLBACK TRAN;  
                SELECT -5 AS 'Ret';  
                RETURN;  
            END  
        END
        ELSE BEGIN
            -- 아이템 수량 감소
            UPDATE  CharacterItem   
            SET     Cnt = @Cnt - @ItemCnt   
            WHERE   CIID = @CIID   
            AND     CID = @CID  
            AND     Cnt - @ItemCnt > 0;  

            IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
                ROLLBACK TRAN;  
                SELECT -6 AS 'Ret';  
                RETURN;  
            END  
        END


    COMMIT TRAN -----------    

    SELECT 0 AS 'Ret';  
END  
GO