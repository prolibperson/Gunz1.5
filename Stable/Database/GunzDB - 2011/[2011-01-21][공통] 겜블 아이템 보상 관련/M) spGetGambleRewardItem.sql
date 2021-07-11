ALTER PROC spGetGambleRewardItem  
AS  
BEGIN  
	SET NOCOUNT ON;
	
	SELECT	GRIID, GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand   
	FROM	GambleRewardItem(NOLOCK)  
END   