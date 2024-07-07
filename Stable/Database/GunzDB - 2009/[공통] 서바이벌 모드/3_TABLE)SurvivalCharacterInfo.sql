USE GunzDB
GO

CREATE TABLE dbo.SurvivalCharacterInfo
(
	SID				TINYINT             NOT NULL,
	CID				INT                 NOT NULL,
	RP				INT                 NOT NULL,
	RP_LatestTime	SMALLDATETIME       NOT NULL,
	tmpRP			INT                 NULL,
	RankRP			INT                 NULL,
	DeleteFlag		TINYINT             DEFAULT(0)
 
	CONSTRAINT [PK_SurvivalCharacterInfo] PRIMARY KEY CLUSTERED 
	(
		SID ASC,
		CID ASC
	)
)

ALTER TABLE dbo.SurvivalCharacterInfo WITH CHECK ADD CONSTRAINT FK_SurvivalCharacterInfo_SurvivalScenarioID 
FOREIGN KEY(SID) REFERENCES dbo.SurvivalScenarioID (SID)

ALTER TABLE dbo.SurvivalCharacterInfo CHECK CONSTRAINT FK_SurvivalCharacterInfo_SurvivalScenarioID

/*
ALTER TABLE dbo.SurvivalCharacterInfo ADD DeleteFlag TINYINT DEFAULT(0)
*/
