CREATE TABLE dbo.DTCharacterRankingHistory
(	
	TimeStamp		CHAR(8)		NOT NULL,
    Name            VARCHAR(24) NOT NULL,
	Rank			INT			NOT NULL,
	CID				INT			NOT NULL,
	TP				INT			NOT NULL,
	FinalWins		INT			NOT NULL,
	Wins			INT			NOT NULL,
	Loses			INT			NOT NULL,	
	Grade			INT			NOT NULL
		
	CONSTRAINT DTCharacterRankingHistory_PK PRIMARY KEY CLUSTERED (TimeStamp, Rank)	
)
ALTER TABLE dbo.DTCharacterRankingHistory WITH CHECK ADD 
CONSTRAINT DTCharacterInfo_DTCharacterRankingHistory_FK1 FOREIGN KEY (TimeStamp, CID)
REFERENCES dbo.DTCharacterInfo (TimeStamp, CID)