------------------------------------------------------------------------------------------------------------------------------------------------
-- 삽입되지 않은 아이템 삽입

INSERT INTO Item(ItemID, Name) Values (520071, 'Rebel Agent Wig (Men)')
INSERT INTO Item(ItemID, Name) Values (520571, 'Rebel Agent Wig (Women)')

UPDATE	Item 
SET		TotalPoint=0, BountyPrice=0, Damage=0, Delay=0, Controllability=0, Magazine=0, 
		ReloadTime=0, SlugOutput=0, Gadget=0, SF=0, FR=0,CR=0,PR=0,LR=0, BlendColor=0, 
		ModelName='', MaxBullet=0, LimitSpeed=100, IsCashItem=1, 
		ResSex=1, ResLevel=25, Slot=4, Weight=3, HP=3, AP=8, MAXWT=0, 
		Description='A man’s wig for the assassin members of Rebel Agent, who are mysterious even to the Empire Information Division.' 
WHERE	ItemID = 520071

UPDATE	Item 
SET		TotalPoint=0, BountyPrice=0, Damage=0, Delay=0, Controllability=0, Magazine=0, 
		ReloadTime=0, SlugOutput=0, Gadget=0, SF=0, FR=0,CR=0,PR=0,LR=0, BlendColor=0, 
		ModelName='', MaxBullet=0, LimitSpeed=100, IsCashItem=1, 
		ResSex=2, ResLevel=25, Slot=4, Weight=3, HP=3, AP=8, MAXWT=0, 
		Description='A woman’s wig for the assassin members of Rebel Agent, who are mysterious even to the Empire Information Division.' 
WHERE	ItemID = 520571






------------------------------------------------------------------------------------------------------------------------------------------------
-- 잘못 업데이트된 아이템 업데이트

UPDATE	Item 
SET		TotalPoint=0, BountyPrice=0, Damage=0, Delay=0, Controllability=0, Magazine=0, 
		ReloadTime=0, SlugOutput=0, Gadget=0, SF=0, FR=0,CR=0,PR=0,LR=0, BlendColor=0, 
		ModelName='', MaxBullet=0, LimitSpeed=100, IsCashItem=1, 
		ResSex=1, ResLevel=0, Slot=4, Weight=2, HP=0, AP=0, MAXWT=0, 
		Description='A skull mask with a smile on it, which tempts people to wear it.' 
WHERE	ItemID = 520070


UPDATE	Item 
SET		TotalPoint=0, BountyPrice=0, Damage=0, Delay=0, Controllability=0, Magazine=0, 
		ReloadTime=0, SlugOutput=0, Gadget=0, SF=0, FR=0,CR=0,PR=0,LR=0, BlendColor=0, 
		ModelName='', MaxBullet=0, LimitSpeed=100, IsCashItem=1, 
		ResSex=2, ResLevel=0, Slot=4, Weight=2, HP=0, AP=0, MAXWT=0, 
		Description='A pitch-black witch’s hat. Despite its sinister look, it gives good luck to those who wear it.' 
WHERE	ItemID = 520570