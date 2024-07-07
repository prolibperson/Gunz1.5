-- USE GunzDB
USE IGunzDB
GO

CREATE TABLE LogDB.dbo.CharacterItemSpendLog
(
	id			INT			IDENTITY(1, 1)
	, CID		INT			NOT NULL
	, ItemID	INT			NOT NULL
	, SpendDate	DATETIME	NOT NULL
)
GO

CREATE TABLE dbo.CharacterBuffStatus
(
	CBID				INT			IDENTITY(1, 1)
	, CID				INT			NOT NULL
	, ItemID			INT			NULL
	, BuffMinutePeriod	INT			NULL
	, StartPlayTime		INT			NULL
	, StartDate			DATETIME	NULL	
	
	CONSTRAINT CharacterBuffStatus_PK PRIMARY KEY CLUSTERED (CBID)
)
GO

CREATE NONCLUSTERED INDEX IX_CharacterBuffStatus_CID_ItemID ON CharacterBuffStatus(CID, ItemID);
GO


