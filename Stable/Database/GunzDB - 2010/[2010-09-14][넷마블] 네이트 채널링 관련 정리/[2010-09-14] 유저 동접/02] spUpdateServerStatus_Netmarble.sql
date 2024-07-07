USE GunzDB
GO

CREATE PROCEDURE dbo.spUpdateServerStatus_Netmarble
  @CurrPlayer   SMALLINT,  
  @NatePlayer	SMALLINT,
  @ServerID     INT
AS BEGIN
  
	UPDATE	ServerStatus   
	Set		CurrPlayer = @CurrPlayer, NatePlayer = @NatePlayer, Time=GETDATE()   
	WHERE	ServerID=@ServerID

END