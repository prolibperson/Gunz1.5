USE IGunzDB
GO

ALTER PROC dbo.spDeleteChar
    @AID  INT  
    , @CharNum SMALLINT  
    , @CharName VARCHAR(24)  
AS BEGIN    

    SET NOCOUNT ON  

    DECLARE @CID  INT  

    SELECT  @CID = CID   
    FROM    Character WITH (nolock)   
    WHERE   AID = @AID 
    AND     CharNum = @CharNum        

    IF (@CID IS NULL) BEGIN
        RETURN (-1)        
    END        

    IF EXISTS(SELECT * FROM CharacterItem(NOLOCK) WHERE CID = @CID AND ItemID >= 500000) BEGIN    
        RETURN (-1)    
    END    

    IF EXISTS(SELECT * FROM Clan(NOLOCK) WHERE MasterCID = @CID) BEGIN    
        RETURN (-1)    
    END    

    IF EXISTS(SELECT * FROM ClanMember(NOLOCK) WHERE CID = @CID) BEGIN    
        RETURN (-1)    
    END    


    BEGIN TRAN ---------  

        UPDATE  Character   
        SET     CharNum = -1, DeleteFlag = 1, DeleteDate = GETDATE(),
                Name = '', DeleteName = @CharName        
        WHERE   AID = @AID   
        AND     CharNum = @CharNum   
        AND     Name = @CharName  

        IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN      
            ROLLBACK TRAN      
            RETURN (-1)      
        END      


        INSERT INTO CharacterMakingLog(AID, CharName, Type, Date)      
        VALUES(@AID, @CharName, 'ªË¡¶', GETDATE())      

        IF (0 <> @@ERROR) OR (0 = @@ROWCOUNT) BEGIN      
            ROLLBACK TRAN      
            RETURN (-1)       
        END      


        UPDATE  Friend    
        SET     DeleteFlag = 1    
        WHERE   CID = @CID OR FriendCID = @CID    

        IF (0 <> @@ERROR) BEGIN    
            ROLLBACK TRAN    
            RETURN (-1)    
        END    


        UPDATE  SurvivalCharacterInfo  
        SET     DeleteFlag = 1  
        WHERE   CID = @CID  

        IF (0 <> @@ERROR) BEGIN    
            ROLLBACK TRAN    
            RETURN (-1)    
        END  

    COMMIT TRAN ---------  

    SELECT 1 AS Ret        
END    

