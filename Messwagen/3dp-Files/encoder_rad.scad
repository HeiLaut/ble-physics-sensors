n=360/20;
$fn=60;
d = 3;
difference(){
   cylinder(d = 30, h = 2);
   for(i=[0:n:360]){
      rotate([0,0,i])translate([10,0,0])scale([2,0.5    ,1])cylinder(d=d,h=3);
   }
   cylinder(d=3.2,h=2);
   }