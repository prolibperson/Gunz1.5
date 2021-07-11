-- USE GunzDB
USE IGunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®

CREATE PROC dbo.spInsertCharacterBuffStatus
-- ALTER PROC dbo.spInsertCharItemSpendStatus
	@CID	INT
AS BEGIN
	SET NOCOUNT ON;
	
	INSERT	CharacterBuffStatus(CID)
	VALUES	(@CID);
	
	SELECT	@@IDENTITY AS CBID;	
END
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC dbo.spInsertCharacterBuffStatus
*/

