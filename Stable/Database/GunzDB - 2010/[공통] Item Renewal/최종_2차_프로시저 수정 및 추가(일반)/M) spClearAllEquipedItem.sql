USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spClearAllEquipedItem', 'BackUp_spClearAllEquipedItem';
GO

CREATE PROC dbo.spClearAllEquipedItem 
-- ALTER PROC dbo.spClearAllEquipedItem 
	@CID  INT
AS BEGIN 

	SET NOCOUNT ON;

	UPDATE	CharacterEquipmentSlot
	SET		CIID = NULL, ItemID = NULL
	WHERE	CID = @CID;

END  
  
-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spClearAllEquipedItem
EXEC sp_rename 'BackUp_spClearAllEquipedItem', 'spClearAllEquipedItem';
*/

