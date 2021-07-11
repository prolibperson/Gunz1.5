USE GunzDB
GO
-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spAdmWebAccountItemInfoByAID', 'BackUp_spAdmWebAccountItemInfoByAID';
GO

CREATE PROC dbo.spAdmWebAccountItemInfoByAID
-- ALTER PROC dbo.spAdmWebAccountItemInfoByAID
	@AID	INT
AS BEGIN

	SET NOCOUNT ON
	
	SELECT	ai.AIID, ai.RentHourPeriod, i.Name, ai.ItemID, ai.Cnt
			, CASE ISNULL(RentDate, 0)	
				WHEN 0 THEN '0'  
				ELSE (RentHourPeriod-DATEDIFF (hh, RentDate, GetDate()))    
			END AS RentRemain  
			, CASE ISNULL(ai.RentDate, 0)	
				WHEN 0 THEN '0'  
				ELSE CAST(ai.RentDate AS VARCHAR(24))  
			END as RentDate  
	FROM   AccountItem ai(NOLOCK) JOIN Item i(NOLOCK)   
			ON	ai.AID = @AID 
			AND i.ItemID = ai.ItemID  	
END

-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spAdmWebAccountItemInfoByAID
EXEC sp_rename 'BackUp_spAdmWebAccountItemInfoByAID', 'spAdmWebAccountItemInfoByAID';
*/