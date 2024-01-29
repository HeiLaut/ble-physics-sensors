include<CASE/Case2.scad>
x = 30;
y = 52;
z = 25;

case([x,y,z],part="top",reset = true, button=true);
down(50)case([x,y,z],part="bottom",thread=true);

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
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0);
   translate([-7.7,-0.1,-0.8])cuboid([7,3,5.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
   translate([7.3,-0.1,-2.5])cuboid([10.5,3,4.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
   }//end difference
   }//end frontplate
 
fwd(50)plate();
back(50)sensor();