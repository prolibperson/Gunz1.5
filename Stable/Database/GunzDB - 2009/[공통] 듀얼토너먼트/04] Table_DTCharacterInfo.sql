CREATE TABLE dbo.DTCharacterInfo
(
	DTCIID				INT	IDENTITY(1, 1) NOT NULL,
	TimeStamp			CHAR(8) NOT NULL,		-- TimeStamp
	CID					INT	NOT NULL,			-- Character ID
	TP					INT	NOT NULL,			-- Tournament Point(Ranking Point)
	Wins				INT	NOT NULL,			
	Loses				INT	NOT NULL,			
	Ranking				INT	NOT NULL,			
	RankingIncrease		INT	NOT NULL,	
	FinalGames			INT NOT NULL,		-- Final Games Count
	FinalWins			INT NOT NULL,		-- Final Games Wins
	SFinalGames			INT NOT NULL,		-- Semi Final
	SFinalWins			INT NOT NULL,
	QFinalGames			INT NOT NULL,		-- Quarter Final
	QFinalWins			INT NOT NULL,
	LeaveCount			INT NOT NULL,
	PreGrade			INT NOT NULL,
	
	CONSTRAINT DTCharacterInfo_PK PRIMARY KEY CLUSTERED (DTCIID ASC, TimeStamp ASC)	
);
ALTER TABLE dbo.DTCharacterInfo WITH CHECK ADD 
CONSTRAINT Character_DTCharacterInfo_FK1 FOREIGN KEY (CID)
REFERENCES dbo.Character (CID)

ALTER TABLE dbo.DTCharacterInfo WITH CHECK ADD 
CONSTRAINT DTTimeStamp_DTCharacterInfo_FK1 FOREIGN KEY (TimeStamp)
REFERENCES dbo.DTTimeStamp(TimeStamp)

ALTER TABLE dbo.DTCharacterInfo CHECK CONSTRAINT Character_DTCharacterInfo_FK1
ALTER TABLE dbo.DTCharacterInfo CHECK CONSTRAINT DTTimeStamp_DTCharacterInfo_FK1

CREATE UNIQUE INDEX DTCharacterInfo_TimeStamp_CID_IX ON DTCharacterInfo(TimeStamp ASC, CID DESC)