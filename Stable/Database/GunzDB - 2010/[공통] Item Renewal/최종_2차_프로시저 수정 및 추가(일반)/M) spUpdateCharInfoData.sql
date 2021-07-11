USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spUpdateCharInfoData', 'BackUp_spUpdateCharInfoData';
GO

CREATE PROC dbo.spUpdateCharInfoData
-- ALTER PROC dbo.spUpdateCharInfoData	
	@CID			INT,
	@XPInc			INT,
	@BPInc			INT,
	@KillInc		INT,  
	@DeathInc		INT,
	@PlayTimeInc	INT
AS BEGIN
	SET NOCOUNT ON;
	    
	UPDATE	dbo.Character   
	SET		XP = XP + (@XPInc)
			, BP = BP + (@BPInc)
			, KillCount = KillCount + (@KillInc)
			, DeathCount = DeathCount + (@DeathInc)
			, PlayTime = PlayTime + (@PlayTimeInc)
	WHERE	CID = @CID;
END

-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spUpdateCharInfoData
EXEC sp_rename 'BackUp_spUpdateCharInfoData', 'spUpdateCharInfoData';
*/