USE GunzDB
GO

SELECT	o.Name, o.crdate, i.rowcnt, i.rows, i.reserved
FROM	SYSOBJECTS o(NOLOCK) JOIN SYSINDEXES i(NOLOCK)
	ON	o.id = i.id
WHERE	o.xtype = 'U'
AND		i.indid in(0, 1, 255)
ORDER BY i.rows DESC

ALTER TABLE Character DROP COLUMN head_slot;
ALTER TABLE Character DROP COLUMN chest_slot;
ALTER TABLE Character DROP COLUMN hands_slot;
ALTER TABLE Character DROP COLUMN legs_slot;
ALTER TABLE Character DROP COLUMN feet_slot;
ALTER TABLE Character DROP COLUMN fingerl_slot;
ALTER TABLE Character DROP COLUMN fingerr_slot;
ALTER TABLE Character DROP COLUMN melee_slot;
ALTER TABLE Character DROP COLUMN primary_slot;
ALTER TABLE Character DROP COLUMN secondary_slot;
ALTER TABLE Character DROP COLUMN custom1_slot;
ALTER TABLE Character DROP COLUMN custom2_slot;

ALTER TABLE Character DROP COLUMN head_itemid;
ALTER TABLE Character DROP COLUMN chest_itemid;
ALTER TABLE Character DROP COLUMN hands_itemid;
ALTER TABLE Character DROP COLUMN legs_itemid;
ALTER TABLE Character DROP COLUMN feet_itemid;
ALTER TABLE Character DROP COLUMN fingerl_itemid;
ALTER TABLE Character DROP COLUMN fingerr_itemid;
ALTER TABLE Character DROP COLUMN melee_itemid;
ALTER TABLE Character DROP COLUMN primary_itemid;
ALTER TABLE Character DROP COLUMN secondary_itemid;
ALTER TABLE Character DROP COLUMN custom1_itemid;
ALTER TABLE Character DROP COLUMN custom2_itemid;

DBCC DBREINDEX(Character)					-- 약 21분
-- DBCC SHOWCONTIG(Character)

DBCC DBREINDEX(CharacterEquipmentSlot)		-- 약 7분
-- DBCC SHOWCONTIG(CharacterEquipmentSlot)

DBCC DBREINDEX(CharacterItem)				-- 약 7분
-- DBCC SHOWCONTIG(CharacterItem)

DBCC DBREINDEX(AccountItem)					--
-- DBCC SHOWCONTIG(AccountItem)

DBCC DBREINDEX(Account)						-- 약 1분
-- DBCC SHOWCONTIG(Account	)

DBCC DBREINDEX(Login)						-- 약 1분
-- DBCC SHOWCONTIG(Login)


--ALTER INDEX Friend_PK ON dbo.Friend REBUILD;
--
--ALTER INDEX IX_Friend_CID_DeleteFlag_FriendCID ON dbo.Friend REBUILD WITH (FILLFACTOR=70);
--ALTER INDEX IX_Friend_FriendCID ON dbo.Friend REBUILD WITH (FILLFACTOR=70)