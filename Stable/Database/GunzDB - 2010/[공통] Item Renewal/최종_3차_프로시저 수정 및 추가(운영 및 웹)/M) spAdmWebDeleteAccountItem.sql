USE GunzDB
GO
-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spAdmWebDeleteAccountItem', 'BackUp_spAdmWebDeleteAccountItem';
GO

CREATE PROC dbo.spAdmWebDeleteAccountItem
-- ALTER PROC dbo.spAdmWebDeleteAccountItem
	@AID		INT
	, @AIID		INT
	, @ItemID	INT
	, @GMID		VARCHAR(20)
	, @Ret		INT OUTPUT    
AS BEGIN
	SET NOCOUNT ON;   

	IF NOT EXISTS ( SELECT AID FROM Account(NOLOCK) WHERE AID = @AID) BEGIN    
		SET @Ret = 0;
		RETURN @Ret;
	END
		
	DELETE	AccountItem 
	WHERE	AIID = @AIID 
	AND		AID = @AID 
	AND		ItemID = @ItemID
		
	IF (0 <> @@ERROR OR 0 = @@ROWCOUNT) BEGIN
		SET @Ret = 0;
		RETURN @Ret;
	END
	
	SET @Ret = 1;
	RETURN @Ret;
END
  
-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spAdmWebDeleteAccountItem
EXEC sp_rename 'BackUp_spAdmWebDeleteAccountItem', 'spAdmWebDeleteAccountItem';
*/

