USE GunzDB
GO

-- 일단, 기존의 spInsertPlayerLog를 백업한다.
EXEC sp_rename 'spInsertPlayerLog', 'BackUp_spInsertPlayerLog'
GO

-- 현재의 spInsertPlayerLog를 만든다.
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
	VALUES(@CID, @DisTime, @PlayTime, @Kills, @Deaths, @XP, @TotalXP)
	
	-- 아래의 날짜가 지나면 아래의 쿼리 부분은 삭제해주자!
	IF( @DisTime BETWEEN '2010-12-01 00:00:00.0' AND '2011-01-06 00:00:00.0' ) 
	BEGIN
	
		EXEC spEventColiseum_UpdatePlayData @CID, @PlayTime, @Kills, @Deaths;
		
	END
END
