include<CASE/Case2.scad>
x = 40;
y = 55;
z = 30;
cutheight = 6;

case([x,y,z],part="top",reset = 1,charge_view = 1, power_view = 1,lolinx=4,cutheight=cutheight, button=true);
down(50)case([x,y,z],part="bottom",cutheight=cutheight,thread=true,lolinx=4);

module sensor(){
    up(3.7)difference(){
        d=6.6;
        s=4.4;
        case([x,y,z],part = "plate",explode=0);
        back(1)union(){
         translate([-s/2,0,4])ycyl(d=d,h=4);
         up(4)cuboid([s,4,d]);
         translate([s/2,0,4])ycyl(d=d,h=4);
         }//end union
        for(i=[1,-1]){
         translate([i*8,0,4])ycyl(d=2,h=3,anchor=FRONT);
        }//end difference
    }
//end if
}//end front     
module plate(){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2,explode=0,jst = 0, usb = 1,switch= 1,switchrot=90,switchpos=[-14,3],lolinx = 4);
  
   }//end difference
   }//end frontplate
 
fwd(50)plate();
back(50)sensor();