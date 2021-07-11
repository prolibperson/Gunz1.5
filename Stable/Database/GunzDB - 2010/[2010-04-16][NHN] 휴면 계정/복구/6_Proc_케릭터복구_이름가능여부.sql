USE GunzDB
GO

CREATE PROC dbo.spWebIsValidCharacterName_NHN
	@NewName	VARCHAR(24)
AS BEGIN
	SET NOCOUNT ON;	
	
	IF( EXISTS(SELECT CID FROM dbo.Character(NOLOCK) WHERE Name = @NewName) ) 
		RETURN -1;

    IF( EXISTS(SELECT id FROM dbo.AbuseList(NOLOCK) WHERE @NewName LIKE Word) )
        RETURN -2;

	RETURN 0;
END


