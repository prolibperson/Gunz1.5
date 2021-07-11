USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spAdmWebInsertCharacterItem', 'BackUp_spAdmWebInsertCharacterItem';
GO

CREATE PROC dbo.spAdmWebInsertCharacterItem  
-- ALTER PROC dbo.spAdmWebInsertCharacterItem  
	@CID		INT
	, @ItemID	INT
	, @Period	SMALLINT
	, @GMID		VARCHAR(20)  
	, @Ret		INT OUTPUT
AS BEGIN 

	-----------------------------------------------------------------------------	-----------
 
	IF (500000 < @ItemID) OR ((@Period IS NOT NULL) AND (0 > @Period)) BEGIN  
		SET @Ret = 0;
		RETURN @Ret;
	END  

	IF NOT EXISTS( SELECT CID FROM Character(NOLOCK) WHERE CID = @CID ) BEGIN  
		SET @Ret = 0;
		RETURN @Ret;
	END
	
	----------------------------------------------------------------------------------------

	DECLARE @RentHourPeriod SMALLINT;

	IF (0 = @Period) OR (@Period IS NULL) BEGIN
		SELECT @RentHourPeriod = 0;
	END
	ELSE BEGIN  
		SELECT @RentHourPeriod = @Period;
	END
	
	----------------------------------------------------------------------------------------

	INSERT INTO CharacterItem( CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt )
	VALUES (@CID, @ItemID, GETDATE(), GETDATE(), @RentHourPeriod, 1 )
	
	IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
		SET @Ret = 0  
		RETURN @Ret
	END 

	----------------------------------------------------------------------------------------

	SET @Ret = 1  
	RETURN @Ret  
END 
-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spAdmWebInsertCharacterItem
EXEC sp_rename 'BackUp_spAdmWebInsertCharacterItem', 'spAdmWebInsertCharacterItem';
*/

