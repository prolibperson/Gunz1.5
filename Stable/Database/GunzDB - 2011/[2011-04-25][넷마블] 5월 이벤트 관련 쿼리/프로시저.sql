USE IGunzDB
GO

CREATE PROC dbo.spEventNetmarble_InsertBoxItem
-- ALTER PROC dbo.spEventNetmarble_InsertBoxItem
    @UserCN         VARCHAR(20)
    , @BoxID        INT
    , @ItemID       INT
    , @ItemPeriod   INT
AS BEGIN

    SET NOCOUNT ON;

    IF( @ItemID >= 1003101 AND @ItemID != 1003102 AND @ItemID != 1003103 ) BEGIN
        SELECT -6 AS 'Ret';
        RETURN;
    END


    DECLARE @AID    INT
            , @CID  INT;

    SELECT  TOP 1 @AID = C.AID, @CID = C.CID
    FROM    dbo.Login L(NOLOCK) 
            JOIN dbo.Character C(NOLOCK)
            ON L.AID = C.AID
    WHERE   L.Password = @UserCN
    AND     C.DeleteFlag = 0
    ORDER BY C.Level DESC;

    IF( @AID IS NULL ) BEGIN
        SELECT -1 AS 'Ret';
        RETURN;
    END

    IF( @CID IS NULL ) BEGIN
        SELECT -2 AS 'Ret';
        RETURN;
    END   

    BEGIN TRAN ----------
        
        INSERT dbo.AccountItem(AID, ItemID, RentDate, RentHourPeriod, Cnt)
        VALUES (@AID, @ItemID, GETDATE(), @ItemPeriod, 1)

        IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
			ROLLBACK TRAN
			SELECT -3 AS 'Ret'
			RETURN;
		END

        INSERT dbo.ItemPurchaseLogByCash(AID, ItemID, Date, RentHourPeriod, Cash)
        VALUES (@AID, @BoxID, GETDATE(), 0, 800)

        IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
			ROLLBACK TRAN
			SELECT -4 AS 'Ret'
			RETURN;
		END

        INSERT INTO LogDB.dbo.GambleLog(CID, GIID, RewardItemID, RegDate)
        VALUES (@CID, @BoxID, @ItemID, GETDATE());

        IF( 0 <> @@ERROR OR 0 = @@ROWCOUNT ) BEGIN      
			ROLLBACK TRAN
			SELECT -5 AS 'Ret'
			RETURN;
		END
        
    COMMIT TRAN ---------

    SELECT 0 AS 'Ret';
END
GO

---------------------------------------------------------------------------------------------------

CREATE PROC dbo.spEventNetmarble_GetCharInfoByUserCN
-- ALTER PROC dbo.spEventNetmarble_GetCharInfoByUserCN
    @UserCN         VARCHAR(20)
AS BEGIN

    SET NOCOUNT ON;

    IF( @UserCN = '' ) RETURN;

    SELECT  C.Name, C.Level
    FROM    (
                SELECT  TOP 1 L.AID
                FROM    dbo.Login L(NOLOCK) 
                WHERE   L.Password = @UserCN
            ) R1 
            JOIN dbo.Character C(NOLOCK)
            ON R1.AID = C.AID
    WHERE   C.DeleteFlag = 0
    ORDER BY C.Level DESC;

END
GO

