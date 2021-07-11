USE GunzDB
GO

--------------------------------------------------------------------------------------------------------
-- 주의) 쿼리문들에 '20100506'을 쿼리를 적용하는 시점의 날짜로 바꿔준다.
--      날짜를 통하여 계정 및 케릭터를 삭제하고 처리한다.
--		예) CAST('20100506' AS DATETIME) -> CAST(현재날짜 AS DATETIME)
--------------------------------------------------------------------------------------------------------
BEGIN TRAN ------------

	--------------------------------------------------------------------------------------------------------
	-- 삭제된 케릭터들에게 있는 캐쉬 아이템을 모두 중앙 은행으로 보내고, 모두 삭제한다.
	INSERT INTO dbo.AccountItem(AID, ItemID, RentDate, RentHourPeriod)
		SELECT	t.AID, ci.ItemID, ci.RentDate, ci.RentHourPeriod
		FROM	dbo.SleepCharacterNHN t
			  , dbo.CharacterItem ci
		WHERE	ci.CID = t.CID 	
		AND		ci.ItemID >= 500000
		AND		t.RegDt = CAST('20100506' AS DATETIME);
		
		
	DELETE	ci
	FROM	dbo.SleepCharacterNHN t, dbo.CharacterItem ci
	WHERE	ci.CID = t.CID 	
	AND		ci.ItemID >= 500000
	AND		t.RegDt = CAST('20100506' AS DATETIME);


COMMIT TRAN ------------
GO