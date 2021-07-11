/* ===============================================================================
  Account 테이블에 추가로 필요할 것 같은 'DeleteFlag' 필드를 추가하는 쿼리문
  
  -- 1. 필드 추가
  -- 2. 먼저, 모든 필드를 0으로 업데이트
  -- 3. 이미 삭제된 이력이 있는 계정에 대해서는 다시 1로 업데이트 한다.
  -- 4. 로그에 없지만 삭제된 계정을 눈으로 찾아서 1로 업데이트 한다.
=============================================================================== */
BEGIN TRAN ----------
	ALTER TABLE Account ADD DeleteFlag TINYINT

	UPDATE Account SET DeleteFlag = 0

	UPDATE	a
	SET		a.DeleteFlag = 1
	FROM	Account a, AccountDeleteLog al
	WHERE	a.AID = al.AID
COMMIT TRAN ---------


-- 아래의 쿼리문에서 눈으로 선별된 데이터임
-----------------------------------------------------------------------------------
/*
SELECT	* 
FROM	Account a
WHERE	a.UserID LIKE '%-%-%-%'
AND		NOT EXISTS ( SELECT * FROM AccountDeleteLog al where al.AID = a.AID )
*/
-----------------------------------------------------------------------------------
/*
UPDATE Account SET DeleteFlag = 1 WHERE AID = 2360246
UPDATE Account SET DeleteFlag = 1 WHERE AID = 5349799
UPDATE Account SET DeleteFlag = 1 WHERE AID = 5653342
UPDATE Account SET DeleteFlag = 1 WHERE AID = 5930447
UPDATE Account SET DeleteFlag = 1 WHERE AID = 5930514
*/