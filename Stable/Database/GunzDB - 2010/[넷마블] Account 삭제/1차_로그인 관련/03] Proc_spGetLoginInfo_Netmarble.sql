CREATE PROCEDURE dbo.spGetLoginInfo_Netmarble
	@UserID		VARCHAR(20),
	@CN			VARCHAR(20)
AS BEGIN	
	SELECT l.AID, l.UserID, l.Password 
	FROM dbo.Login l WITH (NOLOCK) 
	WHERE l.UserID = @UserID AND l.Password = @CN;
END