-- Account Delete 이력을 남기는 테이블입니다.

CREATE TABLE dbo.AccountDeleteLog
(
	AID					INT			NOT NULL,
	UserID				VARCHAR(20) NOT NULL,
	UserCN				VARCHAR(20) NOT NULL,
	DeleteID			VARCHAR(20) NOT NULL,
	DeleteDate			DATETIME	NOT NULL
	
	CONSTRAINT AccountDeleteLog_PK PRIMARY KEY CLUSTERED (AID DESC)
)

CREATE NONCLUSTERED INDEX AccountDeleteLog_UserID_IX ON dbo.AccountDeleteLog(UserID)