DROP TABLE DTTimeStamp
DROP TABLE DTGameLog
DROP TABLE DTGameLogDetail
DROP TABLE DTCharacterInfo
DROP TABLE DTCharacterRanking


SELECT * FROM DTTimeStamp
SELECT * FROM DTGameLog
SELECT * FROM DTGameLogDetail
SELECT c.Name, ci.* FROM DTCharacterInfo ci, Character c WHERE ci.CID = c.CID

EXEC sp_help DTTimeStamp
EXEC sp_help DTGameLog
EXEC sp_help DTGameLogDetail
EXEC sp_help DTCharacterInfo
EXEC sp_help DTCharacterRanking

SELECT * 
FROM DTGameLogDetail 
WHERE TimeStamp = '20091124'
	AND (WinnerCID = 1256 OR WinnerCID = 1257 OR LoserCID = 1256 OR LoserCID = 1257)

