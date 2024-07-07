ALTER PROCEDURE dbo.spDTInsertDTGameLog
	@TournamentType TINYINT, @MatchFactor TINYINT,
	@Player1 INT,	@Player2 INT,	@Player3 INT,	@Player4 INT,	
	@Player5 INT,	@Player6 INT,	@Player7 INT,	@Player8 INT
AS BEGIN
	SET NOCOUNT ON;
	
	DECLARE @TimeStamp CHAR(8);   	
	
	SELECT TOP 1 @TimeStamp = ts.TimeStamp 
	FROM DTTimeStamp ts(NOLOCK) 
	ORDER BY ts.TimeStamp DESC; 
   	
	INSERT dbo.DTGameLog(TimeStamp, TournamentType, StartTime, MatchFactor, Player1CID, Player2CID, Player3CID, Player4CID, Player5CID, Player6CID, Player7CID, Player8CID)
	VALUES(@TimeStamp, @TournamentType, GETDATE(), @MatchFactor, @Player1, @Player2, @Player3, @Player4, @Player5, @Player6, @Player7, @Player8);
	
	SELECT @@IDENTITY AS LogID, @TimeStamp AS TimeStamp;
END