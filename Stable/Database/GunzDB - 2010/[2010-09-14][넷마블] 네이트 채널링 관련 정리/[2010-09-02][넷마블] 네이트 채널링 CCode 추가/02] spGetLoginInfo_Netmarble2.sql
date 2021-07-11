USE GunzDB
GO

CREATE PROCEDURE dbo.spGetLoginInfo_Netmarble2
	@UserID		VARCHAR(20),
	@CN			VARCHAR(20)
AS BEGIN
	SET NOCOUNT ON;
	
	SELECT	l.AID, l.UserID, l.Password, a.CCode
	FROM	dbo.Login l(NOLOCK), dbo.Account a(NOLOCK)
	WHERE	a.AID = l.AID
	AND		l.UserID = @UserID 
	AND		l.Password = @CN;
		
END