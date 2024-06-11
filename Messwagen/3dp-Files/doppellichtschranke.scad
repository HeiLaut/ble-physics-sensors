include<BOSL2/std.scad>
//$fn=60;
a = 3.5;
d = 8;
dLED = 5.04;
dHOLE = 1.5;
h=23.1;
angle = 2.5;
module lightgate(){
   difference(){
      union(){
         //cuboid([a+2*d,20,2],anchor=TOP);
         left(a/2+d/2)cuboid([d,20,h],anchor=BOTTOM);
         right(a/2+d/2)cuboid([d,20,h],anchor=BOTTOM);
         }
         translate([-a/2-d/2,0,9])
            {
            xcyl(d=dLED,h=d);
            *xcyl(d=dHOLE,h=d*2);
            }
         translate([-1+a/2+d/2+1,3,9])rotate([0,0,0]){
            xcyl(d=dLED,h=d);
            *fwd(1)xcyl(d=dHOLE,h=d*2+1);
            }
         translate([a/2+d/2,-3,9])rotate([0,0,0]){
            xcyl(d=dLED,h=d);
            *back(1.5)xcyl(d=dHOLE,h=d*2);
            }
         up(18)xcyl(d=3.5,h=a+2*d);
   }
   }
   
   //lightgate();