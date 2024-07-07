USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- 작업할 쿼리
EXEC sp_rename 'spBuyBountyItem', 'BackUp_spBuyBountyItem'
GO

CREATE PROC dbo.spBuyBountyItem
-- ALTER PROC dbo.spBuyBountyItem
	@CID				INT,  
	@ItemID				INT,
	@ItemCount			INT,
	@Price				INT,
	@IsSpendableItem	INT,
	@RentHourPeriod		INT = NULL	
AS BEGIN
	SET NOCOUNT ON
	
	DECLARE @Bounty INT;
	DECLARE @OrderCIID INT;
	DECLARE @Cnt INT;
	
	IF( @RentHourPeriod IS NULL ) BEGIN
		SET @RentHourPeriod = 0;
	END
	
	DECLARE @CurDate DATETIME;
	SET @CurDate = GETDATE();

	BEGIN TRAN ----------------------------	
	
		-- 잔액검사 => Bounty 감소      
		UPDATE	dbo.Character 
		SET		BP = BP - @Price 
		WHERE	CID = @CID 
		AND		(BP - @Price >= 0);
		
		IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
			ROLLBACK TRAN
			SELECT -1 AS 'Ret'
			RETURN;
		END      		
		
		IF( @IsSpendableItem = 1 ) BEGIN
					
			-- 이미 갖고 있는지 확인해본다.
			SELECT	@OrderCIID = CIID 
			FROM	CharacterItem(NOLOCK) 
			WHERE	CID = @CID 
			AND		ItemID = @ItemID;
		
			-- 이미 갖고 있지 않다면 새로 추가해준다.
			IF( @OrderCIID IS NOT NULL ) BEGIN
			
				UPDATE	dbo.CharacterItem				-- Item 추가
				SET		Cnt = Cnt + @ItemCount
				WHERE	CIID = @OrderCIID
				AND		CID = @CID;
				
				IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
					ROLLBACK TRAN
					SELECT -2 AS 'Ret'
					RETURN;
				END
										
			END ELSE BEGIN
			
				INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
				Values (@CID, @ItemID, @CurDate, @CurDate, @RentHourPeriod, @ItemCount)
				
				IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
					ROLLBACK TRAN
					SELECT -3 AS 'Ret'
					RETURN;
				END
				
				SELECT @OrderCIID = @@IDENTITY;	
			END
						
		END
		ELSE BEGIN
				
			INSERT dbo.CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
			Values (@CID, @ItemID, @CurDate, @CurDate, @RentHourPeriod, @ItemCount)
			
			SELECT @OrderCIID = @@IDENTITY;	
			
			IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
				ROLLBACK TRAN
				SELECT -4 AS 'Ret'
				RETURN;
			END
						
		END 
		
		-- Item 구매로그 추가      
		INSERT INTO ItemPurchaseLogByBounty (ItemID, CID, Date, Bounty, CharBounty, Type)
		VALUES (@ItemID, @CID, @CurDate, @Price, @Bounty, '구입')
		
		IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
			ROLLBACK TRAN
			SELECT -5 AS 'Ret'
			RETURN;
		END
		
	COMMIT TRAN ----------------------------
		
	SELECT 0 AS 'Ret', @OrderCIID AS 'ORDERCIID'
END


----------------------------------------------------------------------------------------------------------------------------------------
-- 복구 쿼리
/*
DROP PROC spBuyBountyItem
EXEC sp_rename 'BackUp_spBuyBountyItem', 'spBuyBountyItem'
*/
