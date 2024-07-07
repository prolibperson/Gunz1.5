USE GunzDB
GO

---------------------------------------------------------------------------------------------------
-- 1. 7월 20일 ~ 8월 20일 동안 30분 이상 플레이 한 유저수 
--	 (30분 이상 7일동안 플레이 한 유저수)
---------------------------------------------------------------------------------------------------
SELECT	*
INTO	Event_Summer_Attendance_Netmarble
FROM	(	SELECT	c.AID, CONVERT(VARCHAR(8), pl.DisTime, 112) AS Date, SUM(pl.PlayTime) AS PlayTime
			FROM	PlayerLog pl(NOLOCK), Character c(NOLOCK)
			WHERE	pl.DisTime BETWEEN '2010-07-20 00:00:00.0' AND '2010-08-20 23:59:59.0'
			AND		c.CID = pl.CID
			GROUP BY c.AID, CONVERT(VARCHAR(8), pl.DisTime, 112) ) r
WHERE	r.PlayTime >= 1800

SELECT	a.UserID, a.AID
INTO	Event_Summer_Attendance_Netmarble_Result
FROM	(	SELECT	e.AID
			FROM	Event_Summer_Attendance_Netmarble e(NOLOCK)
			GROUP BY e.AID HAVING COUNT(e.AID) = 7) r, Account a(NOLOCK)
WHERE	a.AID = r.AID
AND		a.DelFlag != 1

/*
SELECT 	*
INTO	Event_Summer_Attendance_Netmarble_Result_Final
FROM	Event_Summer_Attendance_Netmarble_Result
WHERE	(AID % 26) = 0
AND		(AID % 3) = 0

SELECT	*
FROM	Event_Summer_Attendance_Netmarble_Result_Final
*/


DROP TABLE Event_Summer_Attendance_Netmarble;
DROP TABLE Event_Summer_Attendance_Netmarble_Result;
DROP TABLE Event_Summer_Attendance_Netmarble_Result_Final;