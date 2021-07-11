USE GunzDB
GO

INSERT INTO Item (ItemID, Name, ResSex, ResLevel, Slot, IsCashItem, Description) 
Values (1003101, 'Red Box', 3 , 0, 3, 1, '넷마블 5월 이벤트 - 이벤트용 아이템. 판매용이 아님')


INSERT INTO Item (ItemID, Name, ResSex, ResLevel, Slot, IsCashItem, Description) 
Values (1003102, 'Black Box', 3 , 0, 3, 1, '넷마블 5월 이벤트 - 이벤트용 아이템. 판매용이 아님')


INSERT INTO Item (ItemID, Name, ResSex, ResLevel, Slot, IsCashItem, Description) 
Values (1003103, 'Purple Box', 3 , 0, 3, 1, '넷마블 5월 이벤트 - 이벤트용 아이템. 판매용이 아님')
--------------------------------------------------------------------------------------------------------------------------------

INSERT dbo.GambleItem(GIID, Name, Price, StartDate, LifeTimeHour, IsCash, Opened, Description)
VALUES (1003101, 'Red Box', 800, '2011-05-03 00:00:00.0', 24 * 21, 1, 0, '넷마블 5월 이벤트 - 이벤트용 아이템. 판매용이 아님');

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003001, 1003001, 0, 40)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003002, 1003002, 0, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003003, 1003003, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003004, 1003004, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003005, 1003005, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003006, 1003006, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 1003007, 1003007, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 502003, 502003, 168, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 502010, 502010, 168, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 502002, 502002, 168, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 502001, 502001, 168, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 502004, 502004, 168, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003101, 502005, 502005, 168, 80)

--------------------------------------------------------------------------------------------------------------------------------

INSERT dbo.GambleItem(GIID, Name, Price, StartDate, LifeTimeHour, IsCash, Opened, Description)
VALUES (1003102, 'Black Box', 800, '2011-05-03 00:00:00.0', 24 * 21, 1, 0, '넷마블 5월 이벤트 - 이벤트용 아이템. 판매용이 아님');

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 509008, 509008, 0, 40)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 506005, 506005, 0, 40)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 526001, 526001, 720, 60)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 527002, 527002, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 528002, 528002, 720, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 506004, 506004, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 506002, 506002, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 506001, 506001, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 509008, 509008, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 504503, 504503, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 504502, 504502, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 504501, 504501, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 509003, 509003, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 509002, 509002, 168, 70)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003102, 509001, 509001, 168, 70)

--------------------------------------------------------------------------------------------------------------------------------

INSERT dbo.GambleItem(GIID, Name, Price, StartDate, LifeTimeHour, IsCash, Opened, Description)
VALUES (1003103, 'Purple Box', 800, '2011-05-03 00:00:00.0', 24 * 21, 1, 0, '넷마블 5월 이벤트 - 이벤트용 아이템. 판매용이 아님');

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405007, 405007, 0, 1)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405006, 405006, 0, 20)

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405005, 405005, 0, 60)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 525003, 525003, 168, 60)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 525002, 525002, 168, 60)

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405004, 405004, 0, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405003, 405003, 0, 80)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405002, 405002, 0, 90)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 405001, 405001, 0, 89)

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 525008, 525008, 168, 60)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 525007, 525007, 168, 60)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 525004, 525004, 168, 60)

INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600202, 600202, 168, 30)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600302, 600302, 168, 30)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600002, 600002, 168, 30)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600102, 600102, 168, 30)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600203, 600203, 168, 40)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600303, 600303, 168, 40)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600003, 600003, 168, 40)
INSERT dbo.GambleRewardItem(GIID, ItemIDMale, ItemIDFemale, RentHourPeriod, RatePerThousand) VALUES (1003103, 600103, 600103, 168, 40)

--------------------------------------------------------------------------------------------------------------------------------
