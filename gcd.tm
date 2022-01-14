#Q = {0,write,left,begin,above,below,end,above_end,above_reach,below_end,below_reach}

#S = {0,1}

#G = {0,1,_}

#q0 = 0

#B = _

#F = {end}

#N = 2

0 1_ ** r* 0
0 0_ _* r* write
write 1_ _1 rr write

;write finished, move to leftside
write __ __ ll left
left 11 ** ll left
left 1_ ** l* left
left _1 ** *l left
left __ ** rr begin

;compare the two numbers
begin 11 ** rr begin
begin 1_ ** ll above
begin _1 ** ll below
begin __ ** ** end
;the number is same, end.

;move ptr to the end
above 11 ** r* above
above _1 ** l* above_end
below 11 ** *r below
below 1_ ** *l below_end

;start to minus two numbers
above_end 11 _1 ll above_end
above_end 1_ ** *r above_reach
below_end 11 1_ ll below_end
below_end _1 ** r* below_reach
above_reach 11 ** l* above_reach
above_reach _1 ** r* begin
below_reach 11 ** *l below_reach
below_reach 1_ ** *r begin