USE GunzDB
GO

--------------------------------------------------------------------------------------------------

CREATE TABLE AccountPenaltyCode
(
	PCode		TINYINT		NOT NULL
	, Name		VARCHAR(24)	NOT NULL
			
	CONSTRAINT AccountPenaltyCode_PK PRIMARY KEY CLUSTERED (PCode)	
)
GO

INSERT AccountPenaltyCode(PCode, Name) VALUES (0, 'None');
INSERT AccountPenaltyCode(PCode, Name) VALUES (1, 'Connect Block');
INSERT AccountPenaltyCode(PCode, Name) VALUES (2, 'Chat Block');
GO

--------------------------------------------------------------------------------------------------

CREATE TABLE AccountPenaltyGMType
(
	GM_TypeID		TINYINT		NOT NULL
	, GM_TypeName	VARCHAR(10)	NOT NULL
	
	CONSTRAINT AccountPenaltyGMType_PK PRIMARY KEY CLUSTERED (GM_TypeID)
)
GO

INSERT AccountPenaltyGMType(GM_TypeID, GM_TypeName) VALUES (1, 'AdmWeb');
INSERT AccountPenaltyGMType(GM_TypeID, GM_TypeName) VALUES (2, 'Game');
GO

--------------------------------------------------------------------------------------------------

CREATE TABLE AccountPenaltyGMLog
(
	AID					INT				NOT NULL
	, PEndDate			DATETIME		NOT NULL
	, PCode				INT				NOT NULL
	, PLogID			INT				IDENTITY(1, 1)
	, Set_GM_TypeID		TINYINT			NOT NULL	
	, Set_GM_ID			VARCHAR(24)		NOT NULL
	, Set_Date			DATETIME		NOT NULL
	, Reset_GM_TypeID	TINYINT
	, Reset_GM_ID		VARCHAR(24)
	, Reset_Date		DATETIME
	
	CONSTRAINT AccountPenaltyGMLog_PK PRIMARY KEY CLUSTERED (AID, PEndDate, PCode, PLogID)
)
GO

ALTER TABLE dbo.AccountPenaltyGMLog WITH CHECK ADD CONSTRAINT AccountPenaltyGMLog_Account_FK FOREIGN KEY(AID)
REFERENCES dbo.Account(AID)
GO

ALTER TABLE dbo.AccountPenaltyGMLog CHECK CONSTRAINT AccountPenaltyGMLog_Account_FK
GO

ALTER TABLE dbo.AccountPenaltyGMLog WITH CHECK ADD CONSTRAINT AccountPenaltyGMLog_AccountPenaltyGMType_FK FOREIGN KEY(Set_GM_TypeID)
REFERENCES dbo.AccountPenaltyGMType(GM_TypeID)
GO

ALTER TABLE dbo.AccountPenaltyGMLog CHECK CONSTRAINT AccountPenaltyGMLog_AccountPenaltyGMType_FK
GO

ALTER TABLE dbo.AccountPenaltyGMLog WITH CHECK ADD CONSTRAINT AccountPenaltyGMLog_AccountPenaltyGMType_FK2 FOREIGN KEY(Reset_GM_TypeID)
REFERENCES dbo.AccountPenaltyGMType(GM_TypeID)
GO

ALTER TABLE dbo.AccountPenaltyGMLog CHECK CONSTRAINT AccountPenaltyGMLog_AccountPenaltyGMType_FK2
GO

--------------------------------------------------------------------------------------------------
