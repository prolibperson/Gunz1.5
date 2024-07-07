ALTER PROCEDURE dbo.spGetLoginInfo_Netmarble
	@UserID		VARCHAR(20),
	@CN			VARCHAR(20)
AS BEGIN
	SET NOCOUNT ON;
	
	SELECT	l.AID, l.UserID, l.Password 
	FROM	dbo.Login l(NOLOCK) 
	WHERE	l.UserID = @UserID 
	AND		l.Password = @CN;
	
END