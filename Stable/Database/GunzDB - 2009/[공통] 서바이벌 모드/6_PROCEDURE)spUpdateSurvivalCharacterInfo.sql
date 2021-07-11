USE GunzDB
GO

ALTER PROCEDURE dbo.spUpdateSurvivalCharacterInfo
	@nCID			INT,
	@nSID			TINYINT,
	@nRP			INT
AS BEGIN	
	SET NOCOUNT ON;
	
	UPDATE	dbo.SurvivalCharacterInfo
	SET		RP = CASE WHEN @nRP >= RP THEN @nRP ELSE RP END, RP_LatestTime = GETDATE()
	WHERE	SID = @nSID 
	AND		CID = @nCID

	IF (0 = @@ROWCOUNT)
	BEGIN
		INSERT INTO dbo.SurvivalCharacterInfo(SID, CID, RP, RP_LatestTime, DeleteFlag) 
		VALUES(@nSID, @nCID, @nRP, GETDATE(), 0)
	END	
END
GO