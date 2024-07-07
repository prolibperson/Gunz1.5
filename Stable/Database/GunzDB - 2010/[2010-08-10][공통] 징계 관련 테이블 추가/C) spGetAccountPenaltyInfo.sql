USE GunzDB
GO


CREATE PROC dbo.spGetAccountPenaltyInfo
-- ALTER PROC dbo.spGetAccountPenaltyInfo
	@AID			INT	
AS BEGIN
	SET NOCOUNT ON
	
  	SELECT	a.PEndDate, PCode
	FROM	AccountPenaltyGMLog a(NOLOCK)
	WHERE	a.AID = @AID
	AND		a.Reset_Date IS NULL
END
GO