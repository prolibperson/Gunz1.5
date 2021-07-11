USE GunzDB
GO

ALTER PROCEDURE dbo.spGetSurvivalPrivateRanking
(
	@nCID int
)
AS 
BEGIN	
	SET NOCOUNT ON;
	
	SELECT	s.SID, s.CID, s.RP, r.Ranking
	FROM	SurvivalCharacterInfo s(NOLOCK)
			, SurvivalRanking r(NOLOCK)
	WHERE	r.SID = s.SID
	AND		r.RP = s.RankRP		
	AND		r.SID IN (1, 2, 3)
	AND		s.DeleteFlag != 1
	AND		s.CID = @nCID;
END
GO
