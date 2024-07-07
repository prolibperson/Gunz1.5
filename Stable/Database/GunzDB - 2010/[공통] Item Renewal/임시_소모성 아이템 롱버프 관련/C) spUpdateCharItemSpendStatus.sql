-- USE GunzDB
USE IGunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®

CREATE PROC dbo.spUpdateCharacterBuffStatus
-- ALTER PROC dbo.spUpdateCharacterBuffStatus
	@CBID				INT
	, @ItemID			INT
	, @BuffMinutePeriod	INT
	, @StartPlayTime	INT
AS BEGIN
	SET NOCOUNT ON;
		
	DECLARE @CID		INT;
		
	SELECT	@CID = CID
	FROM	CharacterBuffStatus(NOLOCK)
	WHERE	CBID = @CBID;
	
	IF( @CID IS NOT NULL ) BEGIN
		IF( @ItemID != 0 ) BEGIN						
			UPDATE	CharacterBuffStatus
			SET		ItemID = @ItemID, BuffMinutePeriod = @BuffMinutePeriod
					, StartPlayTime = @StartPlayTime, StartDate = GETDATE()
			WHERE	CBID = @CBID;
			
			SELECT 1 AS Ret;
			RETURN;
		END
		ELSE BEGIN			
			UPDATE	CharacterBuffStatus
			SET		ItemID = NULL, BuffMinutePeriod = NULL
					, StartPlayTime = NULL, StartDate = NULL
			WHERE	CBID = @CBID;
			
			SELECT 1 AS Ret;
			RETURN;
		END		
	END
	
	SELECT 0 AS Ret;
	RETURN;			
END
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC dbo.spUpdateCharacterBuffStatus
*/
SELECT PlayTime FROM Character WHERE CID =1271
EXEC spGetCharacterBuffStatus 1271
SELECT * FROM CharacterBuffStatus

EXEC spUpdateCharacterBuffStatus 1, 40000, 5, 471147
EXEC spUpdateCharacterBuffStatus 2, 40001, 55, 0
EXEC spUpdateCharacterBuffStatus 3, 40002, 15, 0
EXEC spUpdateCharacterBuffStatus 4, 40003, 1, 0
EXEC spUpdateCharacterBuffStatus 5, 0, 0, 0

exec sp_help spDTUpdateDTGameLog
