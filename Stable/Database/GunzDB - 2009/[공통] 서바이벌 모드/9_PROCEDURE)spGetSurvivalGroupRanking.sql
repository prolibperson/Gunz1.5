USE GunzDB
GO

ALTER PROCEDURE dbo.spGetSurvivalGroupRanking
AS BEGIN
	SET NOCOUNT ON;
	
	SELECT	t.SID, t.CID, c.Name, t.Ranking, t.RP
	FROM (	SELECT	s.SID, s.CID, s.RP, r.Ranking
			FROM	SurvivalCharacterInfo s(NOLOCK)
					, SurvivalRanking r(NOLOCK)
			WHERE	s.RankRP = r.RP
			AND		s.SID = r.SID
			AND		r.SID IN (1, 2, 3)
			AND		s.DeleteFlag != 1
			AND		r.Ranking <= 10 			
		) AS t
		, Character c WITH (NOLOCK)
	WHERE	t.CID = c.CID 
	AND		c.DeleteFlag != 1
	ORDER BY SID, Ranking
END
GO