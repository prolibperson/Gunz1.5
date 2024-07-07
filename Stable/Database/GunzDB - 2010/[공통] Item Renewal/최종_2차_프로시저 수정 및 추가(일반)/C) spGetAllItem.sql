USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®

CREATE PROC dbo.spGetAllItem
-- ALTER PROC dbo.spGetAllItem
AS BEGIN

	SET NOCOUNT ON;
	
	SELECT	ItemID, ResSex, ResLevel, Slot, Weight, BountyPrice, Damage, Delay
			, Controllability, MaxBullet, Magazine, ReloadTime, HP, AP
			, IsCashItem, IsSpendableItem
	FROM	Item(NOLOCK)
	
END
GO


-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spBringBackAccountGambleItem
*/