USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
CREATE PROC dbo.spUpdateCharItemSpendCount
-- ALTER PROC dbo.spUpdateCharItemSpendCount
	@CIID		INT,
	@SpendCount	INT
AS BEGIN

	SET NOCOUNT ON;
		
	DECLARE @Cnt				INT;
	DECLARE @CID				INT;
	DECLARE @ItemID				INT;
	
	BEGIN TRAN ----------
			
		SELECT	@CID = CID, @ItemID = ItemID, @Cnt = ISNULL(Cnt, 1)
		FROM	CharacterItem 
		WHERE	CIID = @CIID;
		AND		CID IS NOT NULL
				
		IF( @SpendCount = @Cnt ) BEGIN
		
			UPDATE	CharacterItem 
			SET		CID = NULL
			WHERE	CIID = @CIID
			
			IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
				ROLLBACK TRAN
				SELECT -1 AS 'Ret'
				RETURN;
			END	
			
			UPDATE	CharacterEquipmentSlot 
			SET		CIID = NULL, ItemID = NULL
			WHERE	CID = @CID
			AND		CIID = @CIID
			AND		ItemID = @ItemID;
			
			IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
				ROLLBACK TRAN
				SELECT -2 AS 'Ret'
				RETURN;
			END	
			
		END
		ELSE BEGIN
		
			UPDATE	CharacterItem 
			SET		Cnt = Cnt - @SpendCount 
			WHERE	CIID = @CIID
			AND		Cnt - @SpendCount > 0;
			
			IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
				ROLLBACK TRAN
				SELECT -3 AS 'Ret'
				RETURN;
			END		
			
		END
		
		INSERT ItemChangeLog_CharacterItem(ChangeType, ChangeDate, CID, CIID, ItemID, [Count])
		VALUES (204, GETDATE(), @CID, @CIID, @ItemID, @SpendCount);
		
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
			ROLLBACK TRAN
			SELECT -4 AS 'Ret'
			RETURN;
		END		
		
	COMMIT TRAN ----------		
	
	SELECT 0 AS 'Ret'
END
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spUpdateCharItemSpendCount
*/