USE GunzDB
GO
-------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spWebChangeClanName', 'BackUp_spWebChangeClanName';
GO

CREATE PROC dbo.spWebChangeClanName
-- ALTER PROC dbo.spWebChangeClanName
	@MasterName		VARCHAR(24)  
	, @NewClanName	VARCHAR(28)  
AS BEGIN
	
	SET NOCOUNT ON;
	
	------------------------------------------------------------------------
	
	DECLARE @CID INT;
	DECLARE @Sex TINYINT;
		
	SELECT @CID = CID, @Sex = Sex FROM dbo.Character(NOLOCK) WHERE Name = @MasterName;
	
	IF( @CID IS NULL) or ((0 <> @Sex) and (1 <> @Sex)) RETURN -1;
	
	------------------------------------------------------------------------
	
	DECLARE @CLID			INT;
	DECLARE @OldClanName	VARCHAR(28); 
	
	SELECT	@CLID = CLID, @OldClanName = Name  
	FROM	dbo.Clan(NOLOCK)
	WHERE	MasterCID = @CID AND 1 <> DeleteFlag  
	
	IF( @CLID IS NULL ) RETURN -2;
	
	------------------------------------------------------------------------
	DECLARE @NowDate		DATETIME;	
	DECLARE @ClanCoatItemID INT;
	DECLARE @OrderCIID		INT;  
    
    SET @NowDate = GETDATE();
    
    IF(0 = @Sex)		SET @ClanCoatItemID = 21011;
    ELSE IF(1 = @Sex )	SET @ClanCoatItemID = 21511;
    ELSE				RETURN -3;


		  
	BEGIN TRAN ------------

		UPDATE	dbo.Clan  
		SET		Name = @NewClanName  
		WHERE	CLID = @CLID  
		
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
			ROLLBACK TRAN;
			RETURN -4;
		END
		

		INSERT dbo.CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod)  
		VALUES(@CID, @ClanCoatItemID, @NowDate, @NowDate, 2160 )  
		
		SET @OrderCIID = @@IDENTITY;
		
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
			ROLLBACK TRAN;
			RETURN -5;
		END


		INSERT LogDB.dbo.ChangeClanNameLog(CLID, OldName, NewName, MasterCID, MasterName, RegDate)  
		VALUES(@CLID, @OldClanName, @NewClanName, @CID, @MasterName, @NowDate);
		
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
			ROLLBACK TRAN;
			RETURN -6;
		END

	COMMIT TRAN ------------
  
	RETURN 1;
END
  
-------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spWebChangeClanName
EXEC sp_rename 'BackUp_spWebChangeClanName', 'spWebChangeClanName';
*/

