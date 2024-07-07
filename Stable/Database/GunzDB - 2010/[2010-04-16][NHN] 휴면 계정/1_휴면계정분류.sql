USE GunzDB
GO

--------------------------------------------------------------------------------------------------------
-- 휴면 계정으로 분류될 사람들의 목록을 저장할 테이블
CREATE TABLE SleepAccountNHN
(
	AID		INT 		NOT NULL
  , UserID 	VARCHAR(24) NOT NULL
  , RegDt 	DATETIME 	NOT NULL
);
GO

--------------------------------------------------------------------------------------------------------
-- 삭제될 케릭터들의 목록을 저장할 테이블
CREATE TABLE SleepCharacterNHN
(
	AID 	INT 		NOT NULL
  , CID 	INT 		NOT NULL
  , Name 	VARCHAR(24) NOT NULL
  , RegDt 	DATETIME 	NOT NULL
);
GO



--------------------------------------------------------------------------------------------------------
-- 첫 스텝(처음에만 진행시켜야 되는 쿼리)
BEGIN TRAN ------------
	
	--------------------------------------------------------------------------------------------------------
	-- Step 3. 휴면 계정으로 분류
	DECLARE @NowDate DATETIME;
	SET @NowDate = GETDATE();	
	
	INSERT INTO SleepAccountNHN(AID, UserID, RegDt)
		SELECT AID, UserID, CONVERT(VARCHAR(8), GETDATE(), 112)
		FROM   Account WITH (NOLOCK)
		WHERE  DATEDIFF(day, LastLogoutTime, @Nowdate) > 180
		AND	   (HackingType != 10 OR HackingType IS NULL)
		
	CREATE CLUSTERED INDEX IX_SleepAccountNHN_RegDt ON SleepAccountNHN(RegDt);
	CREATE INDEX IX_SleepAccountNHN_UserID ON SleepAccountNHN(UserID);
	ALTER TABLE SleepAccountNHN ADD CONSTRAINT SleepAccountNHN_PK PRIMARY KEY NONCLUSTERED (AID);

	
	UPDATE Account
	SET    HackingType = 10
	WHERE  DATEDIFF(day, LastLogoutTime, @Nowdate) > 180
	AND	   (HackingType != 10 OR HackingType IS NULL)
	


COMMIT TRAN ------------
GO



--------------------------------------------------------------------------------------------------------
-- 첫 스텝 이후, 추후에 휴면 계정 처리할 때 적용시켜야 되는 쿼리
BEGIN TRAN ------------
	/*	
	DECLARE @NowDate DATETIME;
	SET @NowDate = GETDATE();	
	
	INSERT INTO SleepAccountNHN(AID, UserID, RegDt)
		SELECT AID, UserID, CONVERT(VARCHAR(8), GETDATE(), 112)
		FROM   Account WITH (NOLOCK)
		WHERE  DATEDIFF(day, LastLogoutTime, @NowDate) > 180
		AND	   (HackingType != 10 OR HackingType IS NULL)
		
	UPDATE Account
	SET	   HackingType = 10
	WHERE  AID IN (SELECT AID 
				   FROM NHN_SLEEP_ACCOUNT 
				   WHERE RegDt = CAST('20100506' AS DATETIME));
	*/
COMMIT TRAN ------------
GO