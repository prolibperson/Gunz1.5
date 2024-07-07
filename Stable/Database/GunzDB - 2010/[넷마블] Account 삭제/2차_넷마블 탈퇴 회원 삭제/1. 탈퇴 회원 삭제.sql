/*
DROP TABLE dbo.NetmarbleBreakUser
CREATE TABLE dbo.NetmarbleBreakUser
(
	UserCN 			VARCHAR(20) NOT NULL,		
	UserID			VARCHAR(20) NOT NULL,
	AID				INT			NOT NULL,
	BreakDate		DATETIME	NOT NULL,
	ErrCode			INT			NULL
		
	CONSTRAINT NetmarbleBreakUser_PK PRIMARY KEY CLUSTERED (UserCN ASC)
)
SELECT * FROM dbo.NetmarbleBreakUser
EXEC spRegularRemoveBreakAccount_Netmarble '2010-02-18 00:00:00.000'
*/

CREATE PROC dbo.spRegularRemoveBreakAccount_Netmarble
	@TargetDate DATETIME
AS BEGIN
	SET NOCOUNT ON;
	
	---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	DELETE NetmarbleBreakUser WHERE ErrCode = 0
	---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	DECLARE @strSQL NVARCHAR(512);
	DECLARE @curDate DATETIME;
	
	SET @curDate = @TargetDate;
	SET @strSQL = N'INSERT INTO NetmarbleBreakUser(UserCN, UserID, AID, BreakDate)'
	SET @strSQL = @strSQL + N' SELECT l.Password , l.UserID, l.AID, r.withdrawdate FROM OPENQUERY([NETMARBLE_BREAKUSER], ''SELECT * FROM [SQLSVR].[dbo].[fn_GET_Breack_UserCN](';
	SET @strSQL = @strSQL + N'''''' + CONVERT( CHAR(10), @curDate, 120) + ''''')'') r, Login l WHERE r.CN = l.Password';
	
	--SELECT @strSQL
	EXEC sp_executesql @strSQL;
	
	---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	-- Cursor를 이용, 삭제 대상이 될 AID마다 고유한 DeleteID를 부요한다.
	DECLARE @AID INT;
	DECLARE @UserCN VARCHAR(20);
	DECLARE @Ret INT;
	
	DECLARE Curs CURSOR FAST_FORWARD FOR 
		SELECT t.AID, t.UserCN
		FROM dbo.NetmarbleBreakUser t(NOLOCK)
		WHERE ErrCode IS NULL
	
	
	OPEN Curs
	FETCH NEXT FROM Curs INTO @AID, @UserCN;
	
	WHILE( @@FETCH_STATUS = 0 ) BEGIN
		EXEC spRemoveAccount_Netmarble @AID, @Ret OUTPUT;
		UPDATE NetmarbleBreakUser SET ErrCode = @Ret WHERE UserCN = @UserCN;
		FETCH NEXT FROM Curs INTO @AID, @UserCN;
	END
		
	CLOSE Curs
	DEALLOCATE Curs
	---------------------------------------------------------------------------------------------------------------------------------------------------------------------
END