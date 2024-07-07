USE GunzDB
GO


CREATE PROC dbo.spInsertAccountPenaltyInfo
-- ALTER PROC dbo.spInsertAccountPenaltyInfo
	@AID			INT	
	, @PCode		TINYINT
	, @PHour		INT
	, @GM_ID		VARCHAR(24)
AS BEGIN
	SET NOCOUNT ON	
	
	BEGIN TRAN -----------------
	
		UPDATE	AccountPenaltyGMLog
		SET		Reset_Date = GETDATE(), Reset_GM_TypeID = 2, Reset_GM_ID = @GM_ID
		WHERE	AID = @AID
		AND		PCode = @PCode
		AND		Reset_Date IS NULL

		INSERT AccountPenaltyGMLog(AID, PEndDate, PCode, Set_GM_TypeID, Set_GM_ID, Set_Date)
		VALUES (@AID, DATEADD(hh, @PHour, GETDATE()), @PCode, 2, @GM_ID, GETDATE())		
		
	COMMIT TRAN ----------------		
END
GO