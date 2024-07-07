CREATE PROCEDURE dbo.spGetSurvivalGroupRanking
AS BEGIN
	SET NOCOUNT ON;
	
	SELECT	t.SID, t.CID, c.Name, t.Ranking, t.RP
	FROM (	SELECT	s.SID, s.CID, s.RP, r.Ranking
			FROM	SurvivalCharacterInfo s(NOLOCK)
					, SurvivalRanking r(NOLOCK)
			WHERE	s.RankRP = r.RP
			AND		s.SID = r.SID
			AND		r.SID IN (1, 2, 3)
			AND		s.DeleteFlag = 0
			AND		r.Ranking <= 10 			
		) AS t
		, Character c WITH (NOLOCK)
	WHERE	t.CID = c.CID 
	AND		c.DeleteFlag = 0
	ORDER BY SID, Ranking
END

SELECT * FROM Character WHERE CID = 1313
SELECT * FROM Character WHERE CID = 1182