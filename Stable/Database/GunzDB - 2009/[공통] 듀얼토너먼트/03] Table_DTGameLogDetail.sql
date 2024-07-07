CREATE TABLE dbo.DTGameLogDetail
(
	TimeStamp	CHAR(8)			NOT NULL,
	LogID		INT				NOT NULL,	
	StartTime	DATETIME		NOT NULL,
	PlayTime	INT				NOT NULL,
	MatchType	TINYINT			NOT NULL,
	WinnerCID	INT				NOT NULL,
	LoserCID	INT				NOT NULL,
	GainTP		INT				NOT NULL,
	LoseTP		INT				NOT NULL,
		
	CONSTRAINT DTGameLogDetail_PK PRIMARY KEY CLUSTERED (TimeStamp, LogID, StartTime)
)
ALTER TABLE dbo.DTGameLogDetail WITH CHECK ADD 
CONSTRAINT DTGameLog_DTGameLogDetail_FK1 FOREIGN KEY (TimeStamp, LogID)
REFERENCES dbo.DTGameLog (TimeStamp, LogID)

ALTER TABLE dbo.DTGameLogDetail CHECK CONSTRAINT DTGameLog_DTGameLogDetail_FK1
