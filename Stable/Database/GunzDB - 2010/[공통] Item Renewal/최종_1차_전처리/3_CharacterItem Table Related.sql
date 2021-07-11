USE GunzDB
GO

-----------------------------------------------------------------------------------------------------------
-- 작업할 쿼리

-- CharacterItem 테이블의 Cnt 컬럼을 1로 모두 초기화한다.
-- 주의 1) Cnt 컬럼이 없을 경우 만들어준다(INT형)
-- 주의 2) 시간이 오래 걸리는 쿼리이므로, 리얼에 패치할 때는 주의!
--        (시간이 너무 올래 걸릴 경우, DBA 판단에 따라 처리한다)
UPDATE	CharacterItem
SET		Cnt = 1


-- CharacterItem 테이블의 Cnt 컬럼에 관한 Default 제약 조건을 건다.
-- 주의 1) 이미 있을 경우, 기존의 제약 조건을 삭제한다.
--        (ex. ALTER TABLE CharacterItem DROP CONSTRAINT DF__CharacterIt__Cnt__10766AC2)
ALTER TABLE dbo.CharacterItem ADD CONSTRAINT DF_CharacterItem_Cnt DEFAULT(1) FOR Cnt


-- 케릭터들이 소지하고 있는 겜블 아이템들을 합쳐주는 쿼리
-- 주의 1) 굳이 패치할 때, 실행시키지 않아도 된다(그러므로 일단 주석처리)
/*
SELECT	gi.Name, ci.*
INTO	CharacterGambleItem
FROM	CharacterItem ci(NOLOCK), GambleItem gi(NOLOCK)
WHERE	ci.ItemID = gi.GIID

DELETE	ci
FROM	CharacterItem ci, CharacterGambleItem cg
WHERE	ci.CIID = cg.CIID

INSERT CharacterItem(CID, ItemID, RentDate, RentHourPeriod, Cnt)
	SELECT	cg.CID, cg.ItemID, GETDATE(), 0, COUNT(cg.ItemID) AS ItemCnt
	FROM	CharacterGambleItem cg(NOLOCK)
	GROUP BY cg.CID, cg.ItemID HAVING COUNT(cg.ItemID) > 1

DROP TABLE CharacterGambleItem;	
*/



-----------------------------------------------------------------------------------------------------------
-- 복구 쿼리

/*	
SELECT	ci.*
INTO	CharacterGambleItemForRollBack
FROM	CharacterItem ci(NOLOCK), GambleItem gi(NOLOCK)
WHERE	ci.ItemID = gi.GIID
AND		ci.Cnt > 1
	
DELETE	ci
FROM	CharacterItem ci, CharacterGambleItemForRollBack cg
WHERE	ci.CIID = cg.CIID
	

DECLARE @CID		INT
DECLARE	@ItemID		INT
DECLARE	@Cnt		INT
DECLARE @RentDate	DATETIME	

DECLARE Curs CURSOR FAST_FORWARD FOR   
	SELECT cg.CID, cg.ItemID, cg.RentDate, cg.Cnt
	FROM   CharacterGambleItemForRollBack cg
	
OPEN Curs
FETCH NEXT FROM Curs INTO @CID, @ItemID, @RentDate, @Cnt

WHILE( @@FETCH_STATUS = 0 ) BEGIN
	
	INSERT CharacterItem(CID, ItemID, RentDate, Cnt)
	VALUES(@CID, @ItemID, @RentDate, NULL)		
	
	IF (0 <> @@ERROR) BEGIN  
		ROLLBACK TRAN;  
		CLOSE Curs;  
		DEALLOCATE Curs;  
		RETURN;  
	END

	SET @Cnt = @Cnt - 1
	
	IF( @Cnt = 0 ) BEGIN
		FETCH NEXT FROM Curs INTO @CID, @ItemID, @RentDate, @Cnt
	END
END  

CLOSE Curs  
DEALLOCATE Curs  

DROP TABLE CharacterGambleItemForRollBack;
*/