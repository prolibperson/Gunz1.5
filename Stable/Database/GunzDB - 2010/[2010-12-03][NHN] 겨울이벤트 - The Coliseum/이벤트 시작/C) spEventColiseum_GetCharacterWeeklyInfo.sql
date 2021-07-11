USE GunzDB
GO

CREATE PROC dbo.spEventColiseum_GetCharacterWeeklyInfo
-- ALTER PROC dbo.spEventColiseum_GetCharacterWeeklyInfo
	@CID				INT
	, @TargetWeekDate	DATETIME
AS BEGIN

	SET NOCOUNT ON;
	
	DECLARE @DateTag		DATETIME;
	DECLARE @WeeklyDateTag	CHAR(10);
		
	SELECT	@DateTag = CASE WHEN DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetWeekDate)) < 5
							THEN DATEADD(dd, DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetWeekDate)) * (-1) + 4, @TargetWeekDate)
							ELSE DATEADD(dd, 11 - DATEPART(WEEKDAY, CONVERT(DATETIME, @TargetWeekDate)), @TargetWeekDate)
						END
		
	SELECT	@WeeklyDateTag = CONVERT( CHAR(8), @DateTag, 112) + '_W';
	
	----------------------------------------------------------------------------------------------------
	
	SELECT	[DateTag], PlayTime, Kills, Deaths, PlayTimeRank, KillRank, DeathRank
	FROM	Event_Coliseum_PlayData_NHN(NOLOCK)
	WHERE	CID = @CID
	AND		[DateTag] = @WeeklyDateTag;
	
	----------------------------------------------------------------------------------------------------
	
END