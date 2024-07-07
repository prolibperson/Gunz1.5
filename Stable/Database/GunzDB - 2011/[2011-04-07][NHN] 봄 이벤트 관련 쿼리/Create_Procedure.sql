USE IGunzDB
GO

------------------------------------------------------------------------------------------------------
/*
    -- 기존 프로시저 삭제
    DROP PROC dbo.spEventSpring_GetAcountEasterTickets    
    DROP PROC dbo.spEventSpring_GetEasterEggCount;    
    DROP PROC dbo.spEventSpring_RemoveEasterEgg;    
*/

------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spEventSpring_GetAcountEasterTickets
-- ALTER PROC dbo.spEventSpring_GetAcountEasterTickets
    @UserID     VARCHAR(24)
    , @ItemID   INT
    , @ItemCnt  INT OUTPUT 
AS BEGIN

    SET NOCOUNT ON;

    IF( @ItemID < 701001 OR @ItemID > 701004 ) RETURN;
    
    SET @ItemCnt = 0;

    SELECT  @ItemCnt = SUM(R.ItemCnt)
    FROM
            (
                SELECT  CI.ItemID, COUNT(CI.ItemID) AS ItemCnt
                FROM    dbo.Account A(NOLOCK) 
                        JOIN dbo.Character C(NOLOCK)
                        ON A.AID = C.AID
                        JOIN dbo.CharacterItem CI(NOLOCK) 
                        ON C.CID = CI.CID
                WHERE   A.UserID = @UserID
                AND     CI.ItemID = @ItemID
                AND     C.DeleteFlag = 0
                GROUP BY CI.ItemID

                UNION ALL

                SELECT  AI.ItemID, COUNT(AI.ItemID) AS ItemCnt
                FROM    dbo.Account A(NOLOCK) 
                        JOIN dbo.AccountItem AI(NOLOCK) 
                        ON A.AID = AI.AID
                WHERE   A.UserID = @UserID
                AND     AI.ItemID = @ItemID
                GROUP BY AI.ItemID
            ) R
    GROUP BY ItemID

END
GO

------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spEventSpring_GetEasterEggCount
-- ALTER PROC dbo.spEventSpring_GetEasterEggCount
    @UserID VARCHAR(24)
AS BEGIN

    SET NOCOUNT ON;

    SELECT  R.ItemID, SUM(R.ItemCnt) AS ItemCnt
    FROM
            (
                SELECT  CI.ItemID, COUNT(CI.ItemID) AS ItemCnt
                FROM    dbo.Account A(NOLOCK) 
                        JOIN dbo.Character C(NOLOCK)
                        ON A.AID = C.AID
                        JOIN dbo.CharacterItem CI(NOLOCK) 
                        ON C.CID = CI.CID
                WHERE   A.UserID = @UserID
                AND     CI.ItemID BETWEEN 701001 AND 701004
                AND     C.DeleteFlag = 0
                GROUP BY CI.ItemID

                UNION ALL

                SELECT  AI.ItemID, COUNT(AI.ItemID) AS ItemCnt
                FROM    dbo.Account A(NOLOCK) 
                        JOIN dbo.AccountItem AI(NOLOCK) 
                        ON A.AID = AI.AID
                WHERE   A.UserID = @UserID
                AND     AI.ItemID BETWEEN 701001 AND 701004
                GROUP BY AI.ItemID
            ) R
    GROUP BY R.ItemID
    
END
GO

------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spEventSpring_RemoveEasterEgg
-- ALTER PROC dbo.spEventSpring_RemoveEasterEgg
    @UserID     VARCHAR(24)
    , @ItemID   INT
    , @ItemCnt  INT
