USE IGunzDB
GO

CREATE PROCEDURE dbo.spRegularRemoveBreakCharacter_Netmarble
-- ALTER PROCEDURE dbo.spRegularRemoveBreakCharacter_Netmarble
AS BEGIN

    SET NOCOUNT ON;

    IF( NOT (OBJECT_ID('#tmpDelCharList') IS NULL) )
        DROP TABLE #tmpDelCharList

    SELECT  TOP 100 c.CID, c.AID
    INTO    #tmpDelCharList
    FROM    dbo.AccountDeleteLog al(NOLOCK)
            JOIN dbo.Character c(NOLOCK)
            ON al.AID = c.AID    
    WHERE   c.DeleteFlag = 0

    ----------------------------------------------------------------------------------------------------------------
        
    UPDATE  f
    SET     f.DeleteFlag = 1  
    FROM    dbo.Friend f, #tmpDelCharList t
    WHERE   f.CID = t.CID
    OPTION (MAXDOP 1)

    UPDATE  f
    SET     f.DeleteFlag = 1  
    FROM    dbo.Friend f, #tmpDelCharList t
    WHERE   f.FriendCID = t.CID
    OPTION (MAXDOP 1)

    ----------------------------------------------------------------------------------------------------------------

    UPDATE  c
    SET     c.DeleteDate = GETDATE(), c.DeleteFlag = 1, c.DeleteName = c.Name, 
            c.Name = '', c.CharNum = -1
    FROM    #tmpDelCharList t, dbo.Character c
    WHERE   t.CID = c.CID

    ----------------------------------------------------------------------------------------------------------------

    INSERT INTO dbo.CharacterMakingLog(AID, CharName, Type, Date)    
        SELECT  c.AID, c.Name, 'ªË¡¶', GETDATE()
        FROM    #tmpDelCharList t JOIN Character c ON c.CID = t.CID

END

