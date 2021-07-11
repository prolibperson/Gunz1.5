USE GunzDB
GO

CREATE PROC dbo.spEventColiseum_UpdatePlayData
-- ALTER PROC dbo.spEventColiseum_UpdatePlayData
	@CID			INT
	, @PlayTime		INT
	, @Kills		INT
	, @Deaths		INT
AS BEGIN

	SET NOCOUNT ON;  	
	
	DECLARE @DateTag CHAR(10);
	-- SELECT @DateTag = CONVERT( CHAR(8), DATEADD(dd, DATEPART(WEEKDAY, CONVERT(DATETIME, GETDATE()))*(-1)+4, CONVERT(DATETIME, GETDATE())), 112);
	SELECT @DateTag = CONVERT( CHAR(8), GETDATE(), 112) + '_D';
	
	IF( NOT EXISTS ( SELECT CID FROM dbo.Event_Coliseum_Character_NHN WHERE CID = @CID ) ) RETURN;	
	
	IF( EXISTS ( SELECT CID FROM dbo.Event_Coliseum_PlayData_NHN WHERE CID = @CID AND [DateTag] = @DateTag ) ) 
	BEGIN
	
		UPDATE	dbo.Event_Coliseum_PlayData_NHN
		SET		PlayTime = PlayTime + @PlayTime
				, Kills = Kills + @Kills
				, Deaths = Deaths + @Deaths
				, LastUpdatedDate = GETDATE()
		WHERE	[DateTag] = @DateTag
		AND		CID = @CID;
		
	END
	ELSE BEGIN
	
		INSERT dbo.Event_Coliseum_PlayData_NHN([DateTag], CID, PlayTime, Kills, Deaths, LastUpdatedDate)
		VALUES (@DateTag, @CID, @PlayTime, @Kills, @Deaths, GETDATE());
		
	END					
END