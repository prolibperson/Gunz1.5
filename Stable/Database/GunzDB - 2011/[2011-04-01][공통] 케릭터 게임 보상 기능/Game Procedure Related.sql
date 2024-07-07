USE IGunzDB
Go

--------------------------------------------------------------------------------------------------------------------------------------------
/*
DROP PROC dbo.spGetBattleTimeRewardDescription;
DROP PROC dbo.spGetBattleTimeRewardItemList;
DROP PROC dbo.spGetCharBRInfoAll;
DROP PROC dbo.spGetCharBRInfo;
DROP PROC dbo.spInsertCharBRInfo;
DROP PROC dbo.spUpdateCharBRInfo;
DROP PROC dbo.spRewardCharBattleTimeReward;
*/
--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spGetBattleTimeRewardDescription
-- ALTER PROC dbo.spGetBattleTimeRewardDescription
AS BEGIN
    
    SET NOCOUNT ON;

    SELECT  BR.BRID, BR.[Name], T1.BRTID, BR.RewardMinutePeriod, BR.RewardCount, BR.RewardKillCount, BR.ResetDesc
    FROM    dbo.BattleTimeRewardDescription BR(NOLOCK) 
            JOIN
            (
                SELECT  BRID, MAX(BRTID) AS BRTID
                FROM    dbo.BattleTimeRewardTerm(NOLOCK)
                WHERE   ClosedDate IS NULL
                GROUP BY BRID
            ) T1 
            ON T1.BRID = BR.BRID
    WHERE   GETDATE() BETWEEN BR.StartDate AND BR.EndDate
    AND     DATEPART(hh, GETDATE()) BETWEEN BR.StartHour AND BR.EndHour
    AND     BR.IsOpen = 1
    ORDER BY BR.BRID
    
END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spGetBattleTimeRewardItemList
-- ALTER PROC dbo.spGetBattleTimeRewardItemList
AS BEGIN

    SET NOCOUNT ON;

    WITH T1 AS
    (
        SELECT  BRI.BRID
        FROM    BattleTimeRewardItemList BRI(NOLOCK)
        GROUP BY BRI.BRID HAVING SUM(BRI.RatePerThousand) = 1000
    ), 
    T2 AS
    (
        SELECT  BRI.BRID, BRI.BRIID, BRI.ItemIDMale, BRI.ItemIDFemale, BRI.ItemCnt, BRI.RentHourPeriod, BRI.RatePerThousand
        FROM    dbo.BattleTimeRewardDescription BR(NOLOCK)
                JOIN dbo.BattleTimeRewardItemList BRI(NOLOCK)
                ON BR.BRID = BRI.BRID
        WHERE   GETDATE() BETWEEN BR.StartDate AND BR.EndDate
        AND     DATEPART(hh, GETDATE()) BETWEEN BR.StartHour AND BR.EndHour
        AND     BR.IsOpen = 1
    )
    SELECT  T2.BRID, T2.BRIID, T2.ItemIDMale, T2.ItemIDFemale, T2.ItemCnt, T2.RentHourPeriod, T2.RatePerThousand
    FROM    T1 JOIN T2 ON T1.BRID = T2.BRID
    ORDER BY T2.BRIID

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spGetCharBRInfoAll
-- ALTER PROC dbo.spGetCharBRInfoAll
    @CID        INT
