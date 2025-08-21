include<CASE/Case2.scad>

hESP = 15;
yINA = -5;
boards = 0;
cs = [32,70,25];


bottom();
color("blue")translate([0,-cs[1]/2-10,cs[2]/2-0.3])front();
color("blue")translate([0,+cs[1]/2+10,cs[2]/2-0.3])backplate();


module front(){
difference(){
 case(size =[32,70,27], part= "plate",cutheight=10,explode = 0, lolin32lite=0,usb=0);
 translate([0,0,3.3])cuboid([7.5,5,3.8],rounding = 1, except=[FRONT,BACK]);
 }
}

module backplate(){
difference(){
 case(size =[32,70,27], part= "plate",cutheight=10,explode = 0, lolin32lite=0,usb=0);
 translate([-8,0,5])ycyl(d=5.65,h=20);
 translate([8,0,5])ycyl(d=5.65,h=20);
 translate([0,0,-5])ycyl(d=5.65,h=20);
 #translate([10.8,1.8,9])rotate([-90,-180,0])linear_extrude(height = 0.2)text("+",size=7);
 #translate([-6.4,1.8,9])rotate([-90,-180,0])linear_extrude(height = 0.2)text("-",size=7);
 #translate([13,1.8,-8])rotate([-90,-180,0])linear_extrude(height = 0.2)text("V-",size=7);

 }

}



up(25+8)case(size =cs, part= "top",cutheight=7,explode = 0, lolin32lite=0);

module bottom(){
if(boards){
color("green")translate([0,-15.5,hESP])import("Wemos-Mini-D1-ESP32.stl");
color("green")translate([0,8.5+yINA,4])rotate([0,180,180])import("ina219.stl");
}
case(size =cs, part= "bottom",cutheight=10,explode = 0, lolin32lite=0);
   for(i=[-1,1]){
      translate([(cs[0]/2)*i,-34.8,hESP-1.3])rotate([0,-90*i,0])espholder();
     
   }
    translate([0,yINA,0])difference(){
         cuboid([25,5,3.5],anchor = BOTTOM);
         for(i=[-1,1]){
         translate([-10.2*i,0,0])cyl(d=2,h=4,anchor=BOTTOM);
         }
      }//end difference
   }
   
   
module espholder(){
   difference(){
      h = 0.9;
      b = 6;
      l=40;
      prismoid([b,l],[b/2,l],h = h,anchor=FRONT+BOTTOM);
      fwd(0.01)up(h/2+.7)ycyl(d = 2,h=37,anchor=FRONT);
      }
      }