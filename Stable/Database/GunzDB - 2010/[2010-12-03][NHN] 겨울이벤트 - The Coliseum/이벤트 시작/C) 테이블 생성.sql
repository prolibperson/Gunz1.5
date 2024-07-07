USE GunzDB
GO

------------------------------------------------------------------------------------

/*
DROP TABLE dbo.Event_Coliseum_Character_NHN
DROP TABLE dbo.Event_Coliseum_PlayData_NHN
DROP TABLE dbo.Event_Coliseum_PlayTimeRanking_NHN
DROP TABLE dbo.Event_Coliseum_KillsRanking_NHN
DROP TABLE dbo.Event_Coliseum_DeathsRanking_NHN
*/

------------------------------------------------------------------------------------

CREATE TABLE dbo.Event_Coliseum_Character_NHN
(
	CID				INT			NOT NULL
	, RegDate		DATETIME	NOT NULL
		
	CONSTRAINT PK_Event_Coliseum_Character_NHN PRIMARY KEY CLUSTERED (CID)
		
	, CONSTRAINT FK_Event_Coliseum_Character_NHN_Character FOREIGN KEY (CID) REFERENCES dbo.Character(CID))
GO


------------------------------------------------------------------------------------
	
CREATE TABLE dbo.Event_Coliseum_PlayData_NHN
(	
	[DateTag]			CHAR(10)	NOT NULL
	, CID				INT			NOT NULL
	, PlayTime			INT			NOT NULL	
	, Kills				INT			NOT NULL	
	, Deaths			INT			NOT NULL
	, LastUpdatedDate	DATETIME	NOT NULL
	, PlayTimeRank		INT			NULL
	, KillRank			INT			NULL
	, DeathRank			INT			NULL
		
	CONSTRAINT PK_Event_Coliseum_PlayData_NHN PRIMARY KEY CLUSTERED ([DateTag], CID)
	
	, CONSTRAINT FK_Event_Coliseum_PlayData_NHN_Event_Coliseum_Character_NHN
		FOREIGN KEY (CID) REFERENCES dbo.Event_Coliseum_Character_NHN(CID)
)
GO

CREATE NONCLUSTERED INDEX IX_Event_Coliseum_PlayData_NHN_CID ON dbo.Event_Coliseum_PlayData_NHN(CID)

------------------------------------------------------------------------------------

CREATE TABLE dbo.Event_Coliseum_PlayTimeRanking_NHN
(	
	[DateTag]	CHAR(10)		NOT NULL	
	, [Rank]	INT				NOT NULL
	, CID		INT				NOT NULL
			
	CONSTRAINT PK_Event_Coliseum_PlayTimeRanking_NHN PRIMARY KEY CLUSTERED ([DateTag], [Rank])
)
GO

------------------------------------------------------------------------------------

CREATE TABLE dbo.Event_Coliseum_KillsRanking_NHN
(	
	[DateTag]	CHAR(10)		NOT NULL
	, [Rank]	INT				NOT NULL
	, CID		INT				NOT NULL
		
	CONSTRAINT PK_Event_Coliseum_KillsRanking_NHN PRIMARY KEY CLUSTERED ([DateTag], [Rank])
)
GO

------------------------------------------------------------------------------------

CREATE TABLE dbo.Event_Coliseum_DeathsRanking_NHN
(	
	[DateTag]	CHAR(10)		NOT NULL
	, [Rank]	INT				NOT NULL
	, CID		INT				NOT NULL
		
	CONSTRAINT PK_Event_Coliseum_DeathsRanking_NHN_NHN PRIMARY KEY CLUSTERED ([DateTag], [Rank])
)
GO

------------------------------------------------------------------------------------