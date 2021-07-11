USE GunzDB
GO

ALTER PROC dbo.spWebRestoreSleepCharacter_NHN
	@AID		INT,
	@CID		INT,
	@CharNum	SMALLINT,
	@NewName	VARCHAR(24)	
AS BEGIN
	SET NOCOUNT ON;
			
	DECLARE @RegDate			DATETIME;
	DECLARE @OriginName			VARCHAR(24);
	
	SELECT	@OriginName = Name, @RegDate = RegDt 
	FROM	SleepCharacterNHN(NOLOCK)
	WHERE	CID = @CID;
	
	-- Not Exists Information In SleepCharacterNHN
	IF( @RegDate IS NULL ) RETURN -1;
	
	-- Can't Use CharNum
	IF( @CharNum < 0 OR @CharNum > 3 )	RETURN -2;
	
	-- Can't Use CharNum
	IF( EXISTS(SELECT CID FROM Character(NOLOCK) WHERE AID = @AID AND CharNum = @CharNum) ) RETURN -3;
	
	-- Not Restored Account
	IF( EXISTS(SELECT AID FROM SleepAccountNHN(NOLOCK) WHERE AID = @AID) ) RETURN -4;
	
	-- Duplicate Character Name Exists!		
	IF( EXISTS(SELECT CID FROM Character(NOLOCK) WHERE Name = @NewName) ) RETURN -5;
		
			
	BEGIN TRAN --------		
		
		UPDATE	Character
		SET		Name = @NewName, DeleteName = '', DeleteFlag = 0, CharNum = @CharNum
		WHERE	CID = @CID
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
			ROLLBACK TRAN     
			RETURN -6;		-- Restore Character Information FAIL!
		END
		
		DELETE	SleepCharacterNHN 
		WHERE	CID = @CID;
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
			ROLLBACK TRAN     
			RETURN -7;		-- Delete SleepCharacterNHN FAIL!
		END
		
		INSERT SleepCharacterRestoredLogNHN(CID, OrginName, RestoreName, RegDt, RestoredDt)
		VALUES (@CID, @OriginName, @NewName, @RegDate, GETDATE());
		IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN    
			ROLLBACK TRAN     
			RETURN -8;		-- Insert SleepCharacterRestoredLogNHN FAIL!
		END
		
	COMMIT TRAN -------	
	
	RETURN 0;
	
END
