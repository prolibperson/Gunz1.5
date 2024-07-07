USE GunzDB
GO

CREATE TABLE dbo.SurvivalCharacterInfoWeb
(
	RowNum	INT		IDENTITY(1,1)	NOT NULL,
	SID		TINYINT					NOT NULL,
	CID		INT						NOT NULL,
	
	CONSTRAINT PK_SurvivalCharacterInfoWeb PRIMARY KEY CLUSTERED 
	(
		SID ASC,
		RowNum ASC
	)
)