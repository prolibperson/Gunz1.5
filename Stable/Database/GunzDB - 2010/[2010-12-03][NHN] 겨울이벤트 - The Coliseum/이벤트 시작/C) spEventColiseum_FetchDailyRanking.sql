USE GunzDB
GO

CREATE PROC dbo.spEventColiseum_FetchDailyRanking
-- ALTER PROC dbo.spEventColiseum_FetchDailyRanking
	@TargetDate		DATETIME
AS BEGIN

	SET NOCOUNT ON;
	
	DECLARE @DateTag CHAR(10);	
	-- SELECT @DateTag = CONVERT( CHAR(8), DATEADD(dd, DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetDate))*(-1)+4, @TargetDate), 112);
	SELECT @DateTag = CONVERT( CHAR(8), @TargetDate, 112) + '_D';
	
	--------------------------------------------------------------------------------------------------
		
	DELETE Event_Coliseum_KillsRanking_NHN		WHERE [DateTag] = @DateTag;
	DELETE Event_Coliseum_DeathsRanking_NHN		WHERE [DateTag] = @DateTag;
	DELETE Event_Coliseum_PlayTimeRanking_NHN	WHERE [DateTag] = @DateTag;
		
	--------------------------------------------------------------------------------------------------	
	
	INSERT Event_Coliseum_KillsRanking_NHN([Rank], [DateTag], CID)
		SELECT	ROW_NUMBER() OVER (ORDER BY (CAST(Kills AS FLOAT)/ CAST(PlayTime AS FLOAT)) DESC, PlayTime ASC) AS [Rank]
				, [DateTag], CID
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] = @DateTag
		
	INSERT Event_Coliseum_DeathsRanking_NHN([Rank], [DateTag], CID)
		SELECT	ROW_NUMBER() OVER (ORDER BY (CAST(Deaths AS FLOAT)/ CAST(PlayTime AS FLOAT)) DESC, PlayTime ASC) AS [Rank]
				, [DateTag], CID
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] = @DateTag
		
	INSERT Event_Coliseum_PlayTimeRanking_NHN([Rank], [DateTag], CID)
		SELECT	ROW_NUMBER() OVER (ORDER BY PlayTime DESC) AS [Rank]
				, [DateTag], CID
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] = @DateTag
	
	--------------------------------------------------------------------------------------------------			
	
	UPDATE	ed
	SET		ed.KillRank = er.[Rank]
	FROM	Event_Coliseum_KillsRanking_NHN er, Event_Coliseum_PlayData_NHN ed
	WHERE	er.CID = ed.CID
	AND		er.[DateTag] = @DateTag
	AND		er.[DateTag] = ed.[DateTag]
	
	UPDATE	ed
	SET		ed.DeathRank = er.[Rank]
	FROM	Event_Coliseum_DeathsRanking_NHN er, Event_Coliseum_PlayData_NHN ed
	WHERE	er.CID = ed.CID
	AND		er.[DateTag] = @DateTag
	AND		er.[DateTag] = ed.[DateTag]
	
	
	UPDATE	ed
	SET		ed.PlayTimeRank = er.[Rank]
	FROM	Event_Coliseum_PlayTimeRanking_NHN er, Event_Coliseum_PlayData_NHN ed
	WHERE	er.CID = ed.CID
	AND		er.[DateTag] = @DateTag
	AND		er.[DateTag] = ed.[DateTag]
	
	--------------------------------------------------------------------------------------------------
END