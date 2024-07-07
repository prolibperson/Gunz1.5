USE GunzDB
GO

-------------------------------------------------------------------------------------------------------------------------
-- 작업할 쿼리

CREATE PROC dbo.spBringBackAccountGambleItem
-- ALTER PROC dbo.spBringBackAccountGambleItem
	@AID			INT,
	@CID			INT,
	@CIID			INT,
	@ItemCnt		INT	
AS BEGIN
	SET NOCOUNT ON
  	
  	-- 쿼리 실행 날짜 저장
	DECLARE @CurDate DATETIME;
	SET @CurDate = GETDATE();
	
	DECLARE @CharItemID				INT
	DECLARE @CharItemRentDate		DATETIME  
	DECLARE @CharItemRentHourPeriod SMALLINT  
	DECLARE @CharItemCnt			SMALLINT  	
				
	-- 옮기고자 하는 겜블 아이템의 인벤 정보 
	SELECT	@CharItemID = ItemID, @CharItemRentDate = RentDate
			, @CharItemRentHourPeriod = RentHourPeriod, @CharItemCnt = ISNULL(Cnt, 1)
	FROM	CharacterItem 
	WHERE	CIID = @CIID
	AND		CID IS NOT NULL;

	IF( @CharItemID IS NULL ) BEGIN
		SELECT -1 AS 'Ret';
		RETURN;
	END
	
	IF( @CharItemCnt < @ItemCnt ) BEGIN
		SELECT -2 AS 'Ret';
		RETURN;
	END	
	
	DECLARE @AIID INT;	
	
	SELECT	TOP 1 @AIID = AIID 
	FROM	AccountItem 
	WHERE	AID = @AID 
	AND		ItemID = @CharItemID;
	 
	BEGIN TRAN -------------
	
		IF( @CharItemCnt = @ItemCnt ) BEGIN
		
			UPDATE	CharacterItem 
			SET		CID = NULL
			WHERE	CIID = @CIID
			AND		CID = @CID;
			
			IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN  
				ROLLBACK TRAN;
				SELECT -3 AS 'Ret';
				RETURN;
			END
			
		END
		ELSE BEGIN
			
			UPDATE	CharacterItem 
			SET		Cnt = @CharItemCnt - @ItemCnt 
			WHERE	CIID = @CIID
			AND		CID = @CID
			AND		Cnt - @ItemCnt > 0;
			
			IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN  
				ROLLBACK TRAN;
				SELECT -4 AS 'Ret';
				RETURN;		
			END
			
		END
  
  -------------------------------------------------------------------------------------------  			
			
		IF( @AIID IS NULL ) BEGIN
		
			INSERT INTO AccountItem (AID, ItemID, RentDate, RentHourPeriod, Cnt)   
			VALUES (@AID, @CharItemID, @CharItemRentDate, @CharItemRentHourPeriod, @ItemCnt);
			
			SET @AIID = @@IDENTITY;
			
			IF( 0 <> @@ERROR ) BEGIN  
				ROLLBACK TRAN;
				SELECT -5 AS 'Ret';
				RETURN;		
			END	
			
		END
		ELSE BEGIN
						
			UPDATE	AccountItem 
			SET		Cnt = Cnt + @ItemCnt 
			WHERE	AIID = @AIID 
			AND		AID = @AID;
				
			IF( 0 <> @@ERROR ) BEGIN  
				ROLLBACK TRAN;
				SELECT -6 AS 'Ret';
				RETURN;		
			END					
		END
			
	COMMIT TRAN -----------  
	
	
	
	SELECT 0 AS 'Ret';
END
GO

-------------------------------------------------------------------------------------------------------------------------
-- 복구 쿼리
/*
DROP PROC spBringBackAccountGambleItem
*/