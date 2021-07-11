USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spUpdateEquipItem', 'BackUp_spUpdateEquipItem'
GO

CREATE PROC dbo.spUpdateEquipItem
-- ALTER PROC dbo.spUpdateEquipItem
	@CID		INT,
	@SlotID		INT,
	@CIID		INT,
	@ItemID		INT
AS BEGIN
 
	SET NOCOUNT ON;
  	
	IF( @CIID <> 0 ) BEGIN
	
		DECLARE @OldCIID INT;
		
		------------------------------------------------------------------
		
		IF( @SlotID = 10 ) BEGIN
		
			SELECT	@OldCIID = CIID
			FROM	CharacterEquipmentSlot(NOLOCK) 
			WHERE	CID = @CID 
			AND		CIID = @CIID
			AND		SlotID = 11;
			
			IF( @OldCIID IS NOT NULL ) BEGIN		
				SELECT -1 AS 'Ret';
				RETURN;				
			END			
		END
		ELSE IF( @SlotID = 11 ) BEGIn
		
			SELECT	@OldCIID = CIID
			FROM	CharacterEquipmentSlot(NOLOCK) 
			WHERE	CID = @CID 
			AND		CIID = @CIID
			AND		SlotID = 10;
			
			IF( @OldCIID IS NOT NULL ) BEGIN		
				SELECT -1 AS 'Ret';
				RETURN;				
			END			
		END	
		
		------------------------------------------------------------------
	END
	
	
	UPDATE  dbo.CharacterEquipmentSlot
	SET		CIID = @CIID, ItemID = @ItemID
	WHERE	CID = @CID 
	AND		SlotID = @SlotID
			
	SELECT 0 AS 'Ret';
	RETURN;
	
END

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spUpdateEquipItem
EXEC sp_rename 'BackUp_spUpdateEquipItem', 'spUpdateEquipItem'
*/