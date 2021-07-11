USE GunzDB
GO

SELECT * FROM LogDB.dbo.GameLog
SELECT * FROM LogDB.dbo.GamePlayerLog
EXEC SP_HELPTEXT spCharUpdatePlayInfo
CREATE PROC dbo.spInsertGameLog2
-- ALTER PROC dbo.spInsertGameLog2
    @MasterCID      INT
    , @MapName      VARCHAR(32)
    , @GameType     VARCHAR(24)
AS BEGIN

    SET NOCOUNT ON;

    INSERT INTO LogDB.dbo.GameLog(MasterCID, MapName, GameType)
    VALUES (@MasterCID, @MapName, @GameType);

    SELECT @@IDENTITY AS GameLogID;

END
GO

------------------------------------------------------------------------------------------

CREATE PROC dbo.InsertGamePlayerLog
-- ALTER PROC dbo.InsertGamePlayerLog
    @GameLogID      INT
    , @CID          INT
    , @PlayTime     INT
    , @Kills        INT
    , @Deaths       INT
    , @XP           INT
    , @BP           INT
AS BEGIN

    SET NOCOUNT ON;

    INSERT INTO LogDB.dbo.GamePlayerLog(ID, CID, PlayTime, Kills, Deaths, XP, BP)
    VALUES (@GameLogID, @CID, @PlayTime, @Kills, @Deaths, @XP, @BP);

END
GO


SELECT * FROM ClanGameLog
SELECT * FROM Item

GITEM_ID


SELECT * FROM GameLog