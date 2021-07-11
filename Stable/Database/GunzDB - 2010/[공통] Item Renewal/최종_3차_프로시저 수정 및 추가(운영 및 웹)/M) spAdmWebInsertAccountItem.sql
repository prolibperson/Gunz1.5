USE GunzDB
GO
-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spAdmWebInsertAccountItem', 'BackUp_spAdmWebInsertAccountItem';
GO
  
CREATE PROC dbo.spAdmWebInsertAccountItem
-- ALTER PROC dbo.spAdmWebInsertAccountItem
	@AID		INT
	, @ItemID	INT
	, @Period	INT
	, @GMID		VARCHAR(20)
	, @Ret		INT OUTPUT
AS BEGIN
	
	SET NOCOUNT ON;
	
	IF (500001 > @ItemID) OR ((@Period IS NOT NULL) AND (0 > @Period)) BEGIN
		SET @Ret = 0;
		RETURN @Ret;
	END  

	IF NOT EXISTS ( SELECT AID FROM Account(NOLOCK) WHERE AID = @AID ) BEGIN  
		SET @Ret = 0;
		RETURN @Ret;
	END  

	------------------------------------------------------------------------------------

	DECLARE @RentHourPeriod		INT;

	IF (0 = @Period) OR (@Period IS NULL)	SELECT @RentHourPeriod = 0;
	ELSE									SELECT @RentHourPeriod = @Period;

	INSERT INTO AccountItem( AID, ItemID, RentDate, RentHourPeriod, Cnt)  
	VALUES (@AID, @ItemID, GETDATE(), @RentHourPeriod, 1 )
	
	IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
		SET @Ret = 0;
		RETURN @Ret;
	END  
	
	------------------------------------------------------------------------------------
	
	SET @Ret = 1;
	RETURN @Ret;
END

-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spAdmWebInsertAccountItem
EXEC sp_rename 'BackUp_spAdmWebInsertAccountItem', 'spAdmWebInsertAccountItem';
*/

