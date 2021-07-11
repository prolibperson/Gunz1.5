ALTER PROCEDURE dbo.spDTGetDTCharacterInfo
	@CID INT
AS BEGIN	
	SET NOCOUNT ON;
	
	DECLARE @TimeStamp CHAR(8);
	
	SELECT TOP 1 @TimeStamp = ts.TimeStamp 
	FROM DTTimeStamp ts WITH (NOLOCK) 
	ORDER BY TimeStamp DESC; 
   	
	SELECT * 
	FROM DTCharacterInfo dc WITH (NOLOCK) 
	WHERE dc.CID = @CID AND dc.TimeStamp = @TimeStamp;
END