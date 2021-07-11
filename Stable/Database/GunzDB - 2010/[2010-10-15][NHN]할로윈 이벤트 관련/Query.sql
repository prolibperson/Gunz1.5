USE GunzDB
GO


------------------------------------------------------------------------------------
-- 최초 테이블 생성
CREATE TABLE dbo.Event_CharacterItem_Halloween
(
	UserID			NVARCHAR(20) NOT NULL
	, UpdatedDate	DATETIME	NOT NULL
	, Trick			INT
	, Treat			INT
)
GO

------------------------------------------------------------------------------------
-- 프로시저 생성 및 수정
CREATE PROCEDURE dbo.spGetHalloweenEventItemStatus
-- ALTER PROCEDURE dbo.spGetHalloweenEventItemStatus
	@UserID		NVARCHAR(24)
AS
BEGIN
	SET NOCOUNT ON;
	
	DECLARE @TrickItemID	INT;
	DECLARE @TreatItemID	INT;
	
	DECLARE @UpdatedDate	DATETIME;
	DECLARE @Trick			INT;
	DECLARE @Treat			INT;
	
	SELECT	@TrickItemID = 403010, @TreatItemID = 403011;
	
	SELECT	@UpdatedDate = e.UpdatedDate, @Trick = e.Trick, @Treat = e.Treat
	FROM	Event_CharacterItem_Halloween e
	WHERE	e.UserID = @UserID;

	IF( @UpdatedDate IS NULL OR DATEDIFF(n, @UpdatedDate, GETDATE()) > 5 )
	BEGIN
		SELECT	@Trick = COUNT(ci.ItemID)
		FROM	(	SELECT	c.CID, a.UserID
					FROM	Account a(NOLOCK), Character c(NOLOCK)
					WHERE	a.UserID = @UserID
					AND		a.AID = c.AID
					AND		c.DeleteFlag = 0 ) r
				, CharacterItem ci(NOLOCK)
		WHERE	ci.CID = r.CID
		AND		ci.ItemID = @TrickItemID
		GROUP BY ci.ItemID;
			
		SELECT	@Treat = COUNT(ci.ItemID)
		FROM	(	SELECT	c.CID, a.UserID
					FROM	Account a(NOLOCK), Character c(NOLOCK)
					WHERE	a.UserID = @UserID
					AND		a.AID = c.AID
					AND		c.DeleteFlag = 0 ) r
				, CharacterItem ci(NOLOCK)
		WHERE	ci.CID = r.CID
		AND		ci.ItemID = @TreatItemID
		GROUP BY ci.ItemID;
		
		IF( @UpdatedDate IS NULL ) BEGIN
			INSERT Event_CharacterItem_Halloween(UserID, UpdatedDate, Trick, Treat)
			VALUES(@UserID, GETDATE(), @Trick, @Treat);
		END 
		ELSE BEGIN
			UPDATE	Event_CharacterItem_Halloween
			SET		Trick = @Trick, Treat = @Treat, UpdatedDate = GETDATE()
			WHERE	UserID = @UserID
		END		
	END

	SELECT	CASE WHEN @Trick IS NULL THEN 0 ELSE @Trick END AS 'Trick'
			, CASE WHEN @Treat IS NULL THEN 0 ELSE @Treat END AS 'Treat';
END
GO

------------------------------------------------------------------------------------

