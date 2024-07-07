------------------------------------------------------------------------------------------------------
-- Test Data
------------------------------------------------------------------------------------------------------

DELETE AccountItem WHERE ItemID BETWEEN 701001 AND 701004 AND AID = 1101
DELETE CharacterItem WHERE ItemID BETWEEN 701001 AND 701004 AND CID = 1271

INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701001, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701001, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701001, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701001, GETDATE(), 0, 1)

INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701002, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701002, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701002, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701002, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701002, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701002, GETDATE(), 0, 1)

INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701003, GETDATE(), 0, 1)

INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701004, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701004, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701004, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701004, GETDATE(), 0, 1)
INSERT AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt) VALUES (1101, 701004, GETDATE(), 0, 1)


INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)

INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701001, GETDATE(), GETDATE(), 0, 1)

INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701002, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701002, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701002, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701002, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701002, GETDATE(), GETDATE(), 0, 1)

INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701003, GETDATE(), GETDATE(), 0, 1)

INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)
INSERT CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt) VALUES (1271, 701004, GETDATE(), GETDATE(), 0, 1)



------------------------------------------------------------------------------------------------------
/*
DECLARE @ItemCnt INT;
EXEC spEventSpring_GetAcountEasterTickets 'c1', 701001, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701002, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701003, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701004, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetEasterEggCount 'c1'

EXEC spEventSpring_RemoveEasterEgg 'c1', 701001, 9
EXEC spEventSpring_RemoveEasterEgg 'c1', 701002, 8
EXEC spEventSpring_RemoveEasterEgg 'c1', 701003, 15
EXEC spEventSpring_RemoveEasterEgg 'c1', 701004, 12

EXEC spEventSpring_GetEasterEggCount 'c1'

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701001, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701002, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701003, @ItemCnt OUTPUT
SELECT @ItemCnt;

EXEC spEventSpring_GetAcountEasterTickets 'c1', 701004, @ItemCnt OUTPUT
SELECT @ItemCnt;
*/
------------------------------------------------------------------------------------------------------

