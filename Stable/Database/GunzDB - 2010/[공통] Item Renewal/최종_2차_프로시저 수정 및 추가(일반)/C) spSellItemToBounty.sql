USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- 작업할 쿼리
CREATE PROC dbo.spSellItemToBounty
-- ALTER PROC dbo.spSellItemToBounty
	@CID		INT,  
	@CIID		INT,
	@ItemID		INT,
	@Price		INT,  
	@CharBP		INT  
AS BEGIN

	SET NOCOUNT ON;
	
	BEGIN TRAN -------------------
	
		-- Bounty 증가  
		UPDATE dbo.Character SET BP = BP + @Price WHERE CID = @CID
		
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN  
			ROLLBACK
			SELECT -1 AS Ret;
			RETURN;
		END  


		UPDATE	dbo.CharacterItem
		SET		CID = NULL
		WHERE	CIID = @CIID
		AND		CID = @CID;
		
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN  
			ROLLBACK
			SELECT -2 AS Ret;
			RETURN;
		END		
		
		-- Item 판매 로그 추가  
		INSERT INTO dbo.ItemPurchaseLogByBounty(ItemID, CID, Date, Bounty, CharBounty, Type)
		VALUES (@ItemID, @CID, GETDATE(), @Price, @CharBP, '판매')
		
		IF( @@ERROR <> 0 OR @@ROWCOUNT = 0 ) BEGIN  
			ROLLBACK
			SELECT -3 AS Ret;
			RETURN;
		END		
		
	COMMIT TRAN ------------------- 
			
	SELECT 0 AS Ret
END
GO


----------------------------------------------------------------------------------------------------------------------------------------
-- 복구 쿼리
/*
DROP PROC spSellItemToBounty
*/