USE IGunzDB
Go

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spAdmWebInsertBattleTimeRewardDescription
-- ALTER PROC dbo.spAdmWebInsertBattleTimeRewardDescription
    @Name                   VARCHAR(128)
    , @StartDate            DATETIME
    , @EndDate              DATETIME
    , @StartHour            INT
    , @EndHour              INT
    , @RewardMinutePeriod   INT
    , @RewardCount          SMALLINT
    , @RewardKillCount      SMALLINT
    , @ResetCode            CHAR(7)
    , @ResetDesc            VARCHAR(128)
    , @IsOpen               TINYINT
AS BEGIN

    SET NOCOUNT ON;

    IF( @StartDate > @EndDate ) BEGIN
        SELECT -1 AS 'Ret';
        RETURN;
    END

    BEGIN TRAN -----------

        INSERT dbo.BattleTimeRewardDescription
            ([Name], StartDate, EndDate, StartHour, EndHour, RewardMinutePeriod, RewardCount, RewardKillCount, ResetCode, ResetDesc, IsOpen)
        VALUES(@Name, @StartDate, @EndDate, @StartHour, @EndHour, @RewardMinutePeriod, @RewardCount, @RewardKillCount, @ResetCode, @ResetDesc, @IsOpen);

        IF( @@ERROR <> 0 ) BEGIN
            ROLLBACK TRAN;
            SELECT -2 AS 'Ret';
            RETURN;
        END

        INSERT dbo.BattleTimeRewardTerm(BRID, LastResetDate)
        VALUES(@@IDENTITY, GETDATE());

        IF( @@ERROR <> 0 ) BEGIN
            ROLLBACK TRAN;
            SELECT -3 AS 'Ret';
            RETURN;
        END

    COMMIT TRAN ----------

    SELECT 0 AS 'Ret';
    
END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spAdmWebUpdateBattleTimeRewardDescription
-- ALTER PROC dbo.spAdmWebUpdateBattleTimeRewardDescription
    @BRID                   INT
    , @Name                 VARCHAR(128)
    , @StartDate            DATETIME
    , @EndDate              DATETIME
    , @StartHour            INT
    , @EndHour              INT
    , @RewardMinutePeriod   INT
    , @RewardCount          SMALLINT
    , @RewardKillCount      SMALLINT
    , @ResetCode            CHAR(7)
    , @ResetDesc            VARCHAR(128)
    , @IsOpen               TINYINT
    , @IsReset              TINYINT
AS BEGIN

    SET NOCOUNT ON;

    IF( @StartDate > @EndDate ) BEGIN
        SELECT -1 AS 'Ret';
        RETURN;
    END


    BEGIN TRAN -----------

        UPDATE  dbo.BattleTimeRewardDescription
        SET     [Name] = @Name, StartDate = @StartDate, EndDate = @EndDate, StartHour = @StartHour, EndHour = @EndHour,
                RewardMinutePeriod = @RewardMinutePeriod, RewardCount = @RewardCount, RewardKillCount = @RewardKillCount, 
                ResetCode = @ResetCode, ResetDesc = @ResetDesc, IsOpen = @IsOpen
        WHERE   BRID = @BRID

        IF( @@ERROR <> 0 ) BEGIN
            ROLLBACK TRAN;
            SELECT -2 AS 'Ret';
            RETURN;
        END

        IF( @IsReset = 1 ) BEGIN

            UPDATE  dbo.BattleTimeRewardTerm
            SET     ClosedDate = GETDATE()
            WHERE   BRID = @BRID
            AND     BRTID IN    (   SELECT  TOP 1 BRTID 
                                    FROM    dbo.BattleTimeRewardTerm 
                                    WHERE   BRID = @BRID 
                                    ORDER BY BRTID DESC )

            INSERT dbo.BattleTimeRewardTerm(BRID, LastResetDate)
            VALUES(@BRID, GETDATE());

            IF( @@ERROR <> 0 ) BEGIN
                ROLLBACK TRAN;
                SELECT -3 AS 'Ret';
                RETURN;
            END

        END        

    COMMIT TRAN ----------

    SELECT 0 AS 'Ret';
    
END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spAdmWebInsertBattleTimeRewardItemList
-- ALTER PROC dbo.spAdmWebInsertBattleTimeRewardItemList
    @BRID                   INT
    , @ItemIDMale           INT
    , @ItemIDFemale         INT
    , @RentHourPeriod       INT
    , @ItemCnt              INT
    , @RatePerThousand      INT
AS BEGIN

    SET NOCOUNT ON;

    INSERT dbo.BattleTimeRewardItemList(BRID, ItemIDMale, ItemIDFemale, RentHourPeriod, ItemCnt, RatePerThousand)
    VALUES (@BRID, @ItemIDMale, @ItemIDFemale, @RentHourPeriod, @ItemCnt, @RatePerThousand);
    
END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spAdmWebUpdateBattleTimeRewardItemList
-- ALTER PROC dbo.spAdmWebUpdateBattleTimeRewardItemList
    @BRIID                  INT
    , @ItemIDMale           INT
    , @ItemIDFemale         INT
    , @RentHourPeriod       INT
    , @ItemCnt              INT
    , @RatePerThousand      INT
AS BEGIN

    SET NOCOUNT ON;

    UPDATE  dbo.BattleTimeRewardItemList
    SET     ItemIDMale = @ItemIDMale, ItemIDFemale = @ItemIDFemale,
            RentHourPeriod = @RentHourPeriod, ItemCnt = @ItemCnt, RatePerThousand = @RatePerThousand
    WHERE   BRIID = @BRIID;

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spAdmWebRefreshBattleTimeRewardDescription
-- ALTER PROC dbo.spAdmWebRefreshBattleTimeRewardDescription
AS BEGIN
    
    SET NOCOUNT ON;

    DECLARE @DWIndex    INT,
            @ID         INT,
            @RCode      CHAR(7);

    SELECT @DWIndex = DATEPART(DW, GETDATE());

    DECLARE Curs CURSOR FAST_FORWARD FOR 
        SELECT  BRID, ResetCode
        FROM    dbo.BattleTimeRewardDescription(NOLOCK)
        WHERE   IsOpen = 1
        AND     GETDATE() BETWEEN StartDate AND EndDate


    OPEN Curs
    FETCH NEXT FROM Curs INTO @ID, @RCode;
    		
    WHILE( @@FETCH_STATUS = 0 )
    BEGIN
    	
        IF( SUBSTRING(@RCode, @DWIndex, 1) = 1 )
        BEGIN

            UPDATE  dbo.BattleTimeRewardTerm
            SET     ClosedDate = GETDATE()
            WHERE   BRID = @ID
            AND     BRTID IN    (   SELECT  TOP 1 BRTID 
                                    FROM    dbo.BattleTimeRewardTerm 
                                    WHERE   BRID = @ID
                                    ORDER BY BRTID DESC )

            INSERT  dbo.BattleTimeRewardTerm(BRID, LastResetDate)
            VALUES (@ID, GETDATE());

        END
    	
	    FETCH NEXT FROM Curs INTO @ID, @RCode;
    	
    END

    CLOSE Curs
    DEALLOCATE Curs

END
GO

--------------------------------------------------------------------------------------------------------------------------------------------
