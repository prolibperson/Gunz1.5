USE GunzDB
GO

CREATE TABLE dbo.SurvivalScenarioID
(
	SID		TINYINT IDENTITY(1,1)	NOT NULL,
	SName	VARCHAR(20)				NOT NULL,
 
	CONSTRAINT PK_SurvivalScenarioID PRIMARY KEY CLUSTERED 
	(
		SID ASC
	)
)

INSERT SurvivalScenarioID VALUES('Mansion')
INSERT SurvivalScenarioID VALUES('Prison')
INSERT SurvivalScenarioID VALUES('Dungeon')