USE GunzDB
GO

-------------------------------------------------------------------------------------------
/**
    단계 1. 어카운트 테이블에 DeleteDate 컬럼 추가 및 세팅하기
*/
ALTER TABLE dbo.Account ADD DeleteDate DATETIME;
    
UPDATE  A
SET     A.DeleteDate = AL.DeleteDate
FROM    AccountDeleteLog AL(TABLOCK)
        JOIN Account A(TABLOCK)
        ON AL.AID = A.AID
WHERE   A.DelFlag = 1

/**
    단계 2. 어카운트 테이블에 걸려있는 모든 Foreign Key 죽이기
    주의) 아래 외에 또 다른 키가 있을 수도 있음(각 국가별로 조금씩 다름)
*/
EXEC SP_HELP ACCOUNT

ALTER TABLE dbo.AccountItem             DROP Account_테이블1_FK1
ALTER TABLE dbo.AccountPenaltyGMLog     DROP AccountPenaltyGMLog_Account_FK
ALTER TABLE dbo.AccountPenaltyPeriod    DROP AccountPenaltyPeriod_Account_FK1
ALTER TABLE dbo.BringAccountItemLog     DROP Account_BringAccountItemLog_FK1
ALTER TABLE dbo.Character               DROP FK_Character_Account
ALTER TABLE dbo.ItemPurchaseLogByCash   DROP Account_PurchaseLogByCash_FK1
ALTER TABLE dbo.PremiumAccountHistory   DROP Account_PremiumAccountHis
GO

-------------------------------------------------------------------------------------------

/**
    단계 3. 어카운트 테이블 및 인덱스 스위칭하기
*/
EXEC SP_RENAME 'Account', 'AccountOld';
EXEC SP_RENAME 'Account_PK', 'PK_AccountOld';
EXEC SP_RENAME 'FK_Account_PremiumGrade', 'FK_AccountOld_PremiumGrade'
EXEC SP_RENAME 'FK_Account_UserGrade', 'FK_AccountOld_UserGrade'

-------------------------------------------------------------------------------------------


/**
    단계 4. 새로운 어카운트 테이블 만들기
*/


CREATE TABLE [dbo].[Account]
(
	[AID]	                INT	IDENTITY(1, 1)	NOT NULL, 
	[UserID]	            VARCHAR(20)	 	NOT NULL, 
	[UGradeID]	            INT	 	        NOT NULL, 
	[PGradeID]	            INT	 	        NOT NULL, 
	[RegDate]	            DATETIME	 	NOT NULL, 
	[Name]	                VARCHAR(50)	 	NOT NULL, 
	[Email]	                VARCHAR(50)	 	NULL, 
	[RegNum]	            VARCHAR(50)	 	NULL, 
	[Age]	                SMALLINT	 	NULL, 
	[Sex]	                TINYINT	 	    NULL, 
	[ZipCode]	            VARCHAR(50)	 	NULL, 
	[Address]	            VARCHAR(256)	NULL, 
	[Country]	            VARCHAR(50)	 	NULL, 
	[LastCID]	            INT	 	        NULL, 
	[Cert]	                TINYINT	 	    NULL, 
	[LastLogoutTime]	    SMALLDATETIME	NULL, 
	[HackingType]	        TINYINT	 	    NULL, 
	[HackingRegTime]	    SMALLDATETIME 	NULL, 
	[EndHackingBlockTime]	SMALLDATETIME 	NULL, 
	[LastloginTime]	        SMALLDATETIME 	NULL, 
	[ServerID]	            TINYINT	 	    NULL, 
	[DelFlag]	            TINYINT	 	    NULL, 
	[CCode]	                TINYINT	 	    NULL, 
);
GO
-------------------------------------------------------------------------------------------


/**
    단계 5. 잠시 CID 컬럼의 Identity 속성 끄고 데이터 밀어넣기
*/

ALTER DATABASE GunzDB SET RECOVERY SIMPLE;
SET IDENTITY_INSERT dbo.Account ON;

INSERT dbo.Account([AID], [UserID], [UGradeID], [PGradeID], [RegDate], [Name], [Email], [RegNum], 
	        [Age], [Sex], [ZipCode], [Address],	[Country], [LastCID], [Cert], [LastLogoutTime], 
            [HackingType], [HackingRegTime], [EndHackingBlockTime], [LastloginTime], [ServerID], [DelFlag], [CCode])
    SELECT  [AID], [UserID], [UGradeID], [PGradeID], [RegDate], [Name], [Email], [RegNum], 
	        [Age], [Sex], [ZipCode], [Address],	[Country], [LastCID], [Cert], [LastLogoutTime], 
            [HackingType], [HackingRegTime], [EndHackingBlockTime], [LastloginTime], [ServerID], [DelFlag], [CCode]
    FROM    dbo.AccountOld WITH (TABLOCK)
    WHERE   DelFlag = 0
    OR      ( DelFlag = 1 AND DeleteDate > DATEADD(mm, -6, GETDATE()) )

DECLARE @OldIdentity INT;
SET @OldIdentity = IDENT_CURRENT('AccountOld') + 1;
DBCC CHECKIDENT('Account', RESEED, @OldIdentity)

SET IDENTITY_INSERT dbo.Account OFF;
ALTER DATABASE GunzDB SET RECOVERY SIMPLE;


-------------------------------------------------------------------------------------------


/**
    단계 5. 테이블 각종 Constraint과 인덱스 만들기
*/

ALTER TABLE [dbo].[Account] ADD CONSTRAINT PK_Account PRIMARY KEY CLUSTERED (AID) 
GO

ALTER TABLE dbo.Account WITH CHECK ADD CONSTRAINT FK_Account_PremiumGrade
FOREIGN KEY (PGradeID) REFERENCES [dbo].[Account](PGradeID)
GO

ALTER TABLE dbo.Account WITH CHECK ADD CONSTRAINT FK_Account_UserGrade
FOREIGN KEY (UGradeID) REFERENCES [dbo].[Account](UGradeID)
GO

CREATE NONCLUSTERED INDEX IX_Account_DelFlag_DeleteDate ON [dbo].[Account](DelFlag, DeleteDate) 
CREATE NONCLUSTERED INDEX IX_Account_LastLogoutTime ON [dbo].[Account](LastLogoutTime);
CREATE NONCLUSTERED INDEX IX_Account_RegDate ON [dbo].[Account](RegDate);

CREATE UNIQUE NONCLUSTERED INDEX UIX_Account_UserID ON [dbo].[Account](UserID) 

ALTER TABLE [dbo].[Account] ADD CONSTRAINT DF_Account_DelFlag DEFAULT(0) FOR DelFlag
