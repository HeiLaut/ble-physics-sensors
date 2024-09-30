include<CASE/Case2.scad>
x = 40;
y = 53;
z = 35;
lolinspace = 9;

case([x,y,z],part="top",reset = 0, button=true,lolinspace=lolinspace,cutheight = 6,charge_view=1,power_view = 1);
down(50)case([x,y,z],part="bottom",lolinx=-4,thread=true,lolinspace=lolinspace,cutheight = 6);
down(25)fwd(50)plate();
down(25)back(50)sensor();

module sensor(){
   // translate([-19.7,3,26.5])color("red")rotate([-90,0,0])import("VL53L0X.stl");

    up(3.7)difference(){
        d=6;
        s=7;
        union(){
         case([x,y,z],part = "plate",explode=0,switch=0);
         up(2.5)fwd(2)case([x-5,y,z-5],part = "plate",explode=0);
         }

        back(1)union(){
         up(10)cuboid([s,10,d],rounding = 1, except=[FRONT,BACK]);
         up(4)fwd(2)cuboid([18,2,3]);
         }//end union
        for(i=[1,-1]){
         translate([i*9.7,-4,7.5])ycyl(d=2,h=8,anchor=FRONT);
         //translate([i*9.7,-2,7.5])ycyl(d=4,h=3,anchor=FRONT);

        }//end difference
    }
//end if
}//end front     
module plate(jst=false){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0,lolinspace=lolinspace,lolinx=-4,usbC = 1,switchpos=[0,7],switch=1);
   if(jst) translate([-7.7,-0.1,-0.8])cuboid([7,3,5.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
   //translate([7.3,-0.1,-2.5])cuboid([10.5,3,4.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
   
   }//end difference
   }//end frontplate
 


