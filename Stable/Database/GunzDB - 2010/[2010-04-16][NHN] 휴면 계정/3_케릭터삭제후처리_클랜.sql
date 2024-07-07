USE GunzDB
GO

--------------------------------------------------------------------------------------------------------
-- 주의) 쿼리문들에 '20100506'을 쿼리를 적용하는 시점의 날짜로 바꿔준다.
--      날짜를 통하여 계정 및 케릭터를 삭제하고 처리한다.
--		예) CAST('20100506' AS DATETIME) -> CAST(현재날짜 AS DATETIME)
--------------------------------------------------------------------------------------------------------
BEGIN TRAN ------------

	--------------------------------------------------------------------------------------------------------
	-- 삭제된 케릭터들 중에, 클랜 마스터가 있을 경우, 클랜 폐쇄 후, 모두 탈퇴시킨다.		
	UPDATE	c
	SET		c.DeleteName = c.Name, c.Name = NULL, c.DeleteFlag = 1, c.DeleteDate = GETDATE()
	FROM	dbo.SleepCharacterNHN t
		  , dbo.Clan c 
	WHERE	c.MasterCID = t.CID	
	AND		t.RegDt = CAST('20100506' AS DATETIME);
				

	--------------------------------------------------------------------------------------------------------
	-- 삭제된 케릭터들 중에, 클랜에 속해 있는 사람은 모두 탈퇴시킨다.
	DELETE	c
	FROM	dbo.SleepCharacterNHN t
		  , dbo.ClanMember c 
	WHERE	c.CID = t.CID
	AND		t.RegDt = CAST('20100506' AS DATETIME);
	
		
	--------------------------------------------------------------------------------------------------------
	-- 아무도 없는 클랜이 있을 경우, 클랜을 폐쇄시킬까? -_-;;	
	UPDATE Clan
	SET	   DeleteName = Name, Name = NULL, DeleteFlag = 1, DeleteDate = GETDATE()
	WHERE  DeleteFlag = 0 
	AND	   NOT EXISTS(SELECT 1 FROM ClanMember cm WHERE cm.CLID = Clan.CLID);	
	
COMMIT TRAN ------------
GO