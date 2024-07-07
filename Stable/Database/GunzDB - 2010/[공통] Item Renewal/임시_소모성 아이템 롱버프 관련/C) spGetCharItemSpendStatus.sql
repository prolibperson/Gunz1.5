-- USE GunzDB
USE IGunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
CREATE PROC dbo.spGetCharacterBuffStatus
-- ALTER PROC dbo.spGetCharacterBuffStatus
	@CID INT  
AS BEGIN  
	SET NOCOUNT ON;  
   
	SELECT CBID, ItemID, StartPlayTime, BuffMinutePeriod, BuffMinutePeriod - (DateDiff(n, StartDate, GETDATE())) AS BuffPeriodRemainder
	FROM CharacterBuffStatus(NOLOCK)  
	WHERE CID = @CID
	ORDER BY CBID; 
   
END  

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC dbo.spGetCharacterBuffStatus
*/
