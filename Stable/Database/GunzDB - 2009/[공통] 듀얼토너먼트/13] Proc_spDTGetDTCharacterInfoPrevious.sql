ALTER PROCEDURE dbo.spDTGetDTCharacterInfoPrevious
	@CID				INT
AS BEGIN  
	SET NOCOUNT ON;
	
	DECLARE @TimeStampIndex INT;
	DECLARE @TimeStamp CHAR(8);	
	DECLARE @PreTimeStamp CHAR(8);	
	
	SELECT TOP 1 @TimeStampIndex = ts.ID, @TimeStamp = ts.TimeStamp 
	FROM DTTimeStamp ts(NOLOCK) 
	ORDER BY ts.TimeStamp DESC;
	
	SELECT @PreTimeStamp = TimeStamp 
	FROM DTTimeStamp 
	WHERE ID = @TimeStampIndex - 1;
   	
	SELECT dc.TP, dc.Wins, dc.Loses, dc.Ranking, dc.FinalWins
	FROM DTCharacterInfo dc WITH (NOLOCK)
	WHERE dc.CID = @CID AND dc.TimeStamp = @PreTimeStamp;
END
