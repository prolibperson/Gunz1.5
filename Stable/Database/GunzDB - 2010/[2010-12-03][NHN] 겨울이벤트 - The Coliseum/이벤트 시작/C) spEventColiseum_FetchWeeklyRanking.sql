USE GunzDB
GO

CREATE PROC dbo.spEventColiseum_FetchWeeklyRanking
-- ALTER PROC dbo.spEventColiseum_FetchWeeklyRanking
	@TargetWeekDate		DATETIME
AS BEGIN

	SET NOCOUNT ON;
	
	DECLARE @DateTag		DATETIME;
	DECLARE @PreDateTag		CHAR(10);
	DECLARE @CurDateTag		CHAR(10);
	DECLARE @StartDateTag	CHAR(10);
	DECLARE @EndDateTag		CHAR(10);
	
	SELECT	@DateTag = CASE WHEN DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetWeekDate)) < 5
							THEN DATEADD(dd, DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetWeekDate)) * (-1) + 4, @TargetWeekDate)
							ELSE DATEADD(dd, 11 - DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetWeekDate)), @TargetWeekDate)
						END
	
	SELECT	@StartDateTag = CONVERT( CHAR(8), DATEADD(dd, -6, @DateTag), 112) + '_D'
			, @EndDateTag = CONVERT( CHAR(8), @DateTag, 112) + '_D'
			, @PreDateTag = CONVERT( CHAR(8), DATEADD(dd, -6, @DateTag), 112) + '_W'
			, @CurDateTag = CONVERT( CHAR(8), @DateTag, 112) + '_W'
	--------------------------------------------------------------------------------------------------
	
	DELETE dbo.Event_Coliseum_PlayData_NHN			WHERE [DateTag] = @CurDateTag;
	DELETE dbo.Event_Coliseum_KillsRanking_NHN		WHERE [DateTag] = @CurDateTag;
	DELETE dbo.Event_Coliseum_DeathsRanking_NHN		WHERE [DateTag] = @CurDateTag;
	DELETE dbo.Event_Coliseum_PlayTimeRanking_NHN	WHERE [DateTag] = @CurDateTag;
		
	--------------------------------------------------------------------------------------------------	
	
	INSERT dbo.Event_Coliseum_PlayData_NHN([DateTag], CID, PlayTime, Kills, Deaths, LastUpdatedDate)
		SELECT	@CurDateTag AS [DateTag], CID
				, SUM(PlayTime) AS PlayTime, SUM(Kills) AS Kills, SUM(Deaths) AS Deaths
				, GETDATE()
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] BETWEEN @StartDateTag AND @EndDateTag
		AND		[DateTag] <> @PreDateTag
		GROUP BY CID

	INSERT Event_Coliseum_KillsRanking_NHN([Rank], [DateTag], CID)
		SELECT	ROW_NUMBER() OVER (ORDER BY (CAST(Kills AS FLOAT)/ CAST(PlayTime AS FLOAT)) DESC, PlayTime ASC) AS [Rank]
				, [DateTag], CID
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] = @CurDateTag
		
	INSERT Event_Coliseum_DeathsRanking_NHN([Rank], [DateTag], CID)
		SELECT	ROW_NUMBER() OVER (ORDER BY (CAST(Deaths AS FLOAT)/ CAST(PlayTime AS FLOAT)) DESC, PlayTime ASC) AS [Rank]
				, [DateTag], CID
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] = @CurDateTag
		
	INSERT Event_Coliseum_PlayTimeRanking_NHN([Rank], [DateTag], CID)
		SELECT	ROW_NUMBER() OVER (ORDER BY PlayTime DESC) AS [Rank]
				, [DateTag], CID
		FROM	dbo.Event_Coliseum_PlayData_NHN
		WHERE	[DateTag] = @CurDateTag

	--------------------------------------------------------------------------------------------------			
	
	UPDATE	ed
	SET		ed.KillRank = er.[Rank]
	FROM	Event_Coliseum_KillsRanking_NHN er, Event_Coliseum_PlayData_NHN ed
	WHERE	er.CID = ed.CID
	AND		er.[DateTag] = @CurDateTag
	AND		er.[DateTag] = ed.[DateTag]
	
	UPDATE	ed
	SET		ed.DeathRank = er.[Rank]
	FROM	Event_Coliseum_DeathsRanking_NHN er, Event_Coliseum_PlayData_NHN ed
	WHERE	er.CID = ed.CID
	AND		er.[DateTag] = @CurDateTag
	AND		er.[DateTag] = ed.[DateTag]
	
	
	UPDATE	ed
	SET		ed.PlayTimeRank = er.[Rank]
	FROM	Event_Coliseum_PlayTimeRanking_NHN er, Event_Coliseum_PlayData_NHN ed
	WHERE	er.CID = ed.CID
	AND		er.[DateTag] = @CurDateTag
	AND		er.[DateTag] = ed.[DateTag]
	
	--------------------------------------------------------------------------------------------------
END