USE [GunzDB]
GO

/*
	디비 이전과 관련하여 디비 사이즈를 줄이는 목적의 일환으로 
	테이블을 스위칭하여 임시 보관하는 쿼리문
	
	스위칭 대상 테이블 -	ItemPurchaseLogByBounty(22GB)	- 완료
							PlayerLog(18GB)					- 완료
							GameLog(12GB)					- 완료
							ClanGameLog(11GB)				- 완료
							QuestGameLog(9GB)				- 완료
							BringAccountItemLog(7GB)		- 패스
							LevelUpLog(2GB)					- 완료
							QUniqueItemLog(1GB)				- 완료
	
*/
------------------------------------------------------------------------------------

CREATE TABLE dbo.QUniqueItemLogSW
(
	QUILID		INT		IDENTITY(1,1) NOT NULL,
	QGLID		INT		NULL,
	CID			INT		NOT NULL,
	QIID		INT		NOT NULL

	CONSTRAINT PK_QUniqueItemLogSW PRIMARY KEY NONCLUSTERED (QUILID ASC)
)
GO

CREATE CLUSTERED INDEX IX_QUniqueItemLogSW_CID ON dbo.QUniqueItemLogSW(CID)
CREATE NONCLUSTERED INDEX IX_QUniqueItemLogSW_QGLID ON dbo.QUniqueItemLogSW(QGLID)
CREATE NONCLUSTERED INDEX IX_QUniqueItemLogSW_QIID ON dbo.QUniqueItemLogSW(QIID)

-- EXEC sp_helpindex QUniqueItemLogSW
-- DROP TABLE QUniqueItemLogSW

------------------------------------------------------------------------------------

CREATE TABLE dbo.LevelUpLogSW
(
	ID			INT	IDENTITY(1, 1)	NOT NULL,
	CID			INT					NULL,
	Level		SMALLINT			NULL,
	BP			INT					NULL,
	KillCount	INT					NULL,
	DeathCount	INT					NULL,
	PlayTime	INT					NULL,
	Date		DATETIME			NULL,

	CONSTRAINT PK_LevelUpLogSW PRIMARY KEY NONCLUSTERED (ID ASC)
)
GO

CREATE NONCLUSTERED INDEX IX_LevelUpLogSW_Date_CID_Level ON dbo.LevelUpLogSW(Date, CID, Level)

-- EXEC sp_helpindex LevelUpLogSW
-- DROP TABLE LevelUpLogSW

------------------------------------------------------------------------------------

CREATE TABLE dbo.QuestGameLogSW
(
	ID				INT			IDENTITY(1, 1)	NOT NULL,
	GameName		VARCHAR(64)					NULL,
	Master			INT							NOT NULL,
	Player1			INT							NULL,
	Player2			INT							NULL,
	Player3			INT							NULL,
	TotalQItemCount TINYINT						NULL,
	ScenarioID		SMALLINT					NOT NULL,
	StartTime		SMALLDATETIME				NOT NULL,
	EndTime			SMALLDATETIME				NOT NULL,

	CONSTRAINT PK_QuestGameLogSW PRIMARY KEY NONCLUSTERED (ID ASC) 
)
GO

CREATE NONCLUSTERED INDEX IX_QuestGameLogSW_EndTime ON dbo.QuestGameLogSW(EndTime);
CREATE CLUSTERED INDEX IX_QuestGameLogSW_StartTime ON dbo.QuestGameLogSW(StartTime);

-- EXEC sp_helpindex QuestGameLogSW
-- DROP TABLE QuestGameLogSW

------------------------------------------------------------------------------------

CREATE TABLE dbo.ClanGameLogSW
(
	ID					INT		IDENTITY(1, 1)	NOT NULL,
	WinnerCLID			INT						NOT NULL,
	LoserCLID			INT						NOT NULL,
	WinnerClanName		VARCHAR(24)				NULL,
	LoserClanName		VARCHAR(24)				NULL,
	WinnerMembers		VARCHAR(110)			NULL,
	LoserMembers		VARCHAR(110)			NULL,
	RoundWins			TINYINT					NOT NULL,
	RoundLosses			TINYINT					NOT NULL,
	MapID				TINYINT					NOT NULL,
	GameType			TINYINT					NOT NULL,
	RegDate				DATETIME				NOT NULL,
	WinnerPoint			INT						NULL,
	LoserPoint			INT						NULL,

	CONSTRAINT PK_ClanGameLogSW PRIMARY KEY NONCLUSTERED (ID ASC)
)
GO

