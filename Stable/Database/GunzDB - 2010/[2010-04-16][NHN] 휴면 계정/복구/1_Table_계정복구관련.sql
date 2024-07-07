USE GunzDB

CREATE TABLE dbo.SleepAccountRestoredLogNHN
(
	AID				INT			NOT NULL,
	UserID			VARCHAR(24) NOT NULL,
	RegDt			DATETIME	NOT NULL,
	RestoredDt		DATETIME	NOT NULL		
	
	CONSTRAINT SleepAccountRestoredLogNHN_PK PRIMARY KEY NONCLUSTERED (AID, RegDt)
)

CREATE CLUSTERED INDEX IX_SleepAccountRestoredLogNHN_RestoredDt ON dbo.SleepAccountRestoredLogNHN(RestoredDt)
CREATE INDEX IX_SleepAccountRestoredLogNHN_UserID ON dbo.SleepAccountRestoredLogNHN(UserID)


CREATE TABLE dbo.SleepCharacterRestoredLogNHN
(
	CID				INT			NOT NULL,	
	OrginName		VARCHAR(24) NOT NULL,	
	RestoreName		VARCHAR(24) NOT NULL,
	RegDt			DATETIME	NOT NULL,
	RestoredDt		DATETIME	NOT NULL
	
	CONSTRAINT SleepCharacterRestoredLogNHN_PK PRIMARY KEY NONCLUSTERED (CID, RegDt)
)

CREATE CLUSTERED INDEX IX_SleepCharacterRestoredLogNHN_RestoredDt ON dbo.SleepCharacterRestoredLogNHN(RestoredDt)
