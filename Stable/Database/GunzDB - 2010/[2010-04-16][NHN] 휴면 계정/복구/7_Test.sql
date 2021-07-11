SELECT * FROM dbo.SleepAccountNHN WHERE AID = 1076
SELECT * FROM dbo.SleepCharacterNHN WHERE AID = 1076

SELECT * FROM dbo.SleepAccountRestoredLogNHN
SELECT * FROM dbo.SleepCharacterRestoredLogNHN

EXEC spWebRestoreSleepAccount_NHN 'tigerjk'

EXEC spWebGetActiveCharacterList_NHN	1076
EXEC spWebGetSleepCharacterList_NHN		1076


EXEC spWebRestoreSleepCharacter_NHN CID, CharNum
EXEC spWebRestoreSleepCharacter_NHN 1236, 0 

EXEC spWebRestoreSleepCharacterNewName_NHN CID, CharNum, NewName
EXEC spWebRestoreSleepCharacterNewName_NHN 1267, 3, '¤¡¤²¤¸¤§2'