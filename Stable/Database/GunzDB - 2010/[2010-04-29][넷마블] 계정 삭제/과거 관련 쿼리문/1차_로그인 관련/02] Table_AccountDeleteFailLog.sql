-- Account Delete를 요청하는 이력을 남기는 테이블입니다.

CREATE TABLE dbo.AccountDeleteFailLog
(
	AID			INT			NOT NULL,
	TryDate		DATETIME	NOT NULL,
	ErrorCode	TINYINT		NOT NULL
	
	CONSTRAINT AccountDeleteFailLog_PK PRIMARY KEY CLUSTERED (AID DESC)
)