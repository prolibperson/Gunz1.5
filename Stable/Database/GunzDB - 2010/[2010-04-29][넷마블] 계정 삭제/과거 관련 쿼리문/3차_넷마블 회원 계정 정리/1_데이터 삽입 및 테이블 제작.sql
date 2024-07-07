USE HGunzDB

-- BULK INDEX로 데이터가 삽입될 테이블
CREATE TABLE dbo.[NetmarbleAccountInfo_20100217](
	UserID		NVARCHAR(50) 	NOT NULL,
	AID			NVARCHAR(50) 	NULL,
	UserCN		NVARCHAR(50) 	NULL,
	IsUser		CHAR(1)			NULL,
	IsBreakUser	CHAR(1)			NULL,
	ValidUserCN	NVARCHAR(50)	NULL,
	RegDate		DATETIME		NULL	
	
	CONSTRAINT NetmarbleAccountInfo_20100224_PK PRIMARY KEY CLUSTERED (UserID ASC)
) ON [PRIMARY]

DECLARE @SQL NVARCHAR(4000)
SET @SQL = 'BULK INSERT dbo.NetmarbleAccountInfo_20100217 FROM ''D:\GunzResult_100224.TXT'' '
SET @SQL = @SQL + 'WITH(FIELDTERMINATOR = ''' + CHAR(28) + ''', ROWTERMINATOR = ''\n'', '
SET @SQL = @SQL +' rows_per_batch=10000, MAXERRORS = 1000, KEEPNULLS, tablock )'
SELECT @SQL

EXEC (@SQL) 

============================================================================================

-- INSERT NetmarbleAccountInfo_20100224 VALUES('@1onia',  '2705147', 'NM0030921525', NULL, NULL, NULL, NULL)

-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na																							--		5967790개

-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser = 'Y' AND na.IsBreakUser IS NULL											--		4515549개
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser = 'Y' AND na.IsBreakUser IS NULL AND na.ValidUserCN IS NULL			--  4515549개
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser = 'Y' AND na.IsBreakUser IS NULL AND na.ValidUserCN IS NOT NULL		--		  0개
	
-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsBreakUser = 'Y' AND na.IsUser IS NULL											--		1214985개 
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsBreakUser = 'Y' AND na.IsUser IS NULL AND na.ValidUserCN IS NULL			--	 928194개
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsBreakUser = 'Y' AND na.IsUser IS NULL AND na.ValidUserCN IS NOT NULL		--	 286791개

-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser IS NULL AND na.IsBreakUser IS NULL											--		 237106개  
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser IS NULL AND na.IsBreakUser IS NULL AND na.ValidUserCN IS NULL			--	 153325개
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser IS NULL AND na.IsBreakUser IS NULL AND na.ValidUserCN IS NOT NULL		--	  83781개

-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser IS NOT NULL AND na.IsBreakUser IS NOT NULL										--		150개	
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser IS NOT NULL AND na.IsBreakUser IS NOT NULL	AND na.ValidUserCN IS NULL		--	150개	
	-- SELECT COUNT(*) FROM NetmarbleAccountInfo_20100224 na WHERE na.IsUser IS NOT NULL AND na.IsBreakUser IS NOT NULL	AND na.ValidUserCN IS NOT NULL	--	  0개
	
============================================================================================

-- 삭제 되야될 ID를 임시로 저장하는 테이블
CREATE TABLE NetmarbleTmpForRemove
(
	UserID				VARCHAR(20),
	DEL_ID				VARCHAR(20),
	AID					INT,
	LoginUpdated		TINYINT,
	AccountUpdated		TINYINT

	CONSTRAINT NetmarbleTmpForRemove_PK PRIMARY KEY CLUSTERED (UserID)
)

-- 갱신 되야될 ID를 임시로 저장하는 테이블
CREATE TABLE NetmarbleTmpForUpdate
(
	UserID				VARCHAR(20),
	AID					INT,
	UserCN				VARCHAR(20),
	ValidUserCN			VARCHAR(20),
	LoginUpdated		TINYINT,
	AccountUpdated		TINYINT
	
	CONSTRAINT NetmarbleTmpForUpdate_PK PRIMARY KEY CLUSTERED (UserID)
)

SELECT TOP 100 * FROM NetmarbleAccountInfo_20100224 
============================================================================================
INSERT NetmarbleTmpForRemove	
	SELECT l.UserID, CAST( CAST(NEWID() AS VARCHAR(40)) AS VARCHAR(20)), l.AID, 0, 0
	FROM
	(
		--SELECT a.AID
		SELECT na.*
		FROM Netma_Account a JOIN NetmarbleAccountInfo_20100224 na ON na.AID = a.AID AND na.UserID = a.UserID
		WHERE na.IsBreakUser IS NULL AND na.IsUser IS NULL AND na.ValidUserCN IS NULL AND na.RegDate IS NULL 
		-- 153277 이 놈들은 뭔가 이상하다. 로그인 시간도 없고, 로그 아웃 시간도 없다.
		
		SELECT na.*
		FROM Netma_Account a JOIN NetmarbleAccountInfo_20100224 na ON na.AID = a.AID AND na.UserID = a.UserID
		WHERE na.IsBreakUser IS NULL AND na.IsUser IS NULL AND na.ValidUserCN IS NULL AND na.RegDate IS NULL
			AND a.LastLoginTime IS NULL -- 153277
			
		SELECT na.*
		FROM Netma_Account a JOIN NetmarbleAccountInfo_20100224 na ON na.AID = a.AID AND na.UserID = a.UserID
		WHERE na.IsBreakUser IS NULL AND na.IsUser IS NULL AND na.ValidUserCN IS NULL AND na.RegDate IS NULL
			AND a.LastLoginTime IS NULL AND a.LastLogoutTime IS NULL -- 144937
			
	) r, Netma_Login l
	WHERE r.AID = l.AID
	-- 결과 : 928176개(결과셋의 수는 실서버와 다를 수 있습니다) => AID와 UserID로 비교

-- 일단 탈퇴가 명확한 회원들 집어넣어주자 - Type 1
INSERT NetmarbleTmpForRemove	
	SELECT l.UserID, CAST( CAST(NEWID() AS VARCHAR(40)) AS VARCHAR(20)), l.AID, 0, 0
	FROM
	(
		SELECT a.AID
		FROM Netma_Account a JOIN NetmarbleAccountInfo_20100224 na ON na.AID = a.AID AND na.UserID = a.UserID
		WHERE na.IsBreakUser = 'Y' AND na.RegDate IS NULL 
	) r, Netma_Login l
	WHERE r.AID = l.AID
	-- 결과 : 928176개(결과셋의 수는 실서버와 다를 수 있습니다) => AID와 UserID로 비교

	
-- 일단 탈퇴가 명확한 회원들 집어넣어주자 - Type 2
INSERT NetmarbleTmpForRemove
	SELECT l.UserID, CAST( CAST(NEWID() AS VARCHAR(40)) AS VARCHAR(20)), l.AID, 0, 0
	FROM
	(
		SELECT a.AID
		FROM Netma_Account a JOIN NetmarbleAccountInfo_20100224 na ON na.AID = a.AID AND na.UserID = a.UserID
		WHERE na.RegDate IS NOT NULL AND na.RegDate > a.LastLogoutTime
	) r, Netma_Login l
	WHERE r.AID = l.AID	
	-- 결과 : 69796개(결과셋의 수는 실서버와 다를 수 있습니다) => AID와 UserID로 비교

============================================================================================	

-- 갱신 해줘야될 회원들을 집어넣어주자
INSERT NetmarbleTmpForUpdate
	SELECT l.UserID, l.AID, r.UserCN, r.ValidUserCN, 0, 0
	FROM
	(
		SELECT a.AID, na.UserCN, na.ValidUserCN
		FROM Netma_Account a JOIN NetmarbleAccountInfo_20100224 na ON na.AID = a.AID AND na.UserID = a.UserID
		WHERE na.RegDate IS NOT NULL AND na.RegDate < a.LastLogoutTime -- AND l.LastLogoutTime < '2010-01-25 10:00:00.0'
	) r, Netma_Login l
	WHERE r.AID = l.AID	
	-- 결과 : 58872개, 18초(결과셋의 수는 실서버와 다를 수 있습니다)
	

============================================================================================
	
CREATE NONCLUSTERED INDEX NetmarbleTmpForRemove_AID_IX	ON NetmarbleTmpForRemove(AID);
CREATE NONCLUSTERED INDEX NetmarbleTmpForUpdate_AID_IX	ON NetmarbleTmpForUpdate(AID);