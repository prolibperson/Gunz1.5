--insert into item(itemid, name, description, ressex, slot, iscashitem)
--values(5000002, '클랜명 변경권', '클랜의 이름을 영구히 변경할 수 있다.
--클랜마스터 코트(90일)가 함께 지급된다.
--변경 후에는 되돌릴 수 없으며 클랜 마스터만 구입할 수 있다.'
--, 3, 3, 1)
--
--insert into cashshop(csid, itemid, newitemorder, cashprice, webimgname, opened, regdate, renttype)
--values(371, 5000002, NULL, 7600, NULL, 0, getdate(), 0)

--------------------------------------------------------------

use GunzDB
go

create proc spWebSelectClanMaster
 @UserID varchar(20)
as
begin
 set nocount on 

 select c.Name as 'MasterName', cl.Name as 'ClanName'
 from dbo.Account a(nolock), dbo.Character c(nolock), Clan cl(nolock)
 where a.UserID = @UserID and c.AID = a.AID and cl.MasterCID = c.CID
 order by c.CID
end

--------------------------------------------------------------

use LogDB
go

create table ChangeClanNameLog
(
id int identity primary key
, CLID int
, OldName varchar(24)
, NewName varchar(24)
, MasterCID int
, MasterName varchar(24)
, RegDate datetime
);
go

create nonclustered index IDX_ChangeClanName_CLID
on dbo.ChangeClanNameLog(CLID);

create nonclustered index IDX_ChangeClanName_MasterCID
on dbo.ChangeClanNameLog(MasterCID);

create nonclustered index IDX_ChangeClanName_RegDate
on dbo.ChangeClanNameLog(RegDate);
go

--------------------------------------------------------------

use GunzDB
go

create proc spWebChangeClanName
 @MasterName varchar(24)
, @NewClanName as varchar(24)
as
begin
 set nocount on

 declare @CID int
 declare @sex tinyint

 select @CID = c.CID, @sex = sex from dbo.Character c(nolock) where c.Name = @MasterName and c.DeleteFalg <> 1
 if (@CID is null) or ((0 <> @sex) and (1 <> @sex))
 begin
  return (-1)
 end

 declare @CLID int
 declare @OldClanName varchar(28)
 
 select @CLID = CLID, @OldClanName = Name
 from dbo.Clan cl(nolock) 
 where cl.MasterCID = @CID and 1 <> cl.DeleteFlag
 if @CLID is null 
 begin
  return (-2)
 end

 declare @ItemID int

 if (0 = @sex)
  set @ItemID = 21011
 else if(1 = @sex)
  set @ItemID = 21511
 else
  return (-3)

 begin tran

 update dbo.Clan
 set Name = @NewClanName
 where CLID = @CLID
 if (0 <> @@ERROR) or (0 = @@ROWCOUNT) 
 begin
  rollback tran
  return (-4)
 end

 /* 	이 부분은 이름을 수정한 클랜 마스터한테 아이템을 지급하는 부분입니다.
	만약 적용을 한다면 아이템 추가 작업을 해야 합니다.
 insert into dbo.CharacterItem(CID, ItemID, RegDate, RentDate, RentHourPeriod)
 values(@CID, @ItemID, getdate(), getdate(), 2160)
 if (0 <> @@ERROR) or (0 = @@ROWCOUNT)
 begin
  rollback tran
  return (-5)
 end
 */


 insert into LogDB.dbo.ChangeClanNameLog(CLID, OldName, NewName, MasterCID, MasterName, RegDate)
 values(@CLID, @OldClanName, @NewClanName, @CID, @MasterName, getdate())
 if (0 <> @@ERROR) or (0 = @@ROWCOUNT) 
 begin
  rollback tran
  return (-6)
 end
 
 commit tran

 return 1
end
go

-----------------------------------------------------------------------------

use GunzDB
go

create proc spWebIsValidClanName
 @Name varchar(24)
as
begin
 set nocount on

 if (@Name is null) or (4 > len(@Name))return (-1)
 
 if exists(select * from dbo.AbuseList al(nolock) where @Name like al.Word)
 begin
  return -2
 end

 if exists(select * from dbo.Clan cl(nolock) where cl.Name = @Name) 
 begin
  return -3
 end

 return 1
end
go