USE GunzDB
GO

CREATE PROCEDURE dbo.spGetServerUserCount
AS BEGIN
	SET NOCOUNT ON;
	  
	SELECT	SUM(CurrPlayer)						AS TotalCurrentPlayer
			, SUM(CurrPlayer) - SUM(NatePlayer)	AS TotalNetmarblePlayer
			, SUM(NatePlayer)					AS TotalNatePlayer
	FROM	ServerStatus
END