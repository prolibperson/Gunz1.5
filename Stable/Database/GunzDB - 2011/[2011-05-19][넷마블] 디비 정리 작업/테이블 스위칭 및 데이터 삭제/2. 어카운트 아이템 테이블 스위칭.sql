USE GunzDB
GO

-------------------------------------------------------------------------------------------

/**
    변경 방법. AccountItem 테이블 스위칭으로 해결하자.
*/

------------------------------------------------

EXEC SP_RENAME 'AccountItem',     'AccountItemOld';

CREATE TABLE [dbo].[AccountItem]
(
	[AIID]	            INT	IDENTITY(1, 1)	NOT NULL, 
	[AID]	            INT	 	            NULL, 
	[ItemID]	        INT	     	        NOT NULL, 
	[RentDate]	        DATETIME	 	    NULL, 
	[RentHourPeriod]	SMALLINT	 	    NULL, 
	[Cnt]	            INT                 NULL, 
);

------------------------------------------------

ALTER DATABASE GunzDB SET RECOVERY SIMPLE;
SET IDENTITY_INSERT dbo.AccountItem ON;

INSERT dbo.AccountItem(AIID, AID, ItemID, RentDate, RentHourPeriod, Cnt)
    SELECT  AI.AIID, AI.AID, AI.ItemID, AI.RentDate, AI.RentHourPeriod, AI.Cnt
    FROM    dbo.AccountItemOld AI(TABLOCK)
            INNER JOIN dbo.Account A(TABLOCK)
            ON A.AID = AI.AID

DECLARE @OldIdentity INT;
SET @OldIdentity = IDENT_CURRENT('AccountItemOld') + 1;
DBCC CHECKIDENT('AccountItem', RESEED, @OldIdentity)

SET IDENTITY_INSERT dbo.AccountItem OFF;
ALTER DATABASE GunzDB SET RECOVERY SIMPLE;

------------------------------------------------

ALTER TABLE [dbo].[AccountItem] ADD CONSTRAINT PK_AccountItem PRIMARY KEY CLUSTERED (AIID) 
GO

ALTER TABLE dbo.AccountItem WITH CHECK ADD CONSTRAINT FK_AccountItem_Account
FOREIGN KEY (AID) REFERENCES [dbo].[AccountItem](AID)
GO

ALTER TABLE dbo.AccountItem WITH CHECK ADD CONSTRAINT FK_AccountItem_Item
FOREIGN KEY (ItemID) REFERENCES [dbo].[AccountItem](ItemID)
GO

CREATE NONCLUSTERED INDEX IX_AccountItem_AID ON [dbo].[AccountItem](AID) 

ALTER TABLE dbo.AccountItem WITH CHECK ADD CONSTRAINT FK_AccountItem_Account_AID
FOREIGN KEY (AID) REFERENCES [dbo].[Account](AID)

ALTER TABLE dbo.AccountItem WITH CHECK ADD CONSTRAINT FK_AccountItem_Item_ItemID
FOREIGN KEY (ItemID) REFERENCES [dbo].[Item](ItemID)

ALTER TABLE dbo.AccountItem ADD CONSTRAINT DF_AccountItem_Cnt DEFAULT(0) FOR Cnt;    

------------------------------------------------



