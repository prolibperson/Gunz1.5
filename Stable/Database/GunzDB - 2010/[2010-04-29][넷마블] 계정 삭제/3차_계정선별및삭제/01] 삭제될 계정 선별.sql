/* ===============================================================================
  넷마블에서 추출해준 데이터를 토대로 삭제할 계정의 대상을 선별하는 쿼리문
  
  -- 1. 넷마블에서 추출해준 데이터를 정리
  -- 2. 삭제할 계정의 대상을 저장하는 테이블 생성
  -- 3. 삭제할 계정 선별 - 탈퇴가 확실한 회원들 선별하기
  -- 4. 갱신 되어야 될 계정 갱신해주기(삭제가 아닌 UserCN 갱신)
  -- 5. 삭제 대상의 중복 제거 및 DeleteID 생성
=============================================================================== */

--USE GunzDB
USE TGunzDB


--------------------------------------------------------------------------
-- 넷마블에서 추출해준 데이터를 정리
--------------------------------------------------------------------------
BEGIN TRAN ---------
	--------------------------------------------------------------------------
	-- 중복된 행을 제거하는 부분
	-- 넷마블 탈퇴 DB에 중복된 자료가 있는 것 같습니다.
	--------------------------------------------------------------------------
	SELECT DISTINCT * INTO TempDB_2.dbo.gunz_withdraw_filter 		 FROM TempDB_2.dbo.gunz_withdraw 
	SELECT DISTINCT * INTO TempDB_2.dbo.gunz_baduser_withdraw_filter FROM TempDB_2.dbo.gunz_baduser_withdraw
	SELECT DISTINCT * INTO TempDB_2.dbo.gunz_baduser_nothing_filter	 FROM TempDB_2.dbo.gunz_baduser_nothing
	SELECT DISTINCT * INTO TempDB_2.dbo.gunz_baduser_active_filter 	 FROM TempDB_2.dbo.gunz_baduser_active
COMMIT TRAN --------
GO



--------------------------------------------------------------------------
-- 삭제할 계정의 대상을 저장하는 테이블 생성
--------------------------------------------------------------------------
BEGIN TRAN ---------
	--------------------------------------------------------------------------
	-- 선별된 삭제 대상을 저장하고, Delete ID를 만들 테이블
	-- 모두 선별 후, 중복된 대상이 있어서 2개를 생성!
	--------------------------------------------------------------------------
	CREATE TABLE dbo.AccountDeleteTargetTemp
	(
		AID					INT				NOT NULL,
		UserID				VARCHAR(20) 	NOT NULL,
		UserCN				VARCHAR(20) 	NOT NULL,
		DeleteID			VARCHAR(20)		NULL
	)

	CREATE TABLE dbo.AccountDeleteTarget
	(
		AID					INT				NOT NULL,
		UserID				VARCHAR(20) 	NOT NULL,
		UserCN				VARCHAR(20) 	NOT NULL,
		DeleteID			VARCHAR(20)		NULL
	)	
COMMIT TRAN --------
GO



--------------------------------------------------------------------------
-- 삭제할 계정 선별 - 탈퇴가 확실한 회원들 선별하기
--------------------------------------------------------------------------
BEGIN TRAN ---------	
	CREATE NONCLUSTERED INDEX Login_UserCN_IX ON Login(Password)
	
	--------------------------------------------------------------------------
	-- Step 1 - 정상(탈퇴회원) 삭제 대상 선별하기
	--------------------------------------------------------------------------
	INSERT INTO dbo.AccountDeleteTargetTemp(AID, UserID, UserCN)
		SELECT 	l.AID, l.UserID, l.Password
		FROM 	TempDB_2.dbo.gunz_withdraw_filter gw(NOLOCK)
				, Login l(NOLOCK)
		WHERE 	gw.cn = l.Password

	--------------------------------------------------------------------------
	-- Step 2 - 비정상(탈퇴 회원) 삭제 대상 선별하기
	--------------------------------------------------------------------------
	INSERT INTO dbo.AccountDeleteTargetTemp(AID, UserID, UserCN)
		SELECT 	DISTINCT l.AID, l.UserID, l.Password
		FROM 	TempDB_2.dbo.gunz_baduser_withdraw_filter gw(NOLOCK)
				, Login l(NOLOCK)
		WHERE 	gw.cn = l.Password

	DROP INDEX Login.Login_UserCN_IX
COMMIT TRAN --------
GO




