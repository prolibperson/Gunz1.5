UPDATE t
SET t.Spel_A = r.Count
FROM 
(
	SELECT c.CID, t.Count
	FROM
	(
		SELECT ci.CID, ci.ItemID, COUNT(ci.ItemID) AS Count
		FROM
		(
			SELECT ci.CID, COUNT(ci.CID) AS Count
			FROM CharacterItem ci(NOLOCK), Item i(NOLOCK)
			WHERE ci.CIID > 465012153
				AND ci.RegDate > '2009-12-23 00:00:00.000'  
				AND ci.ItemID = i.ItemID
				AND ci.ItemID BETWEEN 402000 AND 402025
				AND ci.CID IS NOT NULL 
			GROUP BY ci.CID HAVING COUNT(ci.CID) > 9
		) r, CharacterItem ci(NOLOCK)
		WHERE ci.CIID > 465012153
			AND ci.RegDate > '2009-12-23 00:00:00.000'  
			AND ci.CID = r.CID
	--		AND ci.ItemID BETWEEN 402000 AND 402025
			AND ci.ItemID = 402000
		GROUP BY ci.CID, ci.ItemID
	) t, Character c(NOLOCK)
	WHERE t.CID = c.CID
		AND c.DeleteFlag = 0
) r, Event_Christmas_Netmarble t
WHERE r.CID = t.CID
