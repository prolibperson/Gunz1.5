USE GunzDB
GO

ALTER TABLE TPLogNetmarble ADD CCode INT DEFAULT(0);

CREATE PROCEDURE dbo.spInsertTPLogNetmarble
-- ALTER PROCEDURE dbo.spInsertTPLogNetmarble
	@CID		INT    
	, @PlayTime INT
AS BEGIN    
	SET NOCOUNT ON;
	
	DECLARE @DisTime DATETIME
	SET @DisTime = GETDATE();

	INSERT TPLogNetmarble(StartDate, EndDate, UserID, CharacName, IP, PlayTime, Password, CCode)
		SELECT DATEADD(ss, -@PlayTime, @DisTime) AS StartDate    
				, @DisTime AS EndDate    
				, a.UserID    
				, ( SELECT Name FROM Character(NOLOCK) WHERE CID = @CID) AS CharacName    
				, CAST(cl.IPPart1 AS VARCHAR(3)) + '.' + CAST(cl.IPPart2 AS VARCHAR(3)) + '.'     
					+ CAST(cl.IPPart3 AS VARCHAR(3)) + '.' + CAST(cl.IPPart4 AS VARCHAR(3))    
				, @PlayTime AS PlayTime    
				, l.Password  
				, a.CCode  
		FROM Login l(NOLOCK)
			, Account a(NOLOCK)    
			, ( SELECT TOP 1 AID, IPPart1, IPPart2, IPPart3, IPPart4    
				FROM LogDB.dbo.ConnLog    
				WHERE AID IN (	SELECT	AID     
								FROM	Character(NOLOCK)    
								WHERE CID = @CID )    
				ORDER BY Time DESC) AS cl    
		WHERE a.AID = cl.AID   
		AND  l.UserID = a.UserID;    
END