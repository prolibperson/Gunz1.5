USE GunzDB
GO

--------------------------------------------------------------------------------------------------------
-- 주의) 쿼리문들에 '20100506'을 쿼리를 적용하는 시점의 날짜로 바꿔준다.
--      날짜를 통하여 계정 및 케릭터를 삭제하고 처리한다.
--		예) CAST('20100506' AS DATETIME) -> CAST(현재날짜 AS DATETIME)
--------------------------------------------------------------------------------------------------------
BEGIN TRAN ------------

	--------------------------------------------------------------------------------------------------------
	-- 삭제된 케릭터들의 친구 목록을 모두 삭제한다. 
	UPDATE	f
	SET		f.DeleteFlag = 1  
	FROM	dbo.SleepCharacterNHN t
		  , dbo.Friend f
	WHERE	t.CID = f.CID
	AND		t.RegDt = CAST('20100506' AS DATETIME);
	
	UPDATE	f
	SET		f.DeleteFlag = 1  
	FROM	dbo.SleepCharacterNHN t
		  , dbo.Friend f
	WHERE	t.CID = f.FriendCID
	AND		t.RegDt = CAST('20100506' AS DATETIME);
	
COMMIT TRAN ------------
GO