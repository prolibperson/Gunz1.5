USE GunzDB
GO

ALTER TABLE Character ADD DeleteDate DATETIME;

DROP INDEX dbo.Character.IX_Character_DeleteFlag
CREATE NONCLUSTERED INDEX IX_Character_DeleteFlag_DeleteDate ON Character(DeleteFlag, DeleteDate) 