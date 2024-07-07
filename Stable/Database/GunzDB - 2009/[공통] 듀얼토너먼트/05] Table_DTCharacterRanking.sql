CREATE TABLE dbo.DTCharacterRanking
(	
	TimeStamp		CHAR(8)		NOT NULL,
	Rank			INT			IDENTITY(1, 1) NOT NULL,
	CID				INT			NOT NULL,
	TP				INT			NOT NULL,
	FinalWins		INT			NOT NULL,
	Wins			INT			NOT NULL,
	Loses			INT			NOT NULL,
	RankingIncrease INT			,
	PreGrade		TINYINT		NOT NULL
		
	CONSTRAINT DTCharacterRanking_PK PRIMARY KEY CLUSTERED (TimeStamp, Rank)	
)
ALTER TABLE dbo.DTCharacterRanking WITH CHECK ADD 
CONSTRAINT DTCharacterInfo_DTCharacterRanking_FK1 FOREIGN KEY (TimeStamp, CID)
REFERENCES dbo.DTCharacterInfo (TimeStamp, CID)
