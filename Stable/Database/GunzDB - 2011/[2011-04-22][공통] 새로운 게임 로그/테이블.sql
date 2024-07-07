USE LogDB
GO

------------------------------------------------------------------------------------------
/*
DROP TABLE GameLog
DROP TABLE GamePlayerLog
*/
------------------------------------------------------------------------------------------

CREATE TABLE dbo.GameLog
(
    ID              INT             IDENTITY(1, 1)
    , MasterCID     INT             NOT NULL
    , MapName       VARCHAR(32)     NOT NULL
    , GameType      VARCHAR(24)     NOT NULL
    , RegDate       DATETIME        DEFAULT(GETDATE())

    CONSTRAINT PK_GameLog PRIMARY KEY CLUSTERED (ID)
)
GO

CREATE NONCLUSTERED INDEX IX_GameLog_RegDate ON dbo.GameLog(RegDate);

------------------------------------------------------------------------------------------

CREATE TABLE dbo.GamePlayerLog
(
    ID          INT         NOT NULL
    , CID       INT         NOT NULL
    , RegDate   DATETIME    DEFAULT(GETDATE())
    , PlayTime  INT         NOT NULL
    , Kills     INT         NOT NULL
    , Deaths    INT         NOT NULL
    , XP        INT         NOT NULL
    , BP        INT         NOT NULL

    CONSTRAINT PK_GamePlayerLog PRIMARY KEY CLUSTERED (RegDate)
)
GO

ALTER TABLE dbo.GamePlayerLog WITH CHECK ADD CONSTRAINT FK_GamePlayerLog_ID 
FOREIGN KEY(ID) REFERENCES dbo.GameLog(ID)

CREATE NONCLUSTERED INDEX IX_GamePlayerLog_CID  ON dbo.GamePlayerLog(CID)

