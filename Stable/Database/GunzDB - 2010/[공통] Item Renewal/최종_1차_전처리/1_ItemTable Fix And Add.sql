USE GunzDB
GO

-----------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
ALTER TABLE Item ADD IsSpendableItem TINYINT DEFAULT(0);
GO

UPDATE Item SET IsSpendableItem = 0;
GO

-----------------------------------------------------------------------------------------------------------

CREATE TABLE dbo.ItemChangeType
(
	ChangeType	SMALLINT
	, [DESC]	VARCHAR(MAX)
	
	CONSTRAINT PK_ItemChangeType PRIMARY KEY CLUSTERED (ChangeType ASC)
)
GO

INSERT ItemChangeType(ChangeType, [DESC]) VALUES (100, 'Move In');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (101, 'Purchase');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (102, 'Gamble Reward');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (103, 'Inserted');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (104, 'Character Init');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (105, 'Quest Distribute');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (110, 'Inserted');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (150, 'By Admin');

INSERT ItemChangeType(ChangeType, [DESC]) VALUES (200, 'Move Out');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (201, 'Sell');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (202, 'Gamble');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (203, 'Expired');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (204, 'Spend');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (210, 'Deleted');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (250, 'By Admin');
INSERT ItemChangeType(ChangeType, [DESC]) VALUES (300, 'Unknown');

------------------------------------------------------------------------------------


------------------------------------------------------------------------------------
CREATE TABLE dbo.ItemChangeLog_AccountItem
(	
	ChangeDate		DATETIME	NOT NULL
	, ChangeType	SMALLINT	NOT NULL
	, LogID			INT			IDENTITY(1,1)	
	, AID			INT			NOT NULL
	, AIID			INT			NOT NULL
	, CID			INT			NULL
	, CIID			INT			NULL
	, ItemID		INT			NOT NULL
	, [Count]		INT			NOT NULL
	--, [LeftCount]	INT			NOT NULL
		
	CONSTRAINT PK_ItemChangeLog_AccountItem PRIMARY KEY CLUSTERED (ChangeDate, ChangeType, LogID)
	
	, CONSTRAINT FK_ItemChangeLog_AccountItem_ItemChangeType
		FOREIGN KEY (ChangeType) REFERENCES dbo.ItemChangeType(ChangeType)
)
GO

CREATE NONCLUSTERED INDEX IX_ItemChangeLog_AccountItem_AID ON dbo.ItemChangeLog_AccountItem(AID)
------------------------------------------------------------------------------------


CREATE TABLE dbo.ItemChangeLog_CharacterItem
(	
	ChangeDate		DATETIME	NOT NULL
	, ChangeType	SMALLINT	NOT NULL	
	, LogID			INT			IDENTITY(1,1)
	, CID			INT			NOT NULL
	, CIID			INT			NOT NULL
	, AID			INT			NULL
	, AIID			INT			NULL	
	, ItemID		INT			NOT NULL
	, [Count]		INT			NOT NULL
	--, [LeftCount]	INT			NOT NULL
		
	CONSTRAINT PK_ItemChangeLog_CharacterItem PRIMARY KEY CLUSTERED (ChangeDate, ChangeType, LogID)
	
	, CONSTRAINT FK_ItemChangeLog_CharacterItem_ItemChangeType
		FOREIGN KEY (ChangeType) REFERENCES dbo.ItemChangeType(ChangeType)
)
GO

CREATE NONCLUSTERED INDEX IX_ItemChangeLog_CharacterItem_CID ON dbo.ItemChangeLog_CharacterItem(CID)
------------------------------------------------------------------------------------