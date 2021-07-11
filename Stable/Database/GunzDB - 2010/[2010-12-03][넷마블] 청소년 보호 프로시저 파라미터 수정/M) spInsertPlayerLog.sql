USE GunzDB
GO

CREATE PROC [dbo].[spInsertPlayerLog]
-- ALTER PROC [dbo].[spInsertPlayerLog]
	@CID			INT
	, @PlayTime		INT
	, @Kills		INT
	, @Deaths		INT
	, @XP			INT
	, @TotalXP		INT
AS BEGIN

	SET NOCOUNT ON;  
  
	DECLARE @DisTime DATETIME;  
	SET @DisTime = GETDATE();  
	  
	INSERT INTO PlayerLog(CID, DisTime, PlayTime, Kills, Deaths, XP, TotalXP)  
	VALUES(@CID, @DisTime, @PlayTime, @Kills, @Deaths, @XP, @TotalXP);	
END