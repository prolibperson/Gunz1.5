CREATE PROCEDURE dbo.spWebDTGetDTRankingTOP100
	@TimeStamp	CHAR(8),
	@PageNum	INT
AS BEGIN
	DECLARE @PagePerNum INT;
	SET @PagePerNum = 20;
		
	SELECT r.Rank, c.Name, r.TP, r.Wins, r.Loses, r.FinalWins, r.Grade
	FROM
	(
		SELECT rh.Rank, ci.CID, ci.Wins, ci.Loses, ci.FinalWins, ci.TP, rh.Grade
		FROM DTCharacterRankingHistory rh, DTCharacterInfo ci
		WHERE rh.CID = ci.CID
			AND ci.TimeStamp = @TimeStamp
			AND rh.TimeStamp = @TimeStamp
			AND rh.Rank BETWEEN (@PagePerNum * (@PageNum - 1) + 1) AND (@PagePerNum * (@PageNum))
	) r, Character c
	WHERE r.CID = c.CID
		AND c.DeleteFlag = 0	
END