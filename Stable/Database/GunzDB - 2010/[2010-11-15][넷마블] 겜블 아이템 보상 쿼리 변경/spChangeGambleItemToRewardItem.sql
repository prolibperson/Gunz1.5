CREATE PROC dbo.spChangeGambleItemToRewardItem
-- ALTER PROC dbo.spChangeGambleItemToRewardItem
	@CID			INT
	, @CIID			INT 
	, @GIID			INT
	, @RewardItemID INT
AS BEGIN
	SET NOCOUNT ON  
  
	DECLARE @RentHourPeriod smallint  
  
	SELECT	@RentHourPeriod = RentHourPeriod  
	FROM	GambleRewardItem gri(NOLOCK)  
	WHERE	GIID = @GIID 
	AND		(ItemIDMale = @RewardItemID OR ItemIDFemale = @RewardItemID)  
  
	IF (@RentHourPeriod IS NULL)
	BEGIN  
		SELECT -1 as 'Ret'  
		RETURN  
	END  
  
	BEGIN TRAN -----------
		IF( @RentHourPeriod = 0 ) BEGIN
			-- 무제한 아이템일 경우, RentDate가 NULL이 되도록 바꿔준다.
			UPDATE	CharacterItem  
			SET		ItemID = @RewardItemID, RentHourPeriod = @RentHourPeriod
			WHERE	CID = @CID 
			AND		CIID = @CIID 
			AND		ItemID = @GIID 
		END
		ELSE BEGIN
			UPDATE	CharacterItem  
			SET		ItemID = @RewardItemID, RentHourPeriod = @RentHourPeriod, RentDate = GETDATE()  
			WHERE	CID = @CID 
			AND		CIID = @CIID 
			AND		ItemID = @GIID 
		END
		 
		
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
		   ROLLBACK TRAN  
		   SELECT -2 AS 'Ret'  
		   RETURN  
		END  
  
		INSERT INTO LogDB..GambleLog(CID, GIID, RewardItemID, RegDate)  
		VALUES (@CID, @GIID, @RewardItemID, GETDATE())  
	  
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN  
			ROLLBACK TRAN;  
			SELECT -3 AS 'Ret'  
			RETURN  
		END  
	  
	COMMIT TRAN ---------
  
	SELECT 1 AS 'Ret'  
END  