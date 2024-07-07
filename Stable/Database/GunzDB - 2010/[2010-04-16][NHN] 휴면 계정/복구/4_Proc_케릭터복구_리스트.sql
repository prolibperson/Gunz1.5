USE GunzDB
GO

CREATE PROC dbo.spWebGetSleepCharacterList_NHN
	@AID		INT
AS BEGIN
	SET NOCOUNT ON;	
	
	SELECT	t.AID
		  , t.CID
		  , t.Name
		  , -1 AS CharNum
		  , CASE WHEN EXISTS (SELECT Name FROM Character(NOLOCK) WHERE Name = t.Name) 
				 THEN 0 
				 ELSE 1 
			END AS NameUsed
	FROM	SleepCharacterNHN t(NOLOCK)
	WHERE	t.AID = @AID
END