--------------------------------------------------------------------------
-- 삭제할 계정 선별 - 비정상적인 회원들 선별하기
--------------------------------------------------------------------------
BEGIN TRAN ---------

	------------------------------------------------------------------------------
	-- Step 1 - 비정상(유령 회원) - 삭제 대상 선별하기(접속한 기록이 없는 이상한 ID들은 삭제)
	------------------------------------------------------------------------------
	-- 일단 임시 테이블을 하나 생성
	SELECT 	*
	INTO	Temp_baduser_nothing
	FROM 	TempDB_2.dbo.gunz_baduser_nothing_filter gw(NOLOCK)
			, Login l(NOLOCK)
	WHERE 	gw.id = l.UserID

	-- 접속한 적이 없는 사람들도 삭제 대상
	-- (접속한 사람이 있는 경우도 있음 -> 추후 다시 처리)
	INSERT INTO dbo.AccountDeleteTargetTemp(AID, UserID, UserCN)
		SELECT 	t.AID, t.UserID, t.cn
		FROM 	Temp_baduser_nothing t
				, Account a(NOLOCK)
		WHERE 	a.AID = t.AID
		AND 	a.LastLogoutTime IS NULL
		
		
	--------------------------------------------------------------------------
	-- Step 2 - 비정상(회원) - 삭제 대상 선별하기
	--------------------------------------------------------------------------
	-- 일단 임시 테이블을 하나 생성
	SELECT 	a.AID, a.UserID, g.cn, g.netmarble_cn, g.RegDate, a.LastLogoutTime
	INTO 	Temp_baduser_active
	FROM	TempDB_2.dbo.gunz_baduser_active_filter g(NOLOCK)
			, Account a(NOLOCK)
	WHERE	a.UserID = g.id


	-- 로그인한 적 없는 회원들 삭제!
	INSERT INTO dbo.AccountDeleteTargetTemp(AID, UserID, UserCN)
		SELECT 	t.AID, t.UserID, t.cn
		FROM 	Temp_baduser_active t(NOLOCK)
		WHERE 	t.LastLogoutTime IS NULL


	-- 마지막 게임을 즐긴 시간 < 넷마블 아이디 생성 날짜 
	-- = 재가입한 회원은 게임을 즐긴 적이 없음
	INSERT INTO dbo.AccountDeleteTargetTemp(AID, UserID, UserCN)
		SELECT 	t.AID, t.UserID, t.cn
		FROM 	Temp_baduser_active t(NOLOCK)
		WHERE 	t.LastLogoutTime IS NOT NULL
		AND		t.LastLogoutTime < t.RegDate

COMMIT TRAN --------
GO




--------------------------------------------------------------------------
-- 갱신 되어야 될 계정 갱신해주기(삭제가 아닌 UserCN 갱신)
--------------------------------------------------------------------------
BEGIN TRAN ---------

	-- 마지막 게임을 즐긴 시간 > 넷마블 아이디 생성 날짜 
	-- (재가입한 회원이 현재 게임을 즐기고 있음)
	UPDATE	l
	SET		l.Password = gf.netmarble_cn
	FROM 	Temp_baduser_active gf(NOLOCK)
			, Login l
	WHERE 	gf.UserID = l.UserID
	AND		gf.netmarble_cn != l.Password
	AND 	gf.LastLogoutTime IS NOT NULL 
	AND		gf.LastLogoutTime > gf.Regdate
	
COMMIT TRAN --------
GO




--------------------------------------------------------------------------
-- 삭제 대상의 중복 제거 및 DeleteID 생성
--------------------------------------------------------------------------
BEGIN TRAN ---------	

	INSERT dbo.AccountDeleteTarget(AID, UserID, UserCN, DeleteID)
		SELECT DISTINCT AID, UserID, UserCN
		FROM dbo.AccountDeleteTargetTemp(NOLOCK)	

	UPDATE	AccountDeleteTarget
	SET		DeleteID = CAST( CAST(NEWID() AS VARCHAR(40)) AS VARCHAR(20))

	CREATE NONCLUSTERED INDEX AccountDeleteTarget_AID_IX ON AccountDeleteTarget(AID)

	DROP TABLE AccountDeleteTargetTemp
	
	/*
	SELECT	COUNT(*)
	FROM	AccountDeleteTarget t(NOLOCK)
			, AccountDeleteLog l(NOLOCK)
	WHERE	l.DeleteID = t.DeleteID
	*/
COMMIT TRAN --------
GO