ALTER TABLE dbo.ClanGameLogSW WITH CHECK ADD CONSTRAINT FK_ClanGameLogSW_LoserCLID FOREIGN KEY(LoserCLID) REFERENCES dbo.Clan(CLID)
GO

ALTER TABLE dbo.ClanGameLogSW CHECK CONSTRAINT FK_ClanGameLogSW_LoserCLID 
GO

ALTER TABLE dbo.ClanGameLogSW WITH CHECK ADD CONSTRAINT FK_ClanGameLogSW_WinnerCLID FOREIGN KEY(WinnerCLID) REFERENCES dbo.Clan(CLID)
GO

ALTER TABLE dbo.ClanGameLogSW CHECK CONSTRAINT FK_ClanGameLogSW_WinnerCLID 
GO

CREATE NONCLUSTERED INDEX IX_ClanGameLogSW_WinnerCLID ON dbo.ClanGameLogSW(WinnerCLID)
CREATE NONCLUSTERED INDEX IX_ClanGameLogSW_LoserCLID ON dbo.ClanGameLogSW(LoserCLID)
CREATE CLUSTERED INDEX IX_ClanGameLogSW_RegDate	ON dbo.ClanGameLogSW(RegDate)

-- EXEC sp_helpindex ClanGameLogSW
-- DROP TABLE ClanGameLogSW

------------------------------------------------------------------------------------

CREATE TABLE dbo.GameLogSW
(
	ID				INT			IDENTITY(1, 1)	NOT NULL,
	GameName		VARCHAR(64)					NULL,
	MasterCID		INT							NULL,
	Map				VARCHAR(32)					NULL,
	GameType		VARCHAR(24)					NULL,
	Round			INT							NULL,
	StartTime		DATETIME					NOT NULL,
	PlayerCount		TINYINT						NULL,
	Players			VARCHAR(1000)				NULL
)
GO

CREATE CLUSTERED INDEX IX_GameLogSW_StartTime ON dbo.GameLogSW(StartTime);

-- EXEC sp_helpindex GameLogSW
-- DROP TABLE GameLogSW

------------------------------------------------------------------------------------

CREATE TABLE dbo.PlayerLogSW
(
	ID			INT		IDENTITY(1, 1)	NOT NULL,
	CID			INT						NULL,
	DisTime		DATETIME				NULL,
	PlayTime	INT						NULL,
	Kills		INT						NULL,
	Deaths		INT						NULL,
	XP			INT						NULL,
	TotalXP		INT						NULL
)
GO

CREATE CLUSTERED INDEX IX_PlayerLogSW_DisTime ON dbo.PlayerLogSW(DisTime);
CREATE NONCLUSTERED INDEX IX_PlayerLogSW_CID ON dbo.PlayerLogSW(CID);

-- EXEC sp_helpindex PlayerLogSW
-- DROP TABLE PlayerLogSW

------------------------------------------------------------------------------------

CREATE TABLE dbo.ItemPurchaseLogByBountySW
(
	ID				INT		IDENTITY(1, 1)	NOT NULL,
	ItemID			INT						NOT NULL,
	CID				INT						NULL,
	Date			DATETIME				NULL,
	Bounty			INT						NULL,
	CharBounty		INT						NULL,
	Type			VARCHAR(20)				NULL
)
GO

CREATE CLUSTERED INDEX IX_ItemPurchaseLogByBountySW_Date ON dbo.ItemPurchaseLogByBountySW(Date);
CREATE NONCLUSTERED INDEX IX_ItemPurchaseLogByBountySW_CID ON dbo.ItemPurchaseLogByBountySW(CID);

-- EXEC sp_helpindex ItemPurchaseLogByBountySW
-- DROP TABLE ItemPurchaseLogByBountySW

------------------------------------------------------------------------------------