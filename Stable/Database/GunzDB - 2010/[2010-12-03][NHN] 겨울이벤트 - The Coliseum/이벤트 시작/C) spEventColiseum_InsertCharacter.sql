USE GunzDB
GO

CREATE PROC dbo.spEventColiseum_InsertCharacter
-- ALTER PROC dbo.spEventColiseum_InsertCharacter
	@CID	INT
AS BEGIN
	
	SET NOCOUNT ON;
	
	DECLARE @EventCID	INT;
	
	SELECT	@EventCID = CID 
	FROM	Event_Coliseum_Character_NHN 
	WHERE	CID = @CID
		
	IF( EXISTS (SELECT CID FROM Event_Coliseum_Character_NHN(NOLOCK) WHERE CID = @CID ))
		RETURN -1;
		
	
	INSERT Event_Coliseum_Character_NHN(CID, RegDate) 
	VALUES (@CID, GETDATE());
	
	RETURN 0;		
END
