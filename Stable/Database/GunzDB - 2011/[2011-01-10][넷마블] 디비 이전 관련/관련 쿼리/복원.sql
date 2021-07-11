////===============================================================

restore filelistonly from disk='D:\DATA_BACKUP\AdmWebDB_DB_201102170200.BAK'
restore headeronly from disk='D:\DATA_BACKUP\AdmWebDB_DB_201102170200.BAK'

restore database AdmWebDB 
 from disk='D:\DATA_BACKUP\AdmWebDB_DB_201102170200.BAK'
 with move 'AdmWebDB_Data' to 'F:\SQL_Data\AdmWebDB_Data.MDF',
   move 'AdmWebDB_Log' to 'G:\SQL_Log\AdmWebDB_Log.LDF'
-- 2분



////===============================================================================

restore filelistonly from disk='D:\DATA_BACKUP\LogDB_DB_201102170300.BAK'
restore headeronly from disk='D:\DATA_BACKUP\LogDB_DB_201102170300.BAK'
go
--2초

restore database LogDB 
 from disk='D:\DATA_BACKUP\LogDB_DB_201102170300.BAK'
 with move 'LogDB_Data' to 'F:\SQL_Data\LogDB_Data.MDF',
   move 'LogDB_Log' to 'G:\SQL_Log\LogDB_Log.LDF'

-- 6분

////===============================================================================

restore filelistonly from disk='E:\DATA_BACKUP\GunzDB_DB_201102170200.BAK'
restore headeronly from disk='E:\DATA_BACKUP\GunzDB_DB_201102170200.BAK'
go

restore database GunzDB 
 from disk='E:\DATA_BACKUP\GunzDB_DB_201102170200.BAK'
 with	move 'GunzDB_Data' to 'F:\SQL_Data\GunzDB_Data.MDF',
		move 'GunzDB_Log' to 'G:\SQL_Log\GunzDB_Log.LDF'


--25분

/*
USE GunzLogDB

BACKUP LOG GunzLogDB WITH TRUNCATE_ONLY
DBCC SHRINKFILE(GunzLogDB_Log, 1024)

DBCC SHRINKDATABASE ([데이터베이스이름], TRUNCATEONLY)

SELECT * FROM AccountDeleteLogTmp2

EXEC sp_spaceused GameLog
EXEC sp_help GameLog
*/

