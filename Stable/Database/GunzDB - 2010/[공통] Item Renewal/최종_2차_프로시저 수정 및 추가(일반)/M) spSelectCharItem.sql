USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spSelectCharItem', 'BackUp_spSelectCharItem'
GO

CREATE PROC dbo.spSelectCharItem
-- ALTER PROC dbo.spSelectCharItem  
	@CID  INT  
AS BEGIN    
	SET NOCOUNT ON
	 
	SELECT	CIID, ItemID
			, (RentHourPeriod*60) - (DateDiff(n, RentDate, GETDATE())) AS RentPeriodRemainder  
			, ISNULL(RentHourPeriod, 0) as 'RentHourPeriod'
			, ISNULL(Cnt, 1) AS 'Cnt'
	FROM	dbo.CharacterItem (NOLOCK)  
	WHERE	CID = @CID
	AND		Cnt > 0
	ORDER BY CIID
END  
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ÇÒ Äõ¸®
/*
DROP PROC spSelectCharItem
EXEC sp_rename 'BackUp_spSelectCharItem', 'spSelectCharItem'
*/
