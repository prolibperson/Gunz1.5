USE GunzDB
GO


----------------------------------------------------------------------------------------------------------------
-- 아이템의 만료 날짜만 갱신해주면 되는 유저들

-- Type 1
-- 바로 업데이트 해주기
UPDATE	ai
SET		ai.RentHourPeriod = ai.RentHourPeriod + 72
FROM	AccountItem ai
WHERE	ai.RentDate IS NOT NULL
AND		ai.RentDate < '2010-10-28 00:00:00.0'
AND		DATEADD(hh, ai.RentHourPeriod, ai.RentDate) > '2010-10-26 18:40:00.0'
AND		DATEADD(hh, ai.RentHourPeriod, ai.RentDate)	> GETDATE()

UPDATE	ci
SET		ci.RentHourPeriod = ci.RentHourPeriod + 72
FROM	CharacterItem ci
WHERE	ci.RentDate IS NOT NULL
AND		ci.RentDate < '2010-10-28 00:00:00.0'
AND		DATEADD(hh, ci.RentHourPeriod, ci.RentDate) > '2010-10-26 18:40:00.0'
AND		DATEADD(hh, ci.RentHourPeriod, ci.RentDate)	> GETDATE()




/*
-- Type 2
-- 먼저, 임시 테이블이 집어 넣은 후.. 한 번 살펴보고 갱신해주기!
SELECT	ai.*
INTO	TempAccountItem
FROM	AccountItem ai
WHERE	ai.RentDate IS NOT NULL
AND		ai.RentDate < '2010-10-28 00:00:00.0'
AND		DATEADD(hh, ai.RentHourPeriod, ai.RentDate) > '2010-10-26 18:40:00.0'
AND		DATEADD(hh, ai.RentHourPeriod, ai.RentDate)	> GETDATE()


SELECT	ci.*
INTO	TempCharacterItem
FROM	CharacterItem ci
WHERE	ci.RentDate IS NOT NULL
AND		ci.RentDate < '2010-10-28 00:00:00.0'
AND		DATEADD(hh, ci.RentHourPeriod, ci.RentDate) > '2010-10-26 18:40:00.0'
AND		DATEADD(hh, ci.RentHourPeriod, ci.RentDate)	> GETDATE()

UPDATE	ai
SET		ai.RentHourPeriod = ai.RentHourPeriod + 72
FROM	AccountItem ai JOIN TempAccountItem t
ON		ai.AIID = t.AIID


UPDATE	ci
SET		ci.RentHourPeriod = ci.RentHourPeriod + 72
FROM	CharacterItem ci JOIN TempCharacterItem t
ON		ci.CIID = t.CIID

DROP TABLE TempAccountItem;
DROP TABLE TempCharacterItem;
*/



----------------------------------------------------------------------------------------------------------------
-- 새롭게 아이템을 넣어줘야 되는 유저들

/*
SELECT	AID, ItemID, GETDATE(), 72
FROM	ItemPurchaseLogByCash l
WHERE	l.Date < '2010-10-28 00:00:00.0'
AND		DATEADD(hh, l.RentHourPeriod, l.Date) > '2010-10-26 18:40:00.0'
AND		DATEADD(hh, l.RentHourPeriod, l.Date) < GETDATE()
*/

INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod)
	SELECT	l.AID, l.ItemID, GETDATE(), 72
	FROM	ItemPurchaseLogByCash l
	WHERE	l.Date < '2010-10-28 00:00:00.0'
	AND		l.RentHourPeriod IS NOT NULL
	AND		DATEADD(hh, l.RentHourPeriod, l.Date) > '2010-10-26 18:40:00.0'
	AND		DATEADD(hh, l.RentHourPeriod, l.Date) < GETDATE()
	
	
-- INSERT 하기 전에, 아래의 SELECT 문부터 한번 확인 부탁드립니다.
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod)	
	SELECT	l.AID, r.ItemID, GETDATE(), 72
	FROM	(	SELECT	csi.CSSID, csi.CSID, cs.ItemID
				FROM	CashSetItem csi JOIN CashShop cs
				ON		cs.CSID = csi.CSID ) r, SetItemPurchaseLogByCash l
	WHERE	l.Date < '2010-10-28 00:00:00.0'
	AND		l.RentHourPeriod IS NOT NULL
	AND		DATEADD(hh, l.RentHourPeriod, l.Date) > '2010-10-26 18:40:00.0'
	AND		DATEADD(hh, l.RentHourPeriod, l.Date) < GETDATE()
	AND		r.CSSID = l.CSSID
	