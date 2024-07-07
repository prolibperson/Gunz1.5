ALTER PROCEDURE dbo.spDTUpdateDTCharacterInfo
	@CID			INT,
	@TimeStamp		CHAR(8),
	@TP				INT,
	@Wins			INT,
	@Loses			INT,
	@FGames			INT,
	@FWins			INT,
	@SFGames		INT,
	@SFWins			INT,
	@QFGames		INT,
	@QFWins			INT,
	@LeaveCount		INT
AS BEGIN  
	SET NOCOUNT ON;
	
	UPDATE dbo.DTCharacterInfo
	SET TP = @TP
		, Wins			= @Wins
		, Loses			= @Loses
		, FinalGames	= @FGames
		, FinalWins		= @FWins
		, SFinalGames	= @SFGames
		, SFinalWins	= @SFWins
		, QFinalGames	= @QFGames
		, QFinalWins	= @QFWins
		, LeaveCount	= @LeaveCount
	WHERE CID = @CID AND TimeStamp = @TimeStamp
END