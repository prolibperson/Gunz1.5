USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spBringAccountItem', 'BackUp_spBringAccountItem';
GO

CREATE PROC dbo.spBringAccountItem
-- ALTER PROC dbo.spBringAccountItem
	@AID	INT,
	@CID	INT,
	@AIID	INT,		
	@CIID	INT,
	@Count	INT
AS BEGIN

	SET NOCOUNT ON

	------------------------------------------------------------------------------------------------------------

	DECLARE @CAID INT;	
	SELECT @CAID = AID FROM dbo.Character(NOLOCK) WHERE CID = @CID
	
	IF( @CAID != @AID ) BEGIN
		SELECT -1 AS 'Ret'
		RETURN;
	END
		
		
	DECLARE @ItemID				INT    
	DECLARE @RentDate			DATETIME    
	DECLARE @RentHourPeriod		SMALLINT    
	DECLARE @Cnt				SMALLINT
	
	SELECT	@ItemID = ItemID, @RentDate = RentDate, @RentHourPeriod = RentHourPeriod, @Cnt = ISNULL(Cnt, 1)
	FROM	dbo.AccountItem(NOLOCK)
	WHERE	AIID = @AIID
	AND		AID = @AID;
		
	IF( @ItemID IS NULL ) BEGIN
		SELECT -2 AS 'Ret'
		RETURN;
	END
	
	------------------------------------------------------------------------------------------------------------

		
	DECLARE @NowDate DATETIME
	SELECT @NowDate = GETDATE();
	
	DECLARE @OrderCIID	INT
	
	BEGIN TRAN ----------------
	
		IF ( @Cnt = @Count ) BEGIN
			
			DELETE	AccountItem 
			WHERE	AIID = @AIID AND AID = @AID;
			
			IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN 
				ROLLBACK TRAN     
				SELECT -3 AS 'Ret'
				RETURN    
			END
			
		END
		ELSE BEGIN
		
			UPDATE	AccountItem 
			SET		Cnt = @Cnt - @Count 
			WHERE	AIID = @AIID 
			AND		AID = @AID
			AND		Cnt - @Count > 0;
			
			IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
				ROLLBACK TRAN     
				SELECT -4 AS 'Ret'
				RETURN 
			END
			
		END 				
		
		IF( @CIID > 0 ) BEGIN
		
			UPDATE	CharacterItem 
			SET		Cnt = Cnt + @Count 
			WHERE	CIID = @CIID
			AND		CID = @CID;
				
			SET @OrderCIID = @CIID;
			
			IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
				ROLLBACK TRAN
				SELECT -6 AS 'Ret'  
				RETURN     
			END			
			
		END 
		ELSE BEGIN		
		
			INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
			VALUES (@CID, @ItemID, @NowDate, @RentDate, @RentHourPeriod, @Count);
			
			SET @OrderCIID = @@IDENTITY;
			
			IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
				ROLLBACK TRAN
				SELECT -7 AS 'Ret'
				RETURN     
			END
						
		END	
		
		 INSERT INTO BringAccountItemLog (ItemID, AID, CID, Date)  
		 VALUES (@ItemID, @AID, @CID, GETDATE());
		 IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN
			 ROLLBACK TRAN  
			 SELECT -8 AS 'Ret'
			 RETURN  
		 END  
 
		INSERT INTO ItemChangeLog_AccountItem(ChangeType, ChangeDate, AID, AIID, CID, CIID, ItemID, [Count])
		VALUES (200, @NowDate, @AID, @AIID, @CID, @OrderCIID, @ItemID, @Count);
		
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
			ROLLBACK TRAN
			SELECT -9 AS 'Ret'
			RETURN     
		END
		
		INSERT INTO ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, AID, AIID, ItemID, [Count])
		VALUES (100, @NowDate, @CID, @OrderCIID, @AID, @AIID, @ItemID, @Count);
		
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
			ROLLBACK TRAN
			SELECT -10 AS 'Ret'
			RETURN     
		END
		

	COMMIT TRAN ---------------   	 
		

	SELECT	0 AS 'Ret'
			, @OrderCIID AS ORDERCIID, @ItemID AS ItemID  
			, @Count AS Cnt
			, @RentHourPeriod as 'RentHourPeriod'  
			, (@RentHourPeriod*60) - (DateDiff(n, @RentDate, GETDATE())) AS RentPeriodRemainder
END
GO

-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spBringAccountItem
EXEC sp_rename 'BackUp_spBringAccountItem', 'spBringAccountItem';
*/