USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®

CREATE PROC dbo.spDeleteExpiredCharacterItem
-- ALTER PROC dbo.spDeleteExpiredCharacterItem
	@CID	INT,
	@CIID	INT
AS BEGIN

	SET NOCOUNT ON;

	DECLARE @ItemID		INT;
	DECLARE @ItemCnt	INT;
	DECLARE @DateDiff	INT;
		
	SELECT	@ItemID = ItemID, @ItemCnt = ISNULL(Cnt, 1)
			, @DateDiff = (RentHourPeriod * 60) - (DateDiff(n, RentDate, GETDATE()))
	FROM	CharacterItem
	WHERE	CIID = @CIID
	AND		CID = @CID
	AND		CID IS NOT NULL;
	
	IF ( @ItemCnt < 1 ) BEGIN
		SELECT -1 AS 'Ret'
		RETURN;
	END
	
	IF ( @DateDiff > 0 ) BEGIN
		SELECT -2 AS 'Ret'
		RETURN;
	END
	
	
	UPDATE	CharacterItem 
	SET		CID = NULL 
	WHERE	CIID = @CIID
	AND		CID = @CID;
		
	IF (@@ERROR <> 0) OR (@@ROWCOUNT = 0) BEGIN
		SELECT -3 AS 'Ret'
		RETURN;
	END		
			
	SELECT 0 AS 'Ret'	
END
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ÇÒ Äõ¸®
/*
DROP PROC spDeleteExpiredCharacterItem
*/