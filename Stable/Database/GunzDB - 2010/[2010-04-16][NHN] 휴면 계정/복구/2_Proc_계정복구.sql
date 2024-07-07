USE GunzDB
GO
  
CREATE PROC dbo.spWebRestoreSleepAccount_NHN  
    @UserID VARCHAR(24)  
AS BEGIN  
    SET NOCOUNT ON;   

    DECLARE @AID   INT;    
    DECLARE @HackingType INT;  
    DECLARE @RegDate  DATETIME;  

    SELECT  @AID = al.AID, @RegDate = al.RegDt  
    FROM    SleepAccountNHN al   
    WHERE   al.UserID = @UserID;  

    IF( @AID IS NULL ) BEGIN
        SELECT -1 AS 'Ret';  -- UserID is Not Sleeped   
        RETURN;
    END

    BEGIN TRAN --------  

    UPDATE  Account   
    SET     HackingType = NULL   
    WHERE   AID = @AID 
    AND     HackingType = 10;  

    -- Update HackingType Field FAIL!  
    IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
        ROLLBACK TRAN       
        SELECT -2 AS 'Ret';  
        RETURN;
    END  

    
    DELETE  SleepAccountNHN   
    WHERE   AID = @AID;  

    -- Delete SleepAccountNHN FAIL!  
    IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
        ROLLBACK TRAN       
        SELECT -3 AS 'Ret';  
        RETURN;
    END  


    INSERT SleepAccountRestoredLogNHN(AID, UserID, RegDt, RestoredDt)  
    VALUES(@AID, @UserID, @RegDate, GETDATE());
  
    -- Insert SleepAccountRestoredLogNHN FAIL!  
    IF( 0 = @@ROWCOUNT OR 0 <> @@ERROR ) BEGIN      
        ROLLBACK TRAN       
        SELECT -4 AS 'Ret'; 
        RETURN; 
    END  

    COMMIT TRAN -------  

    SELECT 0 AS 'Ret'
END