USE IGunzDB
Go

--------------------------------------------------------------------------------------------------------------------------------------------
/*
DROP TABLE BattleTimeRewardDescription
DROP TABLE BattleTimeRewardTerm
DROP TABLE BattleTimeRewardItemList
DROP TABLE CharacterBattleTimeRewardInfo
DROP TABLE CharacterBattleTimeRewardLog
*/
--------------------------------------------------------------------------------------------------------------------------------------------

CREATE TABLE dbo.BattleTimeRewardDescription
(
	BRID                    INT             IDENTITY(1, 1)
    , [Name]                VARCHAR(128)
    , StartDate             DATETIME        NOT NULL
    , EndDate               DATETIME        NOT NULL
    , StartHour             TINYINT         DEFAULT(0)
    , EndHour               TINYINT         DEFAULT(24)
    , RewardMinutePeriod    INT             NOT NULL
    , RewardCount           TINYINT         NOT NULL
    , RewardKillCount       TINYINT         NOT NULL
    , ResetCode             CHAR(7)         NOT NULL
    , ResetDesc             VARCHAR(128)
    , IsOpen                TINYINT         DEFAULT(0)

    CONSTRAINT PK_BattleTimeRewardDescription PRIMARY KEY CLUSTERED (BRID)
)
GO

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE TABLE dbo.BattleTimeRewardTerm
(
	BRID                    INT             NOT NULL
    , BRTID                 INT             IDENTITY(1, 1)
    , LastResetDate         DATETIME        NOT NULL
    , ClosedDate            DATETIME        NULL

    CONSTRAINT PK_BattleTimeRewardTerm PRIMARY KEY CLUSTERED (BRID, BRTID)
)
GO

ALTER TABLE dbo.BattleTimeRewardTerm WITH CHECK ADD CONSTRAINT FK_BattleTimeRewardTerm_BRID
FOREIGN KEY(BRID) REFERENCES dbo.BattleTimeRewardDescription(BRID)

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE TABLE dbo.BattleTimeRewardItemList
(	
    BRIID                   INT             IDENTITY(1, 1)
    , BRID                  INT             NOT NULL
    , ItemIDMale            INT             NOT NULL
    , ItemIDFemale          INT             NOT NULL
    , RentHourPeriod        INT             DEFAULT(0)    
    , ItemCnt               INT             DEFAULT(1)    
    , RatePerThousand       INT             NOT NULL
    
    CONSTRAINT PK_BattleTimeRewardItemList PRIMARY KEY CLUSTERED (BRIID)   
)

ALTER TABLE dbo.BattleTimeRewardItemList WITH CHECK ADD CONSTRAINT FK_BattleTimeRewardItemList_BRID
FOREIGN KEY(BRID) REFERENCES dbo.BattleTimeRewardDescription(BRID)

ALTER TABLE dbo.BattleTimeRewardItemList WITH CHECK ADD CONSTRAINT FK_BattleTimeRewardItemList_ItemIDMale
FOREIGN KEY(ItemIDMale) REFERENCES dbo.Item(ItemID)

ALTER TABLE dbo.BattleTimeRewardItemList WITH CHECK ADD CONSTRAINT FK_BattleTimeRewardItemList_ItemIDFemale
FOREIGN KEY(ItemIDFemale) REFERENCES dbo.Item(ItemID)

CREATE NONCLUSTERED INDEX IX_BattleTimeRewardItemList_BRID ON dbo.BattleTimeRewardItemList(BRID)

--------------------------------------------------------------------------------------------------------------------------------------------

CREATE TABLE dbo.CharacterBattleTimeRewardInfo
(
    CID                 INT         NOT NULL
    , BRID              INT         NOT NULL
    , BRTID             INT         NOT NULL
    , BattleTime        INT         DEFAULT(0)
    , RewardCount       INT         DEFAULT(0)
    , KillCount         INT         DEFAULT(0)
    , LastUpdatedTime   DATETIME    DEFAULT(GETDATE())

    CONSTRAINT PK_CharacterBattleTimeRewardInfo PRIMARY KEY CLUSTERED (CID, BRID)
)
GO

ALTER TABLE dbo.CharacterBattleTimeRewardInfo WITH CHECK ADD CONSTRAINT FK_CharacterBattleTimeRewardInfo_BRID
FOREIGN KEY(BRID) REFERENCES dbo.BattleTimeRewardDescription(BRID)

--------------------------------------------------------------------------------------------------------------------------------------------


CREATE TABLE dbo.CharacterBattleTimeRewardLog
(
    CID                 INT         NOT NULL
    , BRID              INT         NOT NULL
    , BRTID             INT         NOT NULL
    , RegDate           DATETIME    DEFAULT(GETDATE())
    , BattleTime        INT         NOT NULL
    , KillCount         INT         NOT NULL
    , ItemID            INT         NOT NULL
    , ItemCnt           INT         NOT NULL
    , RentHourPeriod    INT         NOT NULL
)
GO

CREATE CLUSTERED INDEX IX_CharacterBattleTimeRewardLog_RegDate ON CharacterBattleTimeRewardLog(RegDate);
CREATE NONCLUSTERED INDEX IX_CharacterBattleTimeRewardLog_BRID_CID ON CharacterBattleTimeRewardLog(BRID, CID);

--------------------------------------------------------------------------------------------------------------------------------------------