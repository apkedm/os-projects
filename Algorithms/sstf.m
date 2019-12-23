clear;clc;
cur = 15;
req = [40, 48, 14, 44, 90,  99, 55];


sstfed = zeros(1,length(req));
count = 1;
sstfed(count) = cur;
fprintf("(%d) %d -> ",count,cur);
while isempty(req) ~= true
   count = count + 1;
   [~,ind]=min(abs(cur-req)); 
   fprintf("(%d) %d -> ",count,req(ind));
   cur = req(ind);
   sstfed(count) = cur;
   req(ind) = [];
end

fprintf("\n");

tot = sum(abs(diff(sstfed)));
fprintf("total dist traveled = %d\n",tot);
