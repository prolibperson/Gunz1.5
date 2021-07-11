USE GunzDB
GO

/*
DROP PROC spStartUpLocatorStatus  
DROP PROC spUpdateLocatorStatus  
DROP PROC spInsertLocatorLog
*/

CREATE PROC dbo.spStartUpLocatorStatus  
    @LocatorID              INT
    , @IP                   VARCHAR(15)  
    , @Port                 INT
    , @UpdateElapsedTime    INT
AS BEGIN

    SET NOCOUNT ON;

    IF EXISTS ( SELECT LocatorID FROM dbo.LocatorStatus(NOLOCK) WHERE LocatorID = @LocatorID ) 
    BEGIN

        UPDATE  dbo.LocatorStatus   
        SET     IP = @IP, Port = @Port, UpdateElapsedTime = @UpdateElapsedTime
        WHERE   LocatorID = @LocatorID  

    END  
    ELSE BEGIN
        INSERT INTO dbo.LocatorStatus(LocatorID, IP, Port, UpdateElapsedTime)
        VALUES (@LocatorID, @IP, @Port, @UpdateElapsedTime)  
    END

END
GO

------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spUpdateLocatorStatus  
    @LocatorID          INT
    , @RecvCount        INT
    , @SendCount        INT
    , @BlockCount       INT
    , @DuplicatedCount  INT
AS BEGIN

    SET NOCOUNT ON;

    UPDATE  dbo.LocatorStatus   
    SET     RecvCount = @RecvCount, SendCount = @SendCount,   
            BlockCount = @BlockCount, DuplicatedCount = @DuplicatedCount
    WHERE   LocatorID = @LocatorID

END
GO

------------------------------------------------------------------------------------------------------------------------

/*
CREATE PROC dbo.spInsertLocatorLog
    @LocatorID      INT
    , @CountryCode3 VARCHAR(3)
    , @Count        INT
AS BEGIN

    SET NOCOUNT ON;

    INSERT INTO dbo.LocatorLogDB.dbo.LocatorLog(LocatorID, CountryCode3, Count)
    VALUES (@LocatorID, @CountryCode3, @Count)

END
*/
 
------------------------------------------------------------------------------------------------------------------------
