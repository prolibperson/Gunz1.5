USE GunzDB
GO


CREATE PROC dbo.spUpdateServerInfo
-- ALTER PROC dbo.spUpdateServerInfo
	@ServerID		INT,
	@ServerName		VARCHAR(24),
	@MaxPlayer		INT	
AS BEGIN
	SET NOCOUNT ON
	
  	UPDATE	ServerStatus 
  	SET		MaxPlayer = MaxPlayer, ServerName = @ServerName
  	WHERE	ServerID = @ServerID;
END
GO