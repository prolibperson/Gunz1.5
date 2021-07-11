CREATE PROC spBringAccountItem2  
 @AID  int,  
 @CID  int,  
 @AIID  int  
AS  
SET NoCount On  
  
DECLARE @ItemID int  
DECLARE @CAID int  
DECLARE @OrderCIID int  
  
DECLARE @RentDate   DATETIME  
DECLARE @RentHourPeriod  SMALLINT  
DECLARE @Cnt    SMALLINT  
  
SELECT @ItemID=ItemID, @RentDate=RentDate, @RentHourPeriod=RentHourPeriod, @Cnt=Cnt  
FROM AccountItem WHERE AIID = @AIID  
  
  
SELECT @CAID = AID FROM Character WHERE CID=@CID  
  
IF @ItemID IS NOT NULL AND @CAID = @AID  
BEGIN  
 BEGIN TRAN ----------------  
 DELETE FROM AccountItem WHERE AIID = @AIID  
 IF 0 <> @@ERROR BEGIN  
  ROLLBACK TRAN   
  RETURN  
 END  
  
 INSERT INTO CharacterItem (CID, ItemID, RegDate, RentDate, RentHourPeriod, Cnt)  
 VALUES (@CID, @ItemID, GETDATE(), @RentDate, @RentHourPeriod, @Cnt)  
 IF 0 <> @@ERROR BEGIN   
  ROLLBACK TRAN  
  RETURN   
 END  
  
 SET @OrderCIID = @@IDENTITY  
  
 INSERT INTO BringAccountItemLog (ItemID, AID, CID, Date)  
 VALUES (@ItemID, @AID, @CID, GETDATE())  
 IF 0 <> @@ERROR BEGIN  
  ROLLBACK TRAN  
  RETURN  
 END  
  
 COMMIT TRAN ---------------  
  
 SELECT @OrderCIID AS ORDERCIID, @ItemID AS ItemID
  , @RentHourPeriod as 'RentHourPeriod'
  , (@RentHourPeriod*60) - (DateDiff(n, @RentDate, GETDATE())) AS RentPeriodRemainder  
END  
  
  