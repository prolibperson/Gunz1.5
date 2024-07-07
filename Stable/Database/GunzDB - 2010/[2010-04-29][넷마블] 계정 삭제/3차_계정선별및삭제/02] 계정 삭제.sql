/* ===============================================================================
  선별한 대상들을 삭제해주는 쿼리
  
  -- 1. 선별한 대상들 중에서도 이상한 녀석들 찾기
  -- 2. 계정 삭제 및 로그 남기기
=============================================================================== */

--USE GunzdB
USE TGunzDB
GO




---------------------------------------------------------------------------------
-- 선별한 대상들 중에서도 이상한 녀석들 찾기
-- (Login과 Account Table간의 데이터가 안 맞을 수도 있음)
---------------------------------------------------------------------------------
BEGIN TRAN -------------

	-- 지워야할 타겟 중에, Account 테이블에 존재하지 않는 녀석들..	
	SELECT	t.AID, t.UserID, t.UserCN, t.DeleteID, 'Login' AS TableName
	INTO	AccountDeleteTargetNotExist
	FROM	AccountDeleteTarget t(NOLOCK)
	WHERE	NOT EXISTS( SELECT * FROM Account a(NOLOCK) WHERE a.AID = t.AID )

	-- 지워야할 타겟 중에, Login 테이블에 존재하지 않는 녀석들..
	INSERT INTO AccountDeleteTargetNotExist(AID, UserID, UserCN, DeleteID, TableName)
		SELECT	t.AID, t.UserID, t.UserCN, t.DeleteID, 'Account' AS TableName
		FROM	AccountDeleteTarget t(NOLOCK)
		WHERE	NOT EXISTS( SELECT * FROM Login l(NOLOCK) WHERE l.AID = t.AID )		
		
		
	-- 존재하지 않는 녀석들을 타겟에서 제외시킨다.
	DELETE	at
	FROM	AccountDeleteTarget at, AccountDeleteTargetNotExist an
	WHERE	at.AID = an.AID
	
	SELECT * FROM AccountDeleteTargetNotExist

COMMIT TRAN -------------
GO	




---------------------------------------------------------------------------------
-- 계정 삭제 및 로그 남기기
---------------------------------------------------------------------------------
BEGIN TRAN -------------

	-- Account 관련되어 변화가 심할 인덱스는 날려주세염!
	EXEC sp_helpindex Account
	-- DROP INDEX dbo.Account.IX_Account_UserID
	-- DROP INDEX dbo.Account.CUK_Account_UserID

	-- Account 삭제!			
	UPDATE	a
	SET		a.UserID = t.DeleteID, a.DeleteFlag = 1
	FROM	Account a, AccountDeleteTarget t
	WHERE	a.AID = t.AID
			
	
	-- Login 관련되어 변화가 심할 인덱스는 날려주세염!
	EXEC sp_helpindex Login
	-- DROP INDEX dbo.Login.CUK_Login_UserID	
	
	-- Login 삭제!
	UPDATE	l
	SET		l.UserID = t.DeleteID, l.Password = ''
	FROM	Login l, AccountDeleteTarget t
	WHERE	l.AID = t.AID
		
	-- 삭제한 인덱스 재생성!
	-- CREATE UNIQUE NONCLUSTERED INDEX UIX_Account_UserID ON Account(UserID)	
	-- CREATE NONCLUSTERED INDEX IX_Account_DeleteFlag ON Account(DeleteFlag)



	-- 삭제 로그에 남기기!
	DROP INDEX AccountDeleteLog.AccountDeleteLog_UserID_IX
	
	INSERT INTO dbo.AccountDeleteLog(AID, UserID, UserCN, DeleteID, DeleteDate)
		SELECT	AID, UserID, UserCN, DeleteID, GETDATE()
		FROM	AccountDeleteTarget(NOLOCK)
		
	CREATE INDEX AccountDeleteLog_UserID_IX ON AccountDeleteLog(UserID)	
	
COMMIT TRAN ------------
GO
