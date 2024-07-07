USE GunzDB
GO

CREATE PROC dbo.spWebGetActiveCharacterList_NHN
	@AID		INT
AS BEGIN
	SET NOCOUNT ON;	
	
	SELECT	c.AID, c.CID, c.Name, c.CharNum
	FROM	Character c(NOLOCK)
	WHERE	c.AID = @AID AND c.DeleteFlag = 0
END