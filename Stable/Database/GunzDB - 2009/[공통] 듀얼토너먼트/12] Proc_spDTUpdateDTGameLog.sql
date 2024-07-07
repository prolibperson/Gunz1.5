ALTER PROCEDURE dbo.spDTUpdateDTGameLog
	@TimeStamp	CHAR(8),
	@LogID		INT,	
	@ChampCID	INT	
AS BEGIN
	SET NOCOUNT ON;
	
	UPDATE DTGameLog
	SET ChampCID = @ChampCID, EndTime = GETDATE()
	WHERE LogID = @LogID AND TimeStamp = @TimeSTamp
END