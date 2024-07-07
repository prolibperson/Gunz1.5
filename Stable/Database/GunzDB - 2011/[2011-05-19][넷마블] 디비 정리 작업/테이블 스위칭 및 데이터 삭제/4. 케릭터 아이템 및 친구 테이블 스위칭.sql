USE GunzDB
GO

-------------------------------------------------------------------------------------------
/*
ALTER TABLE dbo.CharacterMgrLogByGM DROP FK__CharacterMg__CID__5887175A
ALTER TABLE dbo.Clan                DROP Clan_Character_FK1
ALTER TABLE dbo.ClanMember          DROP ClanMember_Clan_FK2
ALTER TABLE dbo.DTCharacterInfo     DROP Character_DTCharacterInfo_FK1
GO
*/

-------------------------------------------------------------------------------------------

/**
    단계 1. Friend 테이블 스위칭
*/

EXEC SP_RENAME 'Friend',     'FriendOld';
EXEC SP_RENAME 'Friend_PK',  'PK_FriendOld';

CREATE TABLE [dbo].[Friend]
(
	[ID]	            INT	IDENTITY(1, 1)	NOT NULL, 
	[CID]	            INT	 	            NOT NULL, 
	[FriendCID]	        INT	     	        NOT NULL, 
	[Type]	            INT                 NOT NULL,
    [Favorite]          TINYINT             NULL,
    [DeleteFlag]        TINYINT             NULL
);

------------------------------------------------

ALTER DATABASE GunzDB SET RECOVERY SIMPLE;
SET IDENTITY_INSERT dbo.Friend ON;

INSERT dbo.Friend(ID, CID, FriendCID, Type, Favorite, DeleteFlag)
    SELECT  DISTINCT FO.id, FO.CID, FO.FriendCID, FO.Type, FO.Favorite, FO.DeleteFlag
    FROM    dbo.FriendOld FO(TABLOCK)
    WHERE   FO.CID IN ( SELECT CID FROM Character )
    AND     FO.FriendCID IN ( SELECT CID FROM Character )

DECLARE @OldIdentity INT;
SET @OldIdentity = IDENT_CURRENT('FriendOld') + 1;
DBCC CHECKIDENT('Friend', RESEED, @OldIdentity)

SET IDENTITY_INSERT dbo.Friend OFF;
ALTER DATABASE GunzDB SET RECOVERY SIMPLE;


------------------------------------------------

ALTER TABLE [dbo].[Friend] ADD CONSTRAINT PK_Friend PRIMARY KEY CLUSTERED (ID); 

CREATE NONCLUSTERED INDEX IX_Friend_CID_DeleteFlag_FriendCID ON [dbo].[Friend](CID, DeleteFlag, FriendCID);
CREATE NONCLUSTERED INDEX IX_Friend_FriendCID ON [dbo].[Friend](FriendCID) ;

ALTER TABLE dbo.Friend WITH CHECK ADD CONSTRAINT FK_Friend_Character_CID
FOREIGN KEY (CID) REFERENCES [dbo].[Character](CID)

ALTER TABLE dbo.Friend WITH CHECK ADD CONSTRAINT FK_Friend_Character_FriendCID
FOREIGN KEY (FriendCID) REFERENCES [dbo].[Character](CID)


/**
    단계 2. CharacterItem 테이블 스위칭
*/

------------------------------------------------

EXEC SP_RENAME 'CharacterItem',     'CharacterItemOld';
EXEC SP_RENAME 'CharacterItem_PK',  'CharacterItemOld_PK';

CREATE TABLE [dbo].[CharacterItem]
(
	[CIID]	            INT	IDENTITY(1, 1)	NOT NULL, 
	[CID]	            INT	 	            NULL, 
	[ItemID]	        INT	     	        NOT NULL, 
	[RegDate]	        DATETIME	 	    NULL, 
	[RentDate]	        DATETIME	 	    NULL, 
	[RentHourPeriod]	SMALLINT	 	    NULL, 
	[Cnt]	            INT	 	            NULL, 
);

------------------------------------------------

ALTER DATABASE GunzDB SET RECOVERY SIMPLE;
SET IDENTITY_INSERT dbo.CharacterItem ON;

INSERT dbo.CharacterItem(CIID, CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)
    SELECT  CI.CIID, CI.CID, CI.ItemID, CI.RegDate, Ci.RentDate, Ci.RentHourPeriod, CI.Cnt
    FROM    dbo.CharacterItemOld CI(TABLOCK)
            INNER JOIN dbo.Character C(TABLOCK)
            ON  CI.CID = C.CID

DECLARE @OldIdentity INT;
SET @OldIdentity = IDENT_CURRENT('CharacterItemOld') + 1;
DBCC CHECKIDENT('CharacterItem', RESEED, @OldIdentity)

ALTER DATABASE GunzDB SET RECOVERY SIMPLE;
SET IDENTITY_INSERT dbo.CharacterItem OFF;

------------------------------------------------

ALTER TABLE [dbo].[CharacterItem] ADD CONSTRAINT PK_CharacterItem PRIMARY KEY CLUSTERED (CIID) 

CREATE NONCLUSTERED INDEX IX_CharacterItem_CID ON [dbo].[CharacterItem](CID) 

ALTER TABLE dbo.CharacterItem WITH CHECK ADD CONSTRAINT FK_CharacterItem_Character
FOREIGN KEY (CID) REFERENCES [dbo].[Character](CID)

ALTER TABLE dbo.CharacterItem WITH CHECK ADD CONSTRAINT FK_CharacterItem_Item
FOREIGN KEY (ItemID) REFERENCES [dbo].[Item](ItemID)

------------------------------------------------
