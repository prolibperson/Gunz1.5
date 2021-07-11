ALTER PROCEDURE dbo.spRemoveAccount_Netmarble
	@AID		INT
AS BEGIN
	SET NOCOUNT ON;
	
	DECLARE @SelUserID	VARCHAR(20);
	DECLARE @SelUserCN  VARCHAR(20);	
	DECLARE @DeleteID	VARCHAR(20);	
	
	-- DELETE Step 1 - 이상한 ID값 만들기
	SELECT @DeleteID = CAST( CAST(NEWID() AS VARCHAR(40)) AS VARCHAR(20));
	
	-- DELETE Step 2 - 이상한 ID값이 고유한지 판단하기
	IF( EXISTS (SELECT * FROM dbo.Login(NOLOCK) WHERE UserID = @DeleteID) ) BEGIN
		INSERT dbo.AccountDeleteFailLog VALUES(@AID, GETDATE(), 1);
		RETURN 1;
	END

	BEGIN TRAN -----------------
		
		-- DELETE Step 3 - 이상한 ID값으로 Account, Login에 있는 UserID Field를 바꿔주기
		UPDATE dbo.Account SET UserID = @DeleteID, DeleteFlag = 1 WHERE AID = @AID;
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
			ROLLBACK TRAN		
			INSERT dbo.AccountDeleteFailLog VALUES(@AID, GETDATE(), 2);
			RETURN 2;
		END
		
		UPDATE dbo.Login SET UserID = @DeleteID, Password = '' WHERE AID = @AID;
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
			ROLLBACK TRAN		
			INSERT dbo.AccountDeleteFailLog VALUES(@AID, GETDATE(), 3);
			RETURN 3;
		END
		
		-- DELETE Step 4 - AccountDeleteLog 추가!	
		SELECT	@SelUserID = l.UserID, @SelUserCN = l.Password
		FROM	dbo.Login l(NOLOCK) 
		WHERE	l.AID = @AID
			
		INSERT INTO dbo.AccountDeleteLog VALUES(@AID, @SelUserID, @SelUserCN, @DeleteID, GETDATE());
		IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
			ROLLBACK TRAN		
			INSERT dbo.AccountDeleteFailLog VALUES(@AID, GETDATE(), 4);
			RETURN 4;
		END
	
	COMMIT TRAN -----------------
	
	RETURN 0;
END