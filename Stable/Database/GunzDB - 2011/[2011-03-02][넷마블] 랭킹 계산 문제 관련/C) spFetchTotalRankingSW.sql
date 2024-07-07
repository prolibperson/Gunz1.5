USE GunzDB
GO
  
CREATE PROC dbo.spFetchTotalRankingSW  
-- ALTER PROC dbo.spFetchTotalRankingSW  
AS BEGIN  
  
    ------------------------------------------------------------------------------------------------------------------------  
  
    CREATE TABLE dbo.TotalRankingSW  
    (  
        Rank            INT         NOT NULL  
        , UserID        VARCHAR(20) NOT NULL  
        , Name          VARCHAR(24) NOT NULL  
        , Level         SMALLINT    NOT NULL  
        , XP            INT         NOT NULL  
        , KillCount     INT         NOT NULL  
        , DeathCount    INT         NOT NULL  
    );  
  
    ------------------------------------------------------------------------------------------------------------------------  
  
    DECLARE @UNIQUE_ID          NVARCHAR(MAX);  
    DECLARE @Query_PK_RANK      NVARCHAR(MAX);  
    DECLARE @Query_IX_UserID    NVARCHAR(MAX);  
    DECLARE @Query_IX_Name      NVARCHAR(MAX);  
  
    SET @UNIQUE_ID = CAST( CAST(NEWID() AS VARCHAR(40)) AS VARCHAR(8));  
    SET @Query_PK_RANK = N'ALTER TABLE TotalRankingSW ADD CONSTRAINT PK_TotalRanking_' + @UNIQUE_ID + ' PRIMARY KEY CLUSTERED (Rank)';  
    EXEC SP_EXECUTESQL @Query_PK_RANK; 
      
    INSERT dbo.TotalRankingSW(Rank, UserID, Name, Level, XP, KillCount, DeathCount)          
        SELECT  ROW_NUMBER() OVER (ORDER BY c.XP DESC, c.KillCount DESC, c.DeathCount ASC, c.PlayTime DESC) AS Rank  
                , a.UserID, c.name, c.Level, c.XP, c.KillCount, c.DeathCount      
        FROM    Character c(NOLOCK)   
                JOIN Account a(NOLOCK)      
                ON a.AID = c.AID  
        WHERE   c.DeleteFlag = 0   
        AND     c.XP >= 500  
      
    SET @Query_IX_UserID = N'CREATE NONCLUSTERED INDEX IX_TotalRanking_' + @UNIQUE_ID + '_UserID ON TotalRankingSW(UserID) WITH (MAXDOP = 1)';  
    SET @Query_IX_Name   = N'CREATE NONCLUSTERED INDEX IX_TotalRanking_' + @UNIQUE_ID + '_Name ON TotalRankingSW(Name) WITH (MAXDOP = 1)';  
  
    EXEC SP_EXECUTESQL @Query_IX_UserID;  
    EXEC SP_EXECUTESQL @Query_IX_Name;  
  
    ------------------------------------------------------------------------------------------------------------------------  
  
    DROP TABLE dbo.TotalRanking;  
    EXEC SP_RENAME 'TotalRankingSW', 'TotalRanking'  
  
    ------------------------------------------------------------------------------------------------------------------------  
END  