AS BEGIN

    SET NOCOUNT ON;

    WITH T1 AS
    (
        SELECT  CBRI.BRID, CBRI.BRTID, CBRI.BattleTime, CBRI.RewardCount, CBRI.KillCount
        FROM    dbo.CharacterBattleTimeRewardInfo CBRI(NOLOCK)
        WHERE   CBRI.CID = @CID
    ),
    T2 AS
    (   
        SELECT  BRT.BRID, MAX(BRT.BRTID) AS BRTID
        FROM    dbo.BattleTimeRewardTerm BRT(NOLOCK)
                JOIN dbo.BattleTimeRewardDescription BRD(NOLOCK)
                ON BRT.BRID = BRD.BRID
        WHERE   GETDATE() BETWEEN BRD.StartDate AND BRD.EndDate
        AND     DATEPART(hh, GETDATE()) BETWEEN BRD.StartHour AND BRD.EndHour
        AND     BRD.IsOpen = 1 
        AND     BRT.ClosedDate IS NULL   
        GROUP BY BRT.BRID
    )    
    SELECT  T1.BRID, T2.BRTID
            , CASE WHEN T1.BRTID = T2.BRTID   THEN T1.BattleTime
                                              ELSE 0 END AS 'BattleTime'
            , CASE WHEN T1.BRTID = T2.BRTID   THEN T1.RewardCount
                                              ELSE 0 END AS 'RewardCount'
            , CASE WHEN T1.BRTID = T2.BRTID   THEN T1.KillCount
                                              ELSE 0 END AS 'KillCount'
    FROM    T1 JOIN T2 ON T1.BRID = T2.BRID
    
END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spGetCharBRInfo
-- ALTER PROC dbo.spGetCharBRInfo
    @CID        INT
    , @BRID     INT
AS BEGIN

    SET NOCOUNT ON;

    WITH T1 AS
    (
        SELECT  CBRI.BRID, CBRI.BRTID, CBRI.BattleTime, CBRI.RewardCount, CBRI.KillCount
        FROM    dbo.CharacterBattleTimeRewardInfo CBRI(NOLOCK)
        WHERE   CBRI.CID = @CID
        AND     CBRI.BRID = @BRID
    ),
    T2 AS
    (   
        SELECT  BRT.BRID, MAX(BRT.BRTID) AS BRTID
        FROM    dbo.BattleTimeRewardTerm BRT(NOLOCK)
                JOIN dbo.BattleTimeRewardDescription BRD(NOLOCK)
                ON BRT.BRID = BRD.BRID
        WHERE   GETDATE() BETWEEN BRD.StartDate AND BRD.EndDate
        AND     DATEPART(hh, GETDATE()) BETWEEN BRD.StartHour AND BRD.EndHour
        AND     BRD.IsOpen = 1    
        AND     BRT.ClosedDate IS NULL   
        GROUP BY BRT.BRID
    )    
    SELECT  T1.BRID, T2.BRTID
            , CASE WHEN T1.BRTID = T2.BRTID   THEN T1.BattleTime
                                              ELSE 0 END AS 'BattleTime'
            , CASE WHEN T1.BRTID = T2.BRTID   THEN T1.RewardCount
                                              ELSE 0 END AS 'RewardCount'
            , CASE WHEN T1.BRTID = T2.BRTID   THEN T1.KillCount
                                              ELSE 0 END AS 'KillCount'
    FROM    T1 JOIN T2 ON T1.BRID = T2.BRID

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spInsertCharBRInfo
-- ALTER PROC dbo.spInsertCharBRInfo
    @CID        INT
    , @BRID     INT
    , @BRTID    INT
AS BEGIN

    SET NOCOUNT ON;

    INSERT dbo.CharacterBattleTimeRewardInfo(CID, BRID, BRTID)
    VALUES (@CID, @BRID, @BRTID);

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spUpdateCharBRInfo
-- ALTER PROC dbo.spUpdateCharBRInfo
    @CID            INT
    , @BRID         INT
    , @BRTID        INT
    , @RewardCount  INT
    , @BattleTime   INT
    , @KillCount    INT
AS BEGIN

    SET NOCOUNT ON;

    UPDATE  dbo.CharacterBattleTimeRewardInfo
    SET     BRTID = @BRTID, BattleTime = @BattleTime, RewardCount = @RewardCount,
            KillCount = @KillCount, LastUpdatedTime = GETDATE()
    WHERE   CID = @CID
    AND     BRID = @BRID;

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spRewardCharBattleTimeReward
-- ALTER PROC dbo.spRewardCharBattleTimeReward
    @CID                    INT
    , @BRID                 INT
    , @BRTID                INT
    , @BattleTime           INT
    , @KillCount            INT
    , @ItemID               INT
    , @ItemCnt              INT
    , @RentHourPeriod       INT
    , @IsSpendableItem      INT
