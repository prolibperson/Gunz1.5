USE GunzDB
GO

CREATE TABLE dbo.LocatorStatus
(
    LocatorID           INT	            NOT NULL
    , IP                VARCHAR(15)     NOT NULL
    , Port              INT             NOT NULL
    , RecvCount         INT             NULL
    , SendCount         INT             NULL
    , BlockCount        INT             NULL
    , DuplicatedCount   INT             NULL
    , UpdateElapsedTime INT             NOT NULL
    , LastUpdatedTime   SMALLDATETIME   DEFAULT(GETDATE()),

    CONSTRAINT PK_LocatorStatus PRIMARY KEY CLUSTERED (LocatorID ASC)
)
GO

CREATE UNIQUE NONCLUSTERED INDEX UIX_Locator_Status_IP ON dbo.LocatorStatus(IP ASC)

-- EXEC sp_helpindex LocatorStatus