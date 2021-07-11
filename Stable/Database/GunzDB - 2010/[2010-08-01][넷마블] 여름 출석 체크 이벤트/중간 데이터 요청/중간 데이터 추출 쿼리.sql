USE GunzDB
GO

---------------------------------------------------------------------------------------------------
-- 1. 7월 13일~ 19일 동안 30분 이상 플레이한 유저수 
--	 (30분 이상 7일동안 플레이 한 유저수)
---------------------------------------------------------------------------------------------------
SELECT	*
INTO	Event_Summer_Attendance_Netmarble
FROM	(	SELECT	c.AID, CONVERT(VARCHAR(8), pl.DisTime, 112) AS Date, SUM(pl.PlayTime) AS PlayTime
			FROM	PlayerLog pl(NOLOCK), Character c(NOLOCK)
			WHERE	pl.DisTime BETWEEN '2010-07-13 00:00:00.0' AND '2010-07-19 23:59:59.0'
			AND		c.CID = pl.CID
			GROUP BY c.AID, CONVERT(VARCHAR(8), pl.DisTime, 112) ) r
WHERE	r.PlayTime >= 1800


SELECT	a.UserID, a.AID
FROM	(	SELECT	e.AID
		FROM	Event_Summer_Attendance_Netmarble e(NOLOCK)
		GROUP BY e.AID HAVING COUNT(e.AID) = 7) r, Account a(NOLOCK)
WHERE	a.AID = r.AID

DROP TABLE Event_Summer_Attendance_Netmarble;


---------------------------------------------------------------------------------------------------
-- 2. 7월 20일 ~ 26일 동안 30분 이상 플레이현 유저수 
--	 (30분 이상 7일동안 플레이 한 유저수)
---------------------------------------------------------------------------------------------------
SELECT	*
INTO	Event_Summer_Attendance_Netmarble
FROM	(	SELECT	c.AID, CONVERT(VARCHAR(8), pl.DisTime, 112) AS Date, SUM(pl.PlayTime) AS PlayTime
			FROM	PlayerLog pl(NOLOCK), Character c(NOLOCK)
			WHERE	pl.DisTime BETWEEN '2010-07-20 00:00:00.0' AND '2010-07-26 23:59:59.0'
			AND		c.CID = pl.CID
			GROUP BY c.AID, CONVERT(VARCHAR(8), pl.DisTime, 112) ) r
WHERE	r.PlayTime >= 1800


SELECT	a.UserID, a.AID
FROM	(	SELECT	e.AID
			FROM	Event_Summer_Attendance_Netmarble e(NOLOCK)
			GROUP BY e.AID HAVING COUNT(e.AID) = 7) r, Account a(NOLOCK)
WHERE	a.AID = r.AID

DROP TABLE Event_Summer_Attendance_Netmarble;



---------------------------------------------------------------------------------------------------
-- 3. 7월 13일 ~ 26일 동안 일별 유니트 접속 유저수 데이터
---------------------------------------------------------------------------------------------------
USE LogDB

SELECT	t.Day, COUNT(t.AID) as 'Count'
FROM	(	SELECT	AID, DATEPART(dd, time) AS 'Day'
			FROM	dbo.connlog_201007(NOLOCK)
			GROUP BY AID, DATEPART(dd, time) ) AS t
GROUP BY t.Day
ORDER BY t.Day