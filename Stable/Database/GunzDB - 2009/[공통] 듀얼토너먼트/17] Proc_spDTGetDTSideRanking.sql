ALTER PROCEDURE dbo.spDTGetDTSideRanking
	@CID	INT
AS BEGIN
	SET NOCOUNT ON;
-----------------------------------------------------------------

	DECLARE @CurRank	INT;
	DECLARE @TotalUser	INT;
	DECLARE @TimeStamp	CHAR(8);
	
	SELECT TOP 1 @TimeStamp = ts.TimeStamp 
	FROM DTTimeStamp ts WITH (NOLOCK) 
	ORDER BY ts.TimeStamp DESC;
	
	SELECT @CurRank = cr.Rank 
	FROM DTCharacterRanking cr (NOLOCK) 
	WHERE cr.CID = @CID;
	
	SELECT @TotalUser = COUNT(*) 
	FROM DTCharacterRanking cr (NOLOCK) 
	
-----------------------------------------------------------------	

	DECLARE @FactorA INT;
	DECLARE @FactorB INT;

	IF( @CurRank = 1 ) BEGIN
		SET @FactorA = 0;
		SET @FactorB = 4;
	END	ELSE IF( @CurRank = 2 ) BEGIN 
		SET @FactorA = 1;
		SET @FactorB = 3;
	END	ELSE IF( @CurRank - 1 = @TotalUser) BEGIN
		SET @FactorA = 4;
		SET @FactorB = 1;
	END	ELSE IF( @CurRank = @TotalUser ) BEGIN
		SET @FactorA = 4;
		SET @FactorB = 0;
	END ELSE IF( @CurRank IS NULL ) BEGIN
		SET @CurRank = 3;
		SET @FactorA = 2;
		SET @FactorB = 2;		
	END ELSE BEGIN
		SET @FactorA = 2;
		SET @FactorB = 2;

	END
	
-----------------------------------------------------------------

	SELECT c.Name, t.TP, t.Wins, t.Loses, t.FinalWins, t.Rank, t.RankingIncrease, t.PreGrade
	FROM
	(
		SELECT cr.CID, cr.TP, cr.Wins, cr.Loses, cr.FinalWins, cr.Rank, cr.RankingIncrease, cr.PreGrade
		FROM DTCharacterRanking cr WITH (NOLOCK)
		WHERE cr.Rank > 0
			AND cr.Rank BETWEEN @CurRank - @FactorA AND @CurRank + @FactorB
			AND cr.TimeStamp = @TimeStamp
	) t, Character c WITH (NOLOCK)
	WHERE t.CID = c.CID
		AND c.DeleteFlag = 0
	ORDER BY t.Rank
	
-----------------------------------------------------------------
END
