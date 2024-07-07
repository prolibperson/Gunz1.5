USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spDeleteExpiredAccountItem', 'BackUp_spDeleteExpiredAccountItem';
GO

CREATE PROC dbo.spDeleteExpiredAccountItem
-- ALTER PROC dbo.spDeleteExpiredAccountItem
	@AID		INT
	, @AIID		INT
AS BEGIN
	
	SET NOCOUNT ON;

	DECLARE @ItemID		INT;
	DECLARE @ItemCnt	INT;
	DECLARE @DateDiff	INT;
		
	SELECT	@ItemID = ItemID, @ItemCnt = ISNULL(Cnt, 1),
			@DateDiff = (RentHourPeriod * 60) - (DateDiff(n, RentDate, GETDATE()))
	FROM	AccountItem
	WHERE	AIID = @AIID
	AND		AID = @AID;
	
	DELETE AccountItem WHERE AIID = @AIID
	
	SELECT 0 AS 'Ret'
END
GO


-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®

/*
DROP PROC spDeleteExpiredAccountItem
EXEC sp_rename 'BackUp_spDeleteExpiredAccountItem', 'spDeleteExpiredAccountItem';
*/
