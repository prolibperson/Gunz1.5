USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spSelectAccountItem', 'BackUp_spSelectAccountItem';
GO

CREATE PROC dbo.spSelectAccountItem
-- ALTER PROC dbo.spSelectAccountItem
	@AID   int    
AS BEGIN
	SET NOCOUNT ON  
		    
	SELECT	ai.AIID, ai.ItemID
			, (ai.RentHourPeriod * 60) - (DateDiff(n, ai.RentDate, GETDATE())) AS RentPeriodRemainder
			, ISNULL(RentHourPeriod, 0) AS 'RentHourPeriod'
			, ISNULL(ai.Cnt, 1) AS 'CNT'
	FROM	dbo.AccountItem ai(NOLOCK)
	WHERE	ai.AID = @AID 
	ORDER BY ai.AIID
		
END
GO

-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spSelectAccountItem
EXEC sp_rename 'BackUp_spSelectAccountItem', 'spSelectAccountItem';
*/