AS BEGIN

    SET NOCOUNT ON;

    ------------------------------------------------------------------------------------

    IF( @ItemID < 701001 OR @ItemID > 701004 ) BEGIN
        SELECT -1 AS 'Ret';
        RETURN;
    END

    ------------------------------------------------------------------------------------

    DECLARE @UserItemCnt INT;
    SET @UserItemCnt = 0;
    
    SELECT  @UserItemCnt = SUM(R.ItemCnt)
    FROM
            (
                SELECT  CI.ItemID, COUNT(CI.ItemID) AS ItemCnt
                FROM    dbo.Account A(NOLOCK) 
                        JOIN dbo.Character C(NOLOCK)
                        ON A.AID = C.AID
                        JOIN dbo.CharacterItem CI(NOLOCK) 
                        ON C.CID = CI.CID
                WHERE   A.UserID = @UserID
                AND     CI.ItemID = @ItemID
                AND     C.DeleteFlag = 0
                GROUP BY CI.ItemID

                UNION ALL

                SELECT  AI.ItemID, COUNT(AI.ItemID) AS ItemCnt
                FROM    dbo.Account A(NOLOCK) 
                        JOIN dbo.AccountItem AI(NOLOCK) 
                        ON A.AID = AI.AID
                WHERE   A.UserID = @UserID
                AND     AI.ItemID = @ItemID
                GROUP BY AI.ItemID
            ) R
    GROUP BY ItemID

    IF( @UserItemCnt < @ItemCnt ) BEGIN
        SELECT -2 AS 'Ret';
        RETURN;
    END

    ------------------------------------------------------------------------------------

    BEGIN TRAN -----------

        DECLARE @SQLString NVARCHAR(MAX);
        DECLARE @DeletedCnt INT;

        SET @SQLString =                N'DELETE  AccountItem ';
        SET @SQLString = @SQLString +   N'WHERE   AIID IN ';
        SET @SQLString = @SQLString +   N'(';
        SET @SQLString = @SQLString +   N'  SELECT  TOP ' + CAST(@ItemCnt AS VARCHAR(MAX)) + ' AI.AIID';
        SET @SQLString = @SQLString +   N'  FROM    Account A(NOLOCK) JOIN AccountItem AI(NOLOCK) ON A.AID = AI.AID';
        SET @SQLString = @SQLString +   N'  WHERE   A.UserID = ''' + @UserID + ''' AND AI.ItemID = ' + CAST(@ItemID AS VARCHAR(MAX));
        SET @SQLString = @SQLString +   N')';

        EXEC SP_EXECUTESQL @SQLString;
        SET @DeletedCnt = @@ROWCOUNT;
        
        IF( @@ERROR <> 0 ) BEGIN
            ROLLBACK TRAN;
			SELECT -3 AS 'Ret';
			RETURN;
        END

        -- 더 지울 것이 남았다면 CharacterItem 테이블 쪽을 지우자.        
        IF( @ItemCnt - @DeletedCnt > 0 )
        BEGIN

            SET @SQLString =                N'DELETE  CharacterItem ';
            SET @SQLString = @SQLString +   N'WHERE   CIID IN ';
            SET @SQLString = @SQLString +   N'(';
            SET @SQLString = @SQLString +   N'  SELECT  TOP ' + CAST((@ItemCnt - @DeletedCnt) AS VARCHAR(MAX)) + ' CI.CIID';
            SET @SQLString = @SQLString +   N'  FROM    dbo.Character C(NOLOCK) JOIN Account A(NOLOCK) ON C.AID = A.AID';
            SET @SQLString = @SQLString +   N'          JOIN CharacterItem CI(NOLOCK) ON C.CID = CI.CID';
            SET @SQLString = @SQLString +   N'  WHERE   A.UserID = ''' + @UserID + ''' AND C.DeleteFlag = 0 AND CI.ItemID = ' + CAST(@ItemID AS VARCHAR(MAX));
            SET @SQLString = @SQLString +   N')';
            
            -- SELECT @SQLString;
            EXEC SP_EXECUTESQL @SQLString;

            IF( @@ERROR <> 0 ) BEGIN
                ROLLBACK TRAN;
			    SELECT -4 AS 'Ret';
			    RETURN;
            END
        END
        
    COMMIT TRAN ----------
    
    ------------------------------------------------------------------------------------

    SELECT 0 AS 'Ret';

END
GO
