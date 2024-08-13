include<BOSL2/std.scad>
$fn=60;
a = 100;
b = 20;
h = 20;
wall = 4;

difference(){
    cuboid([a,b,h]);
    down(wall)cuboid([a-2*wall,b-2*wall,h-wall]);
    right(a/2-20-wall)cuboid([40,b-2*wall,h+1]);

    for(i=[-1,1]){
        right(i*(a/2-20))ycyl(d=3.7,h=b*2);
        up(i*12.5/2)right((a/2-20-12.5))ycyl(d=2.8,h=b);
    }
    }

translate([-25,7,h+3])rotate([-90,90,0])espholder();
module espholder(){
    down(6)cuboid([26,47,2]);
        difference(){
        y=52;z=0;
        union(){
      translate([0,-y/2+3,-z/2-5])cuboid([26,8,3.4],anchor = BOTTOM+FRONT);
      translate([0,y/2-2,-z/2-5])cuboid([26,5.5,3.4],anchor = BOTTOM+BACK);
   }//end union
   translate([10.8,-y/2+3+5.4,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);
   translate([-10.3,y/2+3-8,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);

   }//end difference
    }