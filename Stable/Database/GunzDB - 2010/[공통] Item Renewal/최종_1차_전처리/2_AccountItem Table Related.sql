USE GunzDB
GO

-----------------------------------------------------------------------------------------------------------
-- 작업할 쿼리

-- AccountItem 테이블의 Cnt 컬럼을 1로 모두 초기화한다.
-- 주의 1) Cnt 컬럼이 없을 경우 만들어준다(INT형)
-- 주의 2) 시간이 오래 걸리는 쿼리이므로, 리얼에 패치할 때는 주의!
--        (시간이 너무 올래 걸릴 경우, DBA 판단에 따라 처리한다)
UPDATE	AccountItem
SET		Cnt = 1

-- AccountItem 테이블의 Cnt 컬럼에 관한 Default 제약 조건을 건다.
-- 주의 1) 이미 있을 경우, 기존의 제약 조건을 삭제한다.
--        (ex. ALTER TABLE dbo.AccountItem DROP CONSTRAINT DF__AccountItem_Cnt_1352D76D)
ALTER TABLE dbo.AccountItem ADD CONSTRAINT DF_AccountItem_Cnt DEFAULT(1) FOR Cnt


-- 중앙은행에 있는 겜블 아이템들을 합쳐주는 쿼리
-- 주의 1) 굳이 패치할 때, 실행시키지 않아도 된다(그러므로 일단 주석처리)
/*
SELECT	gi.Name, ai.*
INTO	AccountGambleItem
FROM	AccountItem ai(NOLOCK), GambleItem gi(NOLOCK)
WHERE	ai.ItemID = gi.GIID
	
DELETE	ai
FROM	AccountItem ai, AccountGambleItem ag
WHERE	ai.AIID = ag.AIID

INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt)
	SELECT	ag.AID, ag.ItemID, GETDATE(), 0, COUNT(ag.ItemID) AS ItemCnt
	FROM	AccountGambleItem ag(NOLOCK)
	GROUP BY ag.AID, ag.ItemID HAVING COUNT(ag.ItemID) > 1
*/			


	




-----------------------------------------------------------------------------------------------------------
-- 복구 쿼리
/*

SELECT	ai.*
INTO	AccountGambleItemForRollBack
FROM	AccountItem ai(NOLOCK), GambleItem gi(NOLOCK)
WHERE	ai.ItemID = gi.GIID
AND		ai.Cnt > 1

DELETE	ai
FROM	AccountItem ai, AccountGambleItemForRollBack ag
WHERE	ai.AIID = ag.AIID
	
	
DECLARE @AID		INT
DECLARE	@ItemID		INT
DECLARE	@Cnt		INT
DECLARE @RentDate	DATETIME	

DECLARE Curs CURSOR FAST_FORWARD FOR   
SELECT ag.AID, ag.ItemID, ag.RentDate, ag.Cnt
FROM   AccountGambleItemForRollBack ag

OPEN Curs
FETCH NEXT FROM Curs INTO @AID, @ItemID, @RentDate, @Cnt

WHILE( @@FETCH_STATUS = 0 ) BEGIN
	
	INSERT AccountItem(AID, ItemID, RentDate, Cnt)
	VALUES(@AID, @ItemID, @RentDate, NULL)		
	
	IF (0 <> @@ERROR) BEGIN  
		ROLLBACK TRAN;  
		CLOSE Curs;  
		DEALLOCATE Curs;  
		RETURN;  
	END

	SET @Cnt = @Cnt - 1
	
	IF( @Cnt = 0 ) BEGIN
		FETCH NEXT FROM Curs INTO @AID, @ItemID, @RentDate, @Cnt
	END
END

CLOSE Curs
DEALLOCATE Curs
		
DROP TABLE AccountGambleItemForRollBack;
DROP TABLE AccountItemMoveLog;

*/