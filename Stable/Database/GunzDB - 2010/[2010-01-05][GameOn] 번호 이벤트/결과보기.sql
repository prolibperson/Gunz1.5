-- [0. 1]
SELECT * 
FROM Event_Lotto_GameOn r
WHERE r.Num0 > 0 AND r.Num1 > 0
	
-- [2. 3. 4]	
SELECT * 
FROM Event_Lotto_GameOn r
WHERE r.Num2 > 0 AND r.Num3 > 0 AND r.Num4 > 0
	
-- [5. 6. 7]
SELECT * 
FROM Event_Lotto_GameOn r
WHERE r.Num5 > 0 AND r.Num6 > 0 AND r.Num7 > 0

-- [8. 9]	
SELECT * 
FROM Event_Lotto_GameOn r
WHERE r.Num8 > 0 AND r.Num9 > 0