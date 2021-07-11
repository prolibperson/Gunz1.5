USE GunzDB
GO

ALTER PROCEDURE dbo.spInsertSurvivalGameLog
	@strGameName		VARCHAR(64),
	@nSID				TINYINT,
	@nTotalRound		SMALLINT,
	@nMasterPlayerCID	INT,
	@nGainRP1			INT,
	@nPlayer2CID		INT,
	@nGainRP2			INT,
	@nPlayer3CID		INT,
	@nGainRP3			INT,
	@nPlayer4CID		INT,
	@nGainRP4			INT,
	@nGamePlayTime		INT
AS
BEGIN
	SET NOCOUNT ON;
	
	DECLARE @StandardRankingPoint INT
	SET @StandardRankingPoint = 1000
	
	INSERT INTO dbo.SurvivalGameLog
	VALUES (DATEADD(n, -@nGamePlayTime, GETDATE()), GETDATE()
		, @nMasterPlayerCID, @nGainRP1
		, @nPlayer2CID, @nGainRP2
		, @nPlayer3CID, @nGainRP3
		, @nPlayer4CID, @nGainRP4
		, @strGameName, @nSID, @nTotalRound)
	
	-- Standard Ranking Point
	IF( @nGainRP1 >= @StandardRankingPoint )
		IF( @nMasterPlayerCID > 0 )
			EXECUTE spUpdateSurvivalCharacterInfo @nMasterPlayerCID, @nSID, @nGainRP1
	
	IF( @nGainRP2 >= @StandardRankingPoint )
		IF( @nPlayer2CID IS NOT NULL and @nPlayer2CID > 0 )
			EXECUTE spUpdateSurvivalCharacterInfo @nPlayer2CID, @nSID, @nGainRP2
			
	IF( @nGainRP3 >= @StandardRankingPoint )
		IF( @nPlayer3CID IS NOT NULL and @nPlayer3CID > 0 )
			EXECUTE spUpdateSurvivalCharacterInfo @nPlayer3CID, @nSID, @nGainRP3	
			
	IF( @nGainRP4 >= @StandardRankingPoint )
		IF( @nPlayer4CID IS NOT NULL and @nPlayer4CID > 0 )
			EXECUTE spUpdateSurvivalCharacterInfo @nPlayer4CID, @nSID, @nGainRP4	
			
	RETURN 0;
END
GO
