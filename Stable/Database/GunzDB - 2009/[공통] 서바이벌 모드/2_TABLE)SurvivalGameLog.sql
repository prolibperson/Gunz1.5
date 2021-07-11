USE GunzDB
GO

CREATE TABLE dbo.SurvivalGameLog
(
	StartTime		DATETIME	NOT NULL,
	FinishTime		DATETIME	NOT NULL,
	Master_Player	INT			NOT NULL,
	GainRP1			INT			NOT NULL,
	Player2			INT			NULL,
	GainRP2			INT			NULL,
	Player3			INT			NULL,
	GainRP3			INT			NULL,
	Player4			INT			NULL,
	GainRP4			INT			NULL,
	GameName		VARCHAR(64) NOT NULL,
	SID				TINYINT		NOT NULL,
	TotalRound		SMALLINT	NOT NULL,
 
	CONSTRAINT PK_SurvivalGameLog_MPlayer_StartTime PRIMARY KEY CLUSTERED 
	(
		StartTime		ASC,
		Master_Player	ASC
	)
) 

ALTER TABLE dbo.SurvivalGameLog  WITH CHECK ADD CONSTRAINT FK_SurvivalGameLog_SurvivalScenarioID
FOREIGN KEY(SID) REFERENCES dbo.SurvivalScenarioID (SID)

ALTER TABLE dbo.SurvivalGameLog CHECK CONSTRAINT FK_SurvivalGameLog_SurvivalScenarioID