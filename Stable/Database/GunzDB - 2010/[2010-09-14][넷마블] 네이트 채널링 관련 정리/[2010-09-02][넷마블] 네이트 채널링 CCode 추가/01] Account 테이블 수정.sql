USE GunzDB

-- Account 테이블에 CCode 추가
ALTER TABLE dbo.Account ADD CCode TINYINT
GO

UPDATE	dbo.Account 
SET		CCode = 0

UPDATE	dbo.Account 
SET		DelFlag = 0
WHERE	DelFlag IS NULL
GO

ALTER TABLE dbo.Account ADD CONSTRAINT DF_Account_DelFlag DEFAULT (0) FOR DelFlag
GO