USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spBringBackAccountItem', 'BackUp_spBringBackAccountItem';
GO


CREATE PROC dbo.spBringBackAccountItem
-- ALTER PROC dbo.spBringBackAccountItem
	@AID			INT,
	@CID			INT,
	@CIID			INT
AS BEGIN
	SET NOCOUNT ON
  	
	DECLARE @ItemID			INT;
	DECLARE @RentDate		DATETIME; 
	DECLARE @RentHourPeriod SMALLINT;
	
	DECLARE @CurDate DATETIME;
	SET @CurDate = GETDATE();
			
	SELECT	@ItemID = ItemID, @RentDate = RentDate
			, @RentHourPeriod = RentHourPeriod
	FROM	CharacterItem 
	WHERE	CIID = @CIID
	AND		CID = @CID
	AND		CID IS NOT NULL;

	IF( (@ItemID IS NULL) AND (@ItemID < 400000) ) BEGIN
		SELECT -1 AS 'Ret';
		RETURN;
	END	
	
	IF( EXISTS(SELECT CID FROM CharacterEquipmentSlot WHERE CIID = @CIID AND CID = @CID) ) BEGIN
		SELECT -2 AS 'Ret';
		RETURN;
	END
	 
	BEGIN TRAN -------------
	
		UPDATE	CharacterItem 
		SET		CID = NULL
		WHERE	CIID = @CIID
		AND		CID = @CID;
		
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN  
			ROLLBACK TRAN;
			SELECT -3 AS 'Ret';
			RETURN;
		END
		
		INSERT INTO AccountItem (AID, ItemID, RentDate, RentHourPeriod, Cnt)   
		VALUES (@AID, @ItemID, @RentDate, @RentHourPeriod, 1);
		
		DECLARE @AIID INT;
		SET @AIID = @@IDENTITY;
			
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN  
			ROLLBACK TRAN;
			SELECT -4 AS 'Ret';
			RETURN;		
		END	
		
	COMMIT TRAN -----------  
	
	SELECT 0 AS 'Ret';
END
GO

-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spBringBackAccountItem
EXEC sp_rename 'BackUp_spBringBackAccountItem', 'spBringBackAccountItem';
*/