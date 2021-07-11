/*
	디비 이전과 관련하여 디비 사이즈를 줄이는 목적의 일환으로 
	테이블을 스위칭하여 임시 보관하는 쿼리문
	
	스위칭 대상 테이블 -	ItemPurchaseLogByBounty(22GB)	-
							PlayerLog(18GB)					- 
							GameLog(12GB)					- 
							ClanGameLog(11GB)				- 
							QuestGameLog(9GB)				- 
							BringAccountItemLog(7GB)		- 
							LevelUpLog(2GB)					- 
							QUniqueItemLog(1GB)				- 완료
	
*/

------------------------------------------------------------------------------------

USE GunzDB
GO

EXEC sp_rename 'QUniqueItemLog', 'AA_QUniqueItemLog'
EXEC sp_rename 'QUniqueItemLogSW', 'QUniqueItemLog'

EXEC sp_rename 'LevelUpLog', 'AA_LevelUpLog'
EXEC sp_rename 'LevelUpLogSW', 'LevelUpLog'

EXEC sp_rename 'QuestGameLog', 'AA_QuestGameLog'
EXEC sp_rename 'QuestGameLogSW', 'QuestGameLog'

EXEC sp_rename 'ClanGameLog', 'AA_ClanGameLog'
EXEC sp_rename 'ClanGameLogSW', 'ClanGameLog'

EXEC sp_rename 'GameLog', 'AA_GameLog'
EXEC sp_rename 'GameLogSW', 'GameLog'

EXEC sp_rename 'PlayerLog', 'AA_PlayerLog'
EXEC sp_rename 'PlayerLogSW', 'PlayerLog'

EXEC sp_rename 'ItemPurchaseLogByBounty', 'AA_ItemPurchaseLogByBounty'
EXEC sp_rename 'ItemPurchaseLogByBountySW', 'ItemPurchaseLogByBounty'

------------------------------------------------------------------------------------

/*
USE GunzDB
GO

-- EXEC sp_rename 'QUniqueItemLog', 'QUniqueItemLogSW'
-- EXEC sp_rename 'AA_QUniqueItemLog', 'QUniqueItemLog'

EXEC sp_rename 'LevelUpLog', 'LevelUpLogSW'
EXEC sp_rename 'AA_LevelUpLog', 'LevelUpLog'

EXEC sp_rename 'QuestGameLog', 'QuestGameLogSW'
EXEC sp_rename 'AA_QuestGameLog', 'QuestGameLog'

EXEC sp_rename 'ClanGameLog', 'ClanGameLogSW'
EXEC sp_rename 'AA_ClanGameLog', 'ClanGameLog'

EXEC sp_rename 'GameLog', 'GameLogSW'
EXEC sp_rename 'AA_GameLog', 'GameLog'

EXEC sp_rename 'PlayerLog', 'PlayerLogSW'
EXEC sp_rename 'AA_PlayerLog', 'PlayerLog'

EXEC sp_rename 'ItemPurchaseLogByBounty', 'ItemPurchaseLogByBountySW'
EXEC sp_rename 'AA_ItemPurchaseLogByBounty', 'ItemPurchaseLogByBounty'
*/
------------------------------------------------------------------------------------



USE GunzDB
GO

DBCC SHRINKFILE(GunzDB_Log, 10240)
GO




------------------------------------------------------------------------------------

USE GunzLogDB
GO

ALTER DATABASE GunzLogDB SET RECOVERY SIMPLE
GO

SELECT	*
INTO	dbo.AA_QUniqueItemLog
FROM	GunzDB.dbo.AA_QUniqueItemLog WITH (TABLOCK)
OPTION (MAXDOP 1)

SELECT	*
INTO	dbo.AA_LevelUpLog
FROM	GunzDB.dbo.AA_LevelUpLog WITH (TABLOCK)
OPTION (MAXDOP 1)

SELECT	*
INTO	dbo.AA_QuestGameLog
FROM	GunzDB.dbo.AA_QuestGameLog WITH (TABLOCK)
OPTION (MAXDOP 1)

SELECT	*
INTO	dbo.AA_ClanGameLog
FROM	GunzDB.dbo.AA_ClanGameLog WITH (TABLOCK)
OPTION (MAXDOP 1)

SELECT	*
INTO	dbo.AA_GameLog
FROM	GunzDB.dbo.AA_GameLog WITH (TABLOCK)
OPTION (MAXDOP 1)

SELECT	*
INTO	dbo.AA_PlayerLog
FROM	GunzDB.dbo.AA_PlayerLog WITH (TABLOCK)
OPTION (MAXDOP 1)

SELECT	*
INTO	dbo.AA_ItemPurchaseLogByBounty
FROM	GunzDB.dbo.AA_ItemPurchaseLogByBounty WITH (TABLOCK)
OPTION (MAXDOP 1)


ALTER DATABASE GunzLogDB SET RECOVERY FULL
GO


/*
USE GunzDB
GO

DROP TABLE AA_QUniqueItemLog
DROP TABLE AA_LevelUpLog
DROP TABLE AA_QuestGameLog
DROP TABLE AA_ClanGameLog
DROP TABLE AA_GameLog
DROP TABLE AA_PlayerLog
DROP TABLE AA_ItemPurchaseLogByBounty
DROP TABLE AA_PlayerLog
*/