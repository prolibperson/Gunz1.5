CREATE TABLE EventLottoResultLog
(
	AID		INT			NOT NULL,
	CID		INT			NOT NULL,
	Num1	TINYINT		NULL,
	Num2	TINYINT		NULL,
	Num3	TINYINT		NULL,
	Num4	TINYINT		NULL	

	CONSTRAINT EventLottoResultLog_PK PRIMARY KEY CLUSTERED (AID ASC, CID ASC)
)
TRUNCATE TABLE EventLottoResultLog

INSERT INTO EventLottoResultLog(AID, CID)
	SELECT c.AID, c.CID
	FROM
	(
		SELECT ci.CID, ci.ItemID, (ci.ItemID - 401000) AS Number, COUNT(ci.ItemID) AS Count
		FROM CharacterItem ci(NOLOCK)
		WHERE ci.CIID >= 456600000
			AND ci.ItemID BETWEEN 401000 AND 401010
			AND ci.CID IS NOT NULLQ
		GROUP BY ci.CID, ci.ItemID
	) r, Character c
	WHERE r.CID = c.CID
	GROUP BY c.AID, c.CID



DECLARE @Count INT;


UPDATE EventLottoResultLog
SET Num4 = 1
WHERE CID IN (
	SELECT c.CID
	FROM
	(
		SELECT ci.CID, ci.ItemID, (ci.ItemID - 401000) AS Number, COUNT(ci.ItemID) AS Count
		FROM CharacterItem ci(NOLOCK)
		WHERE ci.CIID >= 456600000
			AND ci.ItemID BETWEEN 401000 AND 401010
			AND ci.CID IS NOT NULL
		GROUP BY ci.CID, ci.ItemID
	) r, Character c
	WHERE r.Number = 8
		AND r.Count > 0
		AND r.CID = c.CID)


SELECT a.UserID, c.Name, r.*
FROM EventLottoResultLog r
	, Character c
	, Account a
WHERE r.CID = c.CID
	AND r.AID = a.AID
	AND r.Num1 IS NOT NULL 
	AND r.Num2 IS NOT NULL
	AND r.Num3 IS NOT NULL
	AND r.Num4 IS NOT NULL
	