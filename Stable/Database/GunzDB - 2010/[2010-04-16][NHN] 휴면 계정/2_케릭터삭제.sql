USE GunzDB
GO

--------------------------------------------------------------------------------------------------------
-- 첫 스텝(처음에만 진행시켜야 되는 쿼리)
BEGIN TRAN ------------

	--------------------------------------------------------------------------------------------------------
	-- 삭제될 케릭터들을 분류하고, 삭제하고, 로그에 남긴다.
	INSERT dbo.SleepCharacterNHN(CID, AID, Name, RegDt)
		SELECT	c.CID, c.AID, c.Name, CONVERT(VARCHAR(8), GETDATE(), 112)
		FROM	dbo.Character c(NOLOCK) 
			  , dbo.SleepAccountNHN asl(NOLOCK) 
		WHERE	c.AID = asl.AID
		AND		c.DeleteFlag = 0
		
	-- 인덱스 생성
	CREATE CLUSTERED INDEX IX_SleepCharacterNHN_RegDt ON SleepCharacterNHN(RegDt);
	CREATE INDEX IX_SleepCharacterNHN_AID ON dbo.SleepCharacterNHN(AID);
	ALTER TABLE SleepCharacterNHN ADD CONSTRAINT SleepCharacterNHN_PK PRIMARY KEY NONCLUSTERED (AID, CID);
		
	-- Character 테이블에 아래의 업데이트와 관련된 인덱스를 임시로 지운다.
	-- 주의) 인덱스 이름이 다를 수도 있습니다.
	DROP INDEX dbo.Character.IX_Character_DeleteFlag
	DROP INDEX dbo.Character.IX_Character_AID_DeleteFlag
	DROP INDEX dbo.Character.IX_Character_Name			
		 
	UPDATE	c 
	SET		c.DeleteName = c.Name, c.Name ='', c.CharNum = -1, c.DeleteFlag = 1
	FROM	dbo.Character c 
		  , dbo.SleepCharacterNHN t	 	
	WHERE	c.CID = t.CID
	AND		c.DeleteFlag = 0;
	
	-- 임시로 지웠던 인덱스를 다시 생성해준다.
	-- 주의) 아래의 이름으로 꼭 생성해주세요.
	CREATE INDEX IX_Character_DeleteFlag		ON dbo.Character(DeleteFlag)
	CREATE INDEX IX_Character_AID_DeleteFlag	ON dbo.Character(AID, DeleteFlag)
	CREATE INDEX IX_Character_Name				ON dbo.Character(Name)

	
COMMIT TRAN ------------
GO



--------------------------------------------------------------------------------------------------------
-- 첫 스텝 이후, 추후에 휴면 계정 처리할 때 적용시켜야 되는 쿼리
BEGIN TRAN ------------
/*
	INSERT dbo.SleepCharacterNHN(CID, AID, Name, RegDate)
		SELECT	c.CID, c.AID, c.Name, CONVERT(VARCHAR(8), GETDATE(), 112)
		FROM	dbo.Character c(NOLOCK) 
			  , dbo.SleepAccountNHN asl(NOLOCK) 
		WHERE	c.AID = asl.AID
		AND		c.DeleteFlag = 0
		AND		asl.RegDt = CAST('20100506' AS DATETIME);
		
		
	UPDATE	c 
	SET		c.DeleteName = c.Name, c.Name ='', c.CharNum = -1, c.DeleteFlag = 1
	FROM	dbo.Character c 
		  , dbo.SleepCharacterNHN t	 	
	WHERE	c.CID = t.CID
	AND		c.DeleteFlag = 0;
	AND		t.RegDT = CAST('20100506' AS DATETIME);
*/
COMMIT TRAN ------------
GO