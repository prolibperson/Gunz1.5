USE GunzDB
GO

CREATE PROC dbo.spFetchTotalRanking
AS BEGIN
        
    TRUNCATE TABLE dbo.TotalRanking    
      
    DROP INDEX TotalRanking.INDEX_TotalRanking_UserID  
    DROP INDEX TotalRanking.INDEX_TotalRanking_Name  
        
    INSERT TotalRanking(UserID, Name, Level, XP, KillCount, DeathCount)        
        SELECT  a.UserID, c.name, c.Level, c.XP, c.KillCount, c.DeathCount    
        FROM    Character c(NOLOCK)
                JOIN Account a(NOLOCK)
                ON a.AID = c.AID
        WHERE   c.DeleteFlag = 0 
        AND     c.XP >= 500    
        ORDER BY c.xp DESC, c.KillCount DESC, c.DeathCount ASC, c.PlayTime DESC    
        
    CREATE NONCLUSTERED INDEX INDEX_TotalRanking_UserID ON dbo.TotalRanking(UserID) WITH (MAXDOP = 1)
    CREATE NONCLUSTERED INDEX INDEX_TotalRanking_Name ON dbo.TotalRanking(Name) WITH (MAXDOP = 1)

END