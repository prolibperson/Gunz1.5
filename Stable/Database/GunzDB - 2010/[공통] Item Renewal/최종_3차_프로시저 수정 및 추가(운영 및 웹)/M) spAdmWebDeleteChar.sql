USE GunzDB
GO
-------------------------------------------------------------------------------------------------------------------------
-- 작업할 쿼리
EXEC sp_rename 'spAdmWebDeleteChar', 'BackUp_spWebChangeClanName_Netmarble';
GO

CREATE PROC dbo.spAdmWebDeleteChar  
-- ALTER PROC dbo.spAdmWebDeleteChar  
	@AID			INT
	, @CharNum		SMALLINT
	, @CharName		VARCHAR(24)  
	, @GMID			VARCHAR(20)  
	, @Ret			INT OUTPUT   
AS BEGIN

	SET NOCOUNT ON   

	DECLARE @CID		INT
	DECLARE @ErrSlot	INT
	DECLARE @ErrDelInfo INT
	DECLARE @ErrName	INT

	SELECT	@CID = CID 
	FROM	Character(NOLOCK)   
	WHERE	AID = @AID 
	AND		Name = @CharName 
	AND		CharNum = @CharNum
	
	IF (@CID IS NULL) BEGIN
		SET @Ret = 0  
		RETURN @Ret
	END  

	BEGIN TRAN ----------------			
				
		-- 캐쉬아이템은 중앙은행으로 돌려줘야 함.  
		INSERT INTO AccountItem( AID, ItemID, RentDate, RentHourPeriod, Cnt )  
			SELECT	@AID AS AID, ItemID, RentDate, RentHourPeriod, Cnt  
			FROM	CharacterItem(NOLOCK)  
			WHERE	CID = @CID 
			AND		ItemID > 499999
			
		IF (0 <> @@ERROR) BEGIN
			ROLLBACK TRAN  
			SET @Ret = 0  
			RETURN @Ret
		END  


		UPDATE	CharacterItem
		SET		CID = NULL
		WHERE	CID = @CID;
		
		IF (0 <> @@ERROR) BEGIN
			ROLLBACK TRAN
			SET @Ret = 0
			RETURN @Ret
		END  


		UPDATE	CharacterEquipmentSlot
		SET		CIID = NULL, ItemID = NULL
		WHERE	CID = @CID
		
		SET @ErrSlot = @@ROWCOUNT  
		

		UPDATE	Character 
		SET		DeleteName = Name, DeleteFlag = 1 
		WHERE	CID = @CID
		
		SET @ErrDelInfo = @@ROWCOUNT  


		UPDATE	Character 
		SET		Name = '' 
		WHERE	CID = @CID
		
		SET @ErrName = @@ROWCOUNT  

		IF (0 = @ErrSlot) OR (0 = @ErrDelInfo) OR (0 = @ErrName) BEGIN  
		ROLLBACK TRAN  
		SET @Ret = 0  
		RETURN @Ret  
		END
		
	COMMIT TRAN --------------- 

	SET @Ret = 1  
	RETURN @Ret;
END
  
-------------------------------------------------------------------------------------------------------------------------
-- 복구 쿼리
/*
DROP PROC spWebChangeClanName_Netmarble
EXEC sp_rename 'BackUp_spWebChangeClanName_Netmarble', 'spWebChangeClanName_Netmarble';
*/

