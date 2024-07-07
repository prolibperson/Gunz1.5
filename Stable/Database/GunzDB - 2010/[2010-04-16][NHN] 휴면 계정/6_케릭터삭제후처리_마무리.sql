USE GunzDB
GO

--------------------------------------------------------------------------------------------------------
-- 주의) 쿼리문들에 '20100506'을 쿼리를 적용하는 시점의 날짜로 바꿔준다.
--      날짜를 통하여 계정 및 케릭터를 삭제하고 처리한다.
--		예) CAST('20100506' AS DATETIME) -> CAST(현재날짜 AS DATETIME)
--------------------------------------------------------------------------------------------------------
BEGIN TRAN ------------

	INSERT INTO dbo.CharacterMakingLog(AID, CharName, Type, Date)
		SELECT	t.AID, t.Name, 'Deleteted', GETDATE()
		FROM	dbo.SleepCharacterNHN t(NOLOCK)
		WHERE	t.RegDt = CAST('20100506' AS DATETIME);
	
COMMIT TRAN ------------
GO