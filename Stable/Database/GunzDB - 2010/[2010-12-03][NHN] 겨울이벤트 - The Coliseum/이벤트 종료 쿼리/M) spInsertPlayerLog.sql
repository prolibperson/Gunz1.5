
-- 새로 만들어진 spInsertPlayerLog를 삭제한다.
DROP PROC spInsertPlayerLog;

-- 일단, 백업된 기존의 BackUp_spInsertPlayerLog의 이름을 바꿔준다.
EXEC sp_rename 'BackUp_spInsertPlayerLog', 'spInsertPlayerLog'
GO