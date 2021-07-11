USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- 작업할 쿼리
EXEC sp_rename 'spInsertChar', 'BackUp_spInsertChar'
GO

CREATE PROC dbo.spInsertChar
-- ALTER PROC dbo.spInsertChar
	@AID		INT,
	@CharNum	SMALLINT,
	@Name		VARCHAR(24),
	@Sex		TINYINT,
	@Hair		INT,
	@Face		INT,
	@Costume	INT
AS BEGIN

	SET NOCOUNT ON;
	
	IF EXISTS(SELECT CID FROM dbo.Character where (AID=@AID AND CharNum=@CharNum) OR (Name=@Name)) BEGIN   
		SELECT -1 AS 'Ret';
		RETURN;
	END  

	DECLARE @CharIdent		INT;
	DECLARE @ChestCIID		INT;
	DECLARE @LegsCIID		INT;
	DECLARE @MeleeCIID		INT;
	DECLARE @PrimaryCIID	INT;
	DECLARE @SecondaryCIID	INT; 
	DECLARE @Custom1CIID	INT;
	DECLARE @Custom2CIID	INT;
	
	DECLARE @ChestItemID		INT;
	DECLARE @LegsItemID			INT;
	DECLARE @MeleeItemID		INT;
	DECLARE @PrimaryItemID		INT;
	DECLARE @SecondaryItemID	INT;
	DECLARE @Custom1ItemID		INT;
	DECLARE @Custom2ItemID		INT;
	
	DECLARE @NowDate DATETIME;
	SET @NowDate = GETDATE();

	SET @SecondaryCIID = NULL  
	SET @SecondaryItemID = NULL  

	SET @Custom1CIID = NULL  
	SET @Custom1ItemID = NULL  

	SET @Custom2CIID = NULL  
	SET @Custom2ItemID = NULL  

	BEGIN TRAN -----------
	
		INSERT INTO dbo.Character(AID, Name, CharNum, Level, Sex, Hair, Face, XP, BP, FR, CR, ER, WR,
									 GameCount, KillCount, DeathCount, RegDate, PlayTime, DeleteFlag)  
		Values(@AID, @Name, @CharNum, 1, @Sex, @Hair, @Face, 0, 0, 0, 0, 0, 0, 0, 0, 0, @NowDate, 0, 0)
		
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
			ROLLBACK TRAN
			SELECT -2 AS 'Ret';
			RETURN;
		END
			

		SET @CharIdent = @@IDENTITY  


		/* Melee */  
		SET @MeleeItemID =   
			CASE @Costume  
				WHEN 0 THEN 1  
				WHEN 1 THEN 2  
				WHEN 2 THEN 1  
				WHEN 3 THEN 2  
				WHEN 4 THEN 2  
				WHEN 5 THEN 1  
			END  

		INSERT INTO dbo.CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
		VALUES (@CharIdent, @MeleeItemID, @NowDate, @NowDate, 0, 1)
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
			ROLLBACK TRAN
			SELECT -3 AS 'Ret';
			RETURN;
		END
		
		SET @MeleeCIID = @@IDENTITY
				
		/* Primary */  
		SET @PrimaryItemID =   
			CASE @Costume  
				WHEN 0 THEN 5001  
				WHEN 1 THEN 5002  
				WHEN 2 THEN 4005  
				WHEN 3 THEN 4001  
				WHEN 4 THEN 4002  
				WHEN 5 THEN 4006  
			END  

		INSERT INTO dbo.CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
		VALUES (@CharIdent, @PrimaryItemID, @NowDate, @NowDate, 0, 1)
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
			ROLLBACK TRAN
			SELECT -5 AS 'Ret';
			RETURN;
		END
	
		SET @PrimaryCIID = @@IDENTITY;			
		
				
		/* Secondary */  
		IF( @Costume = 0 OR @Costume = 2 ) BEGIN  
			SET @SecondaryItemID =  
				CASE @Costume  
					WHEN 0 THEN 4001  
					WHEN 1 THEN 0  
					WHEN 2 THEN 5001  
					WHEN 3 THEN 4006  
					WHEN 4 THEN 0  
					WHEN 5 THEN 4006  
				END  

			IF( @SecondaryItemID <> 0 ) BEGIN  
				INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
				VALUES (@CharIdent, @SecondaryItemID, @NowDate, @NowDate, 0, 1)
				IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
					ROLLBACK TRAN
					SELECT -7 AS 'Ret';
					RETURN;
				END
				
				SET @SecondaryCIID = @@IDENTITY;								
			END  
		END 
	 
	
		/* Custom1 */ 
		SET @Custom1ItemID =   
			CASE @Costume  
				WHEN 0 THEN 30301
				WHEN 1 THEN 30301
				WHEN 2 THEN 30401
				WHEN 3 THEN 30401
				WHEN 4 THEN 30401
				WHEN 5 THEN 30101
			END
				 	
		INSERT INTO dbo.CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
		VALUES (@CharIdent, @Custom1ItemID, @NowDate, @NowDate, 0, 1)
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
			ROLLBACK TRAN
			SELECT -9 AS 'Ret';
			RETURN;
		END
							
		SET @Custom1CIID = @@IDENTITY
		
		/* Custom2 */  
		IF( @Costume = 4 OR @Costume = 5 ) BEGIN  
			SET @Custom2ItemID =  
				CASE @Costume  
					WHEN 0 THEN 0
					WHEN 1 THEN 0
					WHEN 2 THEN 0
					WHEN 3 THEN 0
					WHEN 4 THEN 30001
					WHEN 5 THEN 30001
				END

			IF( @Custom2ItemID <> 0 ) BEGIN  
				INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
				VALUES (@CharIdent, @Custom2ItemID, @NowDate, @NowDate, 0, 1)
				IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
					SELECT -11 AS 'Ret';
					RETURN;
				END
				
				SET @Custom2CIID = @@IDENTITY;				
				
			END  
		END  

		/* 남자일 경우 */
		IF( @Sex = 0 ) BEGIN  
			
			/* Chest */  
			SET @ChestItemID =  
				CASE @Costume  
					WHEN 0 THEN 21001  
					WHEN 1 THEN 21001  
					WHEN 2 THEN 21001  
					WHEN 3 THEN 21001  
					WHEN 4 THEN 21001  
					WHEN 5 THEN 21001  
				END

			INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
			VALUES (@CharIdent, @ChestItemID, @NowDate, @NowDate, 0, 1)
			IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
				ROLLBACK TRAN
				SELECT -13 AS 'Ret';
				RETURN;
			END
			
			SET @ChestCIID = @@IDENTITY
			
			
			/* Legs */  
			SET @LegsItemID =  
				CASE @Costume  
					WHEN 0 THEN 23001  
					WHEN 1 THEN 23001  
					WHEN 2 THEN 23001  
					WHEN 3 THEN 23001  
					WHEN 4 THEN 23001  
					WHEN 5 THEN 23001  
				END  

			INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
			VALUES (@CharIdent, @LegsItemID, @NowDate, @NowDate, 0, 1)
			IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
				ROLLBACK TRAN
				SELECT -15 AS 'Ret';
				RETURN;
			END
			
			SET @LegsCIID = @@IDENTITY;
						
		END 
		/* 여자일 경우 */  
		ELSE BEGIN   
			
			/* Chest */  
			SET @ChestItemID =  
				CASE @Costume  
					WHEN 0 THEN 21501  
					WHEN 1 THEN 21501  
					WHEN 2 THEN 21501  
					WHEN 3 THEN 21501  
					WHEN 4 THEN 21501  
					WHEN 5 THEN 21501  
				END 

			INSERT INTO dbo.CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
			VALUES (@CharIdent, @ChestItemID, @NowDate, @NowDate, 0, 1)
			IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
				ROLLBACK TRAN
				SELECT -17 AS 'Ret';
				RETURN;
			END
			
			SET @ChestCIID = @@IDENTITY  
			
			
			/* Legs */  
			SET @LegsItemID =  
				CASE @Costume  
					WHEN 0 THEN 23501  
					WHEN 1 THEN 23501  
					WHEN 2 THEN 23501  
					WHEN 3 THEN 23501  
					WHEN 4 THEN 23501  
					WHEN 5 THEN 23501  
				END  

			INSERT INTO dbo.CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) 
			VALUES (@CharIdent, @LegsItemID, @NowDate, @NowDate, 0, 1)
			IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN
				ROLLBACK TRAN
				SELECT -19 AS 'Ret';
				RETURN;
			END
			
			SET @LegsCIID = @@IDENTITY;
		END
		
	COMMIT TRAN -----------
		
	INSERT dbo.CharacterEquipmentSlot(CID, SlotID)
		SELECT	@CharIdent, SlotID
		FROM	dbo.CharacterEquipmentSlotCode(NOLOCK)
			
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @ChestCIID, ItemID = @ChestItemID 
	WHERE	CID = @CharIdent AND SlotID = 1;
	
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @LegsCIID, ItemID = @LegsItemID 
	WHERE	CID = @CharIdent AND SlotID = 3;
	
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @MeleeCIID, ItemID = @MeleeItemID 
	WHERE	CID = @CharIdent AND SlotID = 7;
	
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @PrimaryCIID, ItemID = @PrimaryItemID 
	WHERE	CID = @CharIdent AND SlotID = 8;
	
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @SecondaryCIID, ItemID = @SecondaryItemID 
	WHERE	CID = @CharIdent AND SlotID = 9;
	
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @Custom1CIID, ItemID = @Custom1ItemID 
	WHERE	CID = @CharIdent AND SlotID = 10;
	
	UPDATE	dbo.CharacterEquipmentSlot 
	SET		CIID = @Custom2CIID, ItemID = @Custom2ItemID 
	WHERE	CID = @CharIdent AND SlotID = 11;
	
	SELECT 0 AS 'Ret'
END
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- 복구 쿼리
/*
DROP PROC spInsertChar
EXEC sp_rename 'BackUp_spInsertChar', 'spInsertChar'
*/

