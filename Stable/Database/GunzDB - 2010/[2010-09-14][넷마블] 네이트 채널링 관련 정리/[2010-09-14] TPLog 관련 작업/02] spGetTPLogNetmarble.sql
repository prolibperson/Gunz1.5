USE GunzDB
GO

CREATE PROCEDURE dbo.spGetTPLogNetmarbleTemp	-- 테스트할 땐.. 임시 프로시저로..
--DROP PROCEDURE dbo.spGetTPLogNetmarbleTemp	-- 임시 삭제할 때..
--ALTER PROCEDURE dbo.spGetTPLogNetmarble		-- 결국엔 Alter
	@StartDate DATETIME  
	, @EndDate DATETIME  
AS BEGIN  
	SET NOCOUNT ON;  
   
	SELECT	CONVERT(VARCHAR(8), StartDate, 112) AS Date  
			, Password AS CN  
			, UserID  AS UserID  
			, CharacName AS CharacName  
			, StartDate AS LoginDate  
			, EndDate AS LogoutDate  
			, PlayTime AS PlayTime  
			, IP AS IP
			, CCode AS CCode
	FROM TPLogNetmarble(NOLOCK)  
	WHERE StartDate BETWEEN @StartDate AND @EndDate  
END