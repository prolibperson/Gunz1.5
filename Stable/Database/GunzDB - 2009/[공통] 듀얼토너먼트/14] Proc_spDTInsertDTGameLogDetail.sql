ALTER PROCEDURE dbo.spDTInsertDTGameLogDetail
	@LogID		INT,
	@TimeStamp	CHAR(8),
	@MatchType	TINYINT,
	@PlayTime	INT,
	@WinnerCID	INT,
	@GainTP		INT,
	@LoserCID	INT,
	@LoseTP		INT
AS BEGIN
	SET NOCOUNT ON;
	
	INSERT INTO dbo.DTGameLogDetail
	VALUES(@TimeStamp, @LogID
		, DATEADD(ss, -@PlayTime, GETDATE()), @PlayTime, @MatchType
		, @WinnerCID, @LoserCID, @GainTP, @LoseTP);
END