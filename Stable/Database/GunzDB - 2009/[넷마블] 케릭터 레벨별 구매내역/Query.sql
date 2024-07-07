SELECT l.UserID, l.Password, t.Level AS CharMaxLevel, t.Cash 
FROM
(
	SELECT c.AID, MAX(c.Level) AS Level, MAX(r.Cash) AS Cash
	FROM
	(
		SELECT l.AID, SUM(Cash) AS Cash
		FROM ItemPurchaseLogByCash l WITH (NOLOCK)
		WHERE l.Date BETWEEN '2009-10-01 00:00:00.000' AND '2009-10-31 23:59:59.000'
		GROUP BY l.AID
	) r, Character c WITH (NOLOCK)
	WHERE r.AID = c.AID
		AND c.DeleteFlag = 0
	GROUP BY c.AID
) t, Login l WITH (NOLOCK)
WHERE l.AID = t.AID
ORDER BY CharMaxLevel DESC, t.Cash DESC


127,956,200