AS BEGIN

    SET NOCOUNT ON;

    DECLARE @OrderCIID INT;

    BEGIN TRAN ----

        -- 업데이트 하고
        UPDATE  dbo.CharacterBattleTimeRewardInfo
        SET     RewardCount = CASE WHEN BRTID = @BRTID  THEN RewardCount + 1 
                                                        ELSE 1 END,
                BRTID = @BRTID, BattleTime = 0, KillCount = 0, LastUpdatedTime = GETDATE()
        WHERE   CID = @CID
        AND     BRID = @BRID

        IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
            ROLLBACK TRAN;
            SELECT -1 AS 'Ret'
            RETURN;
        END

        IF( @IsSpendableItem = 1 ) BEGIN
					
			-- 이미 갖고 있는지 확인해본다.
			SELECT	@OrderCIID = CIID 
			FROM	CharacterItem(NOLOCK) 
			WHERE	CID = @CID 
			AND		ItemID = @ItemID;

            -- 이미 갖고 있지 않다면 새로 추가해준다.
            IF( @OrderCIID IS NOT NULL ) BEGIN

	            UPDATE	dbo.CharacterItem				-- Item 추가
	            SET		Cnt = Cnt + @ItemCnt
	            WHERE	CIID = @OrderCIID
	            AND		CID = @CID;
            	
	            IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
		            ROLLBACK TRAN
		            SELECT -2 AS 'Ret'
		            RETURN;
	            END
            							
            END ELSE BEGIN

	            INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
	            VALUES (@CID, @ItemID, GETDATE(), GETDATE(), @RentHourPeriod, @ItemCnt)
            	
	            IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
		            ROLLBACK TRAN
		            SELECT -3 AS 'Ret'
		            RETURN;
	            END
            	
	            SET @OrderCIID = @@IDENTITY;	
            END
				
        END
        ELSE BEGIN
        		
	        INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
            VALUES (@CID, @ItemID, GETDATE(), GETDATE(), @RentHourPeriod, @ItemCnt)
        	
	        SET @OrderCIID = @@IDENTITY;	
        	
	        IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
		        ROLLBACK TRAN
		        SELECT -4 AS 'Ret'
		        RETURN;
	        END
        				
        END 

        -- 로그 남기고
        INSERT dbo.CharacterBattleTimeRewardLog(CID, BRID, BRTID, BattleTime, KillCount, ItemID, ItemCnt, RentHourPeriod)
        VALUES (@CID, @BRID, @BRTID, @BattleTime, @KillCount, @ItemID, @ItemCnt, @RentHourPeriod);

        IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
            ROLLBACK TRAN;
            SELECT -5 AS 'Ret'
            RETURN;
        END        

    COMMIT TRAN ---

    SELECT @OrderCIID AS 'Ret';

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

/*

EXEC spInsertCharBRInfo 1271, 1, 1

EXEC spGetBattleTimeRewardDescription
EXEC spGetBattleTimeRewardItemList

EXEC spGetCharBRInfoAll 1271
EXEC spGetCharBRInfo 12,1



*/

EXEC spRewardCharBattleTimeReward 1271, 1, 100000, 40103, 100, 0, 1

EXEC spRefreshBattleTimeRewardDescription




EXEC spGetBattleTimeRewardDescription

SELECT * FROM CharacterBattleTimeRewardInfo

EXEC spGetCharBRInfoAll 1271

SELECT  C.Name, C.CID, CRL.RegDate, CRL.BRTID,
        (SELECT Name FROM Item WHERE ItemID = CRL.ItemID) AS ItemName, 
        CRL.ItemCnt, CRL.RentHourPeriod, CRL.BattleTime
FROM    CharacterBattleTimeRewardLog CRL(NOLOCK)
        JOIN Character C(NOLOCK)
        ON CRL.CID = C.CID
ORDER BY CRL.CID, CRL.RegDate



SELECT * FROM CharacterBattleTimeRewardInfo

SELECT * FROM BattleTimeRewardDescription

