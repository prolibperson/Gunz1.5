ALTER PROCEDURE spDTGetDTGroupRanking
AS BEGIN
	SET NOCOUNT ON;
	
	DECLARE @ShowCount INT;
	SET @ShowCount = 100;
	
	SELECT c.Name, t.TP, t.Wins, t.Loses, t.FinalWins, t.Rank, t.RankingIncrease, t.PreGrade
	FROM
	(
		SELECT cr.CID, cr.TP, ci.Wins, ci.Loses, ci.FinalWins, cr.Rank, ci.RankingIncrease, ci.PreGrade
		FROM DTCharacterRanking cr WITH (NOLOCK), DTCharacterInfo ci WITH (NOLOCK)
		WHERE cr.CID = ci.CID
			AND cr.Rank > 0
			AND cr.Rank <= @ShowCount
	) t, Character c WITH (NOLOCK)
	WHERE t.CID = c.CID
		AND c.DeleteFlag = 0
	ORDER BY t.Rank
END