EXEC spRemoveBreakCharacter_Netmarble '2011-02-01 00:00:00.0'

ALTER PROCEDURE dbo.spRemoveBreakCharacter_Netmarble
     @TargetDate DATETIME
AS BEGIN

    SET NOCOUNT ON;

    IF( NOT (OBJECT_ID('#tmpDelChar') IS NULL) )
        DROP TABLE #tmpDelChar      

    SELECT  c.CID, c.AID
    INTO    #tmpDelChar    
    FROM    dbo.AccountDeleteLog al(NOLOCK)
            JOIN dbo.Character c(NOLOCK)
            ON al.AID = c.AID    
    WHERE   al.DeleteDate BETWEEN CONVERT(CHAR(10), @TargetDate, 120) AND CONVERT(CHAR(10), @TargetDate + 1, 120)
    AND     c.DeleteFlag = 0

    ----------------------------------------------------------------------------------------------------------------
        
    UPDATE  f
    SET     f.DeleteFlag = 1  
    FROM    dbo.Friend f, #tmpDelChar t
    WHERE   f.CID = t.CID
    OPTION (MAXDOP 1)

    UPDATE  f
    SET     f.DeleteFlag = 1  
    FROM    dbo.Friend f, #tmpDelChar t
    WHERE   f.FriendCID = t.CID
    OPTION (MAXDOP 1)

    ----------------------------------------------------------------------------------------------------------------

    UPDATE  c
    SET     c.DeleteDate = GETDATE(), c.DeleteFlag = 1, c.DeleteName = c.Name, 
            c.Name = '', c.CharNum = -1
    FROM    #tmpDelChar t, dbo.Character c
    WHERE   t.CID = c.CID

    ----------------------------------------------------------------------------------------------------------------

    INSERT INTO dbo.CharacterMakingLog(AID, CharName, Type, Date)    
        SELECT  c.AID, c.Name, 'ªË¡¶', GETDATE()
        FROM    #tmpDelChar t JOIN Character c ON c.CID = t.CID

END

