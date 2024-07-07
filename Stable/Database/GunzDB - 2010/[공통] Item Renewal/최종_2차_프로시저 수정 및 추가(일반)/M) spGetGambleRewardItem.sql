USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- 작업할 쿼리
EXEC sp_rename 'spGetGambleRewardItem', 'BackUp_spGetGambleRewardItem';
GO

CREATE PROC spGetGambleRewardItem  
-- ALTER PROC spGetGambleRewardItem  
AS  
BEGIN  
	SET NOCOUNT ON;
	
	SELECT	GRIID, GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand   
	FROM	GambleRewardItem(NOLOCK)  
END   