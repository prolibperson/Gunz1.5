USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®

CREATE PROC dbo.spUpdateCharGambleItemCount
-- ALTER PROC dbo.spUpdateCharGambleItemCount
	@CIID			INT,
	@GIID			INT,
	@GambleCount	INT
AS BEGIN

	SET NOCOUNT ON
	
	DECLARE @Cnt	INT;
	DECLARE @CID	INT;
	
	SELECT	@CID = CID,  @Cnt = ISNULL(Cnt, 1)
	FROM	CharacterItem 
	WHERE	CIID = @CIID
	AND		ItemID = @GIID
	AND		CID IS NOT NULL;
	
	BEGIN TRAN ----------
	
		IF( @GambleCount = @Cnt ) BEGIN
		
			UPDATE	CharacterItem 
			SET		CID = NULL
			WHERE	CIID = @CIID
			
			IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
				ROLLBACK TRAN
				SELECT -1 AS 'Ret'
				RETURN;
			END		
			
		END
		ELSE BEGIN
		
			UPDATE	CharacterItem 
			SET		Cnt = Cnt - @GambleCount 
			WHERE	CIID = @CIID
			AND		CID = @CID
			AND		Cnt - @GambleCount > 0
			
			IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
				ROLLBACK TRAN
				SELECT -2 AS 'Ret'
				RETURN;
			END		
			
		END
		
	COMMIT TRAN ----------		
		
	SELECT 0 AS 'Ret'
END
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spUpdateCharGambleItemCount
*/