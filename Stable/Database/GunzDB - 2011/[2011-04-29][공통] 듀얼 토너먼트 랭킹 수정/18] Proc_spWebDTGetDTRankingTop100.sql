CREATE PROC dbo.spWebDTGetDTRankingTOP100
-- ALTER PROC dbo.spWebDTGetDTRankingTOP100
	@TimeStamp	CHAR(8),
	@PageNum	INT
AS BEGIN

	DECLARE @PagePerNum INT;
	SET @PagePerNum = 20;
		
	SELECT  rh.Rank, rh.Name, ci.TP, ci.Wins, ci.Loses, ci.FinalWins, rh.Grade
	FROM    DTCharacterRankingHistory rh 
            JOIN DTCharacterInfo ci
            ON rh.CID = ci.CID
	WHERE   ci.TimeStamp = @TimeStamp
    AND     rh.TimeStamp = @TimeStamp
    AND     rh.Rank BETWEEN (@PagePerNum * (@PageNum - 1) + 1) AND (@PagePerNum * (@PageNum))
    ORDER BY rh.Rank

END