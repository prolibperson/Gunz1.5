ALTER PROCEDURE dbo.spInsertAccount_Netmarble
	@UserID		VARCHAR(20)
	, @CN		VARCHAR(20)
	, @Cert		TINYINT
	, @Name		VARCHAR(128)
	, @Age		SMALLINT
	, @Sex		TINYINT
AS BEGIN
	SET NOCOUNT ON;

	DECLARE @SelAID		INT;	
	DECLARE @SelUserID	VARCHAR(20);
	DECLARE @SelUserCN  VARCHAR(20);
	
	-------------------------------------------------------------------------------------
	-- Account Info를 추가하기 전에, 먼저 기존에 같은 UserID인 사람이 있는지 확인해보자
	-- 중복된 ID가 있는지 확인..	
	SELECT @SelAID = l.AID, @SelUserID = l.UserID, @SelUserCN = l.Password
	FROM dbo.Login l(NOLOCK) 
	WHERE l.UserID = @UserID
	
	
	-- 만약 중복되는 ID가 있다면.... 삭제하는 쿼리 실행!
	IF (@SelAID IS NOT NULL) BEGIN
		DECLARE @RetValue INT;
		EXEC spRemoveAccount_Netmarble @SelAID, @RetValue;
		
		IF( @RetValue != 0 ) BEGIN
			SELECT @RetValue AS 'Ret'
			RETURN
		END		
	END
	
	-------------------------------------------------------------------------------------
	
	BEGIN TRAN
		
	INSERT INTO dbo.Account(UserID, Cert, Name, Age, Sex, UGradeID, PGradeID, RegDate)  
	VALUES(@UserID, @Cert, @Name, @Age, @Sex, 0, 0, GETDATE());	
	IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
		ROLLBACK TRAN
		SELECT 5 AS 'Ret'
		RETURN 
	END
 	
 	DECLARE @AIDIdenty  INT;
	SET @AIDIdenty = @@IDENTITY  
  
	INSERT INTO dbo.Login(UserID, AID, Password)  
	VALUES(@UserID, @AIDIdenty, @CN);	
	IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN
		ROLLBACK TRAN
		SELECT 6 AS 'Ret'
		RETURN 
	END
		
	COMMIT TRAN
	
	-------------------------------------------------------------------------------------
	
	SELECT 0 AS 'Ret'
END