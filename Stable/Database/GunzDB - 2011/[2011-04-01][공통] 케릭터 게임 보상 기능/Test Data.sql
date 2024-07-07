--------------------------------------------------------------------------------------------------------------------------------------------

EXEC spInsertBattleTimeRewardDescription '가정의 달 선물상자 - 자동 보상', '2011-04-26 00:00:00.0', '2011-04-29 00:00:00.0', 5, 20, 0, '1111111', '매주 월요일 새벽 3시에 리셋됩니다.', 1;
EXEC spInsertBattleTimeRewardDescription '그냥 자동 보상(테스트 - Type 1)', '2011-04-26 00:00:00.0', '2011-04-29 00:00:00.0', 1, 0, 0, '0000000', '무한대로 그냥 받는 이벤트입니다.', 1;
EXEC spInsertBattleTimeRewardDescription '그냥 자동 보상(테스트 - Type 2)', '2011-04-26 00:00:00.0', '9999-12-31 00:00:00.0', 5, 0, 0, '1111111', '매일 무한대로 그냥 받는 이벤트입니다.', 1;


--------------------------------------------------------------------------------------------------------------------------------------------

INSERT dbo.BattleTimeRewardItemList(BRID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1, 1001005, 1001005, 0, 1000)

INSERT dbo.BattleTimeRewardItemList(BRID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (2, 1001001, 1001001, 0, 400)
INSERT dbo.BattleTimeRewardItemList(BRID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (2, 1, 1, 0, 200)
INSERT dbo.BattleTimeRewardItemList(BRID, ItemIDMale, ItemIDFemale, RentHourPeriod, ItemCnt, RatePerThousand) VALUES (2, 40103, 40103, 0, 100, 400)

INSERT dbo.BattleTimeRewardItemList(BRID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (3, 1001002, 1001002, 0, 1000)


/*
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 200 WHERE BRIID = 1;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 200 WHERE BRIID = 2;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 200 WHERE BRIID = 3;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 200 WHERE BRIID = 4;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 200 WHERE BRIID = 5;

UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 50 WHERE BRIID = 1;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 200 WHERE BRIID = 2;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 300 WHERE BRIID = 3;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 400 WHERE BRIID = 4;
UPDATE dbo.BattletimeRewardItem SET RatePerThousand = 50 WHERE BRIID = 5;
*/

/*
SELECT o.*, m.*
  FROM sys.objects o
  JOIN sys.sql_modules m
    ON o.object_id = m.object_id
 WHERE o.type = 'P';
*/