USE GunzDB
GO

-----------------------------------------------------------------------------------------------------------
-- 작업할 쿼리

-- 케릭터들이 장비하는 아이템 슬롯의 코드 테이블 생성 쿼리
CREATE TABLE CharacterEquipmentSlotCode
(
	SlotID		INT,
	SlotType	VARCHAR(20)
	
	CONSTRAINT PK_CharacterEquipmentSlotCode PRIMARY KEY CLUSTERED (SlotID ASC)
)
GO

-- 아이템 슬롯의 코드 입력
INSERT CharacterEquipmentSlotCode VALUES(0, 'Head');
INSERT CharacterEquipmentSlotCode VALUES(1, 'Chest');
INSERT CharacterEquipmentSlotCode VALUES(2, 'Hands');
INSERT CharacterEquipmentSlotCode VALUES(3, 'Legs');
INSERT CharacterEquipmentSlotCode VALUES(4, 'Feet');
INSERT CharacterEquipmentSlotCode VALUES(5, 'FingerLeft');
INSERT CharacterEquipmentSlotCode VALUES(6, 'FingerRight');	
INSERT CharacterEquipmentSlotCode VALUES(7, 'Melee');
INSERT CharacterEquipmentSlotCode VALUES(8, 'Primary');
INSERT CharacterEquipmentSlotCode VALUES(9, 'Secondary');
INSERT CharacterEquipmentSlotCode VALUES(10, 'Custom1');
INSERT CharacterEquipmentSlotCode VALUES(11, 'Custom2');
INSERT CharacterEquipmentSlotCode VALUES(12, 'Avatar');
	
------------------------------------------------------------------------------------------------------------------------------------------------

-- Character 테이블에 존재하던 케릭터들의 장비 정보를 모두 삽입
-- 주의 1) 시간이 오래 걸리는 쿼리입니다. 하지만, 소모성 아이템 패치 당일날 실행해야 합니다.
--        그래야 최신의 장비 장착 정보가 테이블에 삽입됩니다.

-- 먼저, 삭제된 케릭터들의 장비 정보를 모두 NULL로 삽입
WITH T_CHAR AS
	(
		SELECT CID / CID AS HS, CID
		  FROM dbo.CHARACTER C WITH (TABLOCK)
		 WHERE DeleteFlag <> 0
		 GROUP BY CID
	)
	, T_EQ AS
	(
		SELECT CASE WHEN SlotID <> 1 THEN 1 ELSE 1 END AS HS, SlotID
		  FROM dbo.CharacterEquipmentSlotCode WITH (TABLOCK)
		 GROUP BY SlotID
	)
SELECT C.CID, E.SlotID, NULL AS CIID, NULL AS ItemID
  INTO CharacterEquipmentSlot
  FROM T_EQ E
  JOIN T_CHAR C
	ON E.HS = C.HS
OPTION (HASH JOIN);


-- 삭제되지 않은 케릭터들의 장비 정보를 다음의 쿼리문을 이용하여 일괄적으로 삽입
SELECT	*
INTO	tmpCharacter
FROM	Character
WHERE	DeleteFlag = 0

INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 0 AS SlotID, head_slot AS CIID, head_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)

INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 1 AS SlotID, chest_slot AS CIID, chest_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)

INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 2 AS SlotID, Hands_slot AS CIID, Hands_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)
	
INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 3 AS SlotID, Legs_slot AS CIID, Legs_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)
	
INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 4 AS SlotID, Feet_slot AS CIID, Feet_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)
	
INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 5 AS SlotID, fingerl_slot AS CIID, fingerl_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)
		
INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 6 AS SlotID, fingerr_slot AS CIID, fingerr_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)

INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 7 AS SlotID, melee_slot AS CIID, melee_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)

INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 8 AS SlotID, Primary_slot AS CIID, Primary_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)
	
INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 9 AS SlotID, Secondary_slot AS CIID, Secondary_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)
	
INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 10 AS SlotID, custom1_slot AS CIID, custom1_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)

INSERT	CharacterEquipmentSlot(CID, SlotID, CIID, ItemID)
	SELECT	CID, 11 AS SlotID, custom2_slot AS CIID, custom2_itemid AS ItemID
	FROM	tmpCharacter WITH (TABLOCK)


-- Clustered Index 만들기	
ALTER TABLE dbo.CharacterEquipmentSlot WITH CHECK ADD CONSTRAINT PK_CharacterEquipmentSlot PRIMARY KEY CLUSTERED (CID ASC, SlotID ASC)
GO

-- SlotID Foreign Key 생성
ALTER TABLE dbo.CharacterEquipmentSlot WITH CHECK ADD CONSTRAINT FK_CharacterEquipmentSlotCode_CharacterEquipmentSlot FOREIGN KEY(SlotID)
REFERENCES dbo.CharacterEquipmentSlotCode (SlotID)
GO


-- 케릭터를 미리 선별해뒀던 테이블 삭제
DROP TABLE tmpCharacter

	
/*
	-- 아래의 불필요한 데이터는 추후에 지워주는 것을 추천합니다.
	
	ALTER TABLE Character DROP COLUMN head_slot;
	ALTER TABLE Character DROP COLUMN chest_slot;
	ALTER TABLE Character DROP COLUMN legs_slot;
	ALTER TABLE Character DROP COLUMN hands_slot;
	ALTER TABLE Character DROP COLUMN feet_slot;
	ALTER TABLE Character DROP COLUMN fingerl_slot;
	ALTER TABLE Character DROP COLUMN fingerr_slot;
	ALTER TABLE Character DROP COLUMN melee_slot;
	ALTER TABLE Character DROP COLUMN primary_slot;
	ALTER TABLE Character DROP COLUMN secondary_slot;
	ALTER TABLE Character DROP COLUMN custom1_slot;
	ALTER TABLE Character DROP COLUMN custom2_slot;
		
	ALTER TABLE Character DROP COLUMN head_ItemID;
	ALTER TABLE Character DROP COLUMN chest_ItemID;
	ALTER TABLE Character DROP COLUMN legs_ItemID;
	ALTER TABLE Character DROP COLUMN hands_ItemID;
	ALTER TABLE Character DROP COLUMN feet_ItemID;
	ALTER TABLE Character DROP COLUMN fingerl_ItemID;
	ALTER TABLE Character DROP COLUMN fingerr_ItemID;
	ALTER TABLE Character DROP COLUMN melee_ItemID;
	ALTER TABLE Character DROP COLUMN primary_ItemID;
	ALTER TABLE Character DROP COLUMN secondary_ItemID;
	ALTER TABLE Character DROP COLUMN custom1_ItemID;
	ALTER TABLE Character DROP COLUMN custom2_ItemID;
*/