/*
  Case for phyphox force-sensor
  
  Created by: Heinrich Lauterbach
  License: CC-BY-SA
  This file is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/
  
  needs BOSL2 library
  download BOSL2 to your libraries folder via:
  
  https://github.com/BelfrySCAD/BOSL2
  
  needs Case2.scad
  https://github.com/HeiLaut/ble-physics-sensors/blob/main/Case2.scad
*/


include<CASE/Case2.scad>
include<CASE/oled_case.scad>


//"Version A" 1kg load cell 80x12.7 2x M4; 2x M5
//"Version B" 1kg load cell 75x12.7 4x M4
//"Version C" 100g load cell 47x12 4xM3
Load_Cell_Version = "Version C";

panel1 = 1;
panel2 = 1;
keystone = 0;
oled = 0;
bottom = 1;
top = 1;
loadcell_connector = 1;
lockscrew = 0;

x = 45;//;32;//45;
y = 85;//85;
z = 50;
wall=2;
cutheight = 17.5;
parts = 0;

if(bottom)bottom();

if(top)up(z+20)case([x,y,z],part = "top",cutheight=cutheight,button = 1,wall=wall);

if(panel1)translate([0,60,23])plate1();

if(panel2)translate([-0,-60,11])plate2();

if(loadcell_connector)up(25)load_connector();

left(100)if(lockscrew)lockscrew(l=10,cut = true);

*right(12.2)up(25)loadcell();

module plate2(){
case([x,y,z],part = "plate",cutheight=cutheight,switchpos=[0,15],switch=1, jst=0,usbC = 1,wall=wall);
}

module load_connector(){
    rotate([0,0,90])difference(){
       left(16.5)cuboid([47,9,15]);
        if(Load_Cell_Version == "Version B"){
        for(i=[[-22,0,0],[-32,0,0]]){
                translate(i)ycyl(h = 60, d = 4); 
        }
        }//end if
        else if(Load_Cell_Version == "Version C"){
            for(k=[-1,1]){
                translate([-20,-5,3*k]){
                    ycyl(d=3.2,h=20);
                 }
                 }
                 left(42)cuboid(30);
        }
        else{
            for(i=[[-20,0,0],[-35,0,0]]){
                translate(i)ycyl(h = 60, d = 4);   
            }
            }//end else
        ycyl(h=20,d=5);
         translate([0,2,0])rotate([90,0,0]){
            if(Load_Cell_Version=="Version C"){
            rotate([0,0,90])cylinder(h = 4, d=8, $fn = 6);
            translate([0,10,2])cube([7,20,4],center = true);
            translate([0,0,-2.5])cylinder(d = 4, h =10);
            }
            else{rotate([0,0,90])cylinder(h = 4, d=9.5, $fn = 6);
            translate([0,10,2])cube([8,20,4],center = true);
             translate([0,0,-2.5])cylinder(d = 5, h =10);
             }
          }
    }
 }

module plate1(){
    xR=-8;
    zR=5;
    difference(){
        down(12)case([x,y,z],part = "plate",cutheight=cutheight,button = 1,wall=wall);
        if(keystone)translate([xR,0,zR+2])cuboid([15.3,30,25.4]);
        if(oled)translate([-10,0,0])rotate([90,90,0])oledCase("cutout");

    }
    if(oled)translate([-10,0,0])rotate([90,90,0])oledCase("snap");
    
    if(keystone)translate([0.55+xR,-70.4,55+zR])rotate([0,0,-90])import("parts/keystone.stl");
    if(keystone)mirror([0,1,0])if(parts)translate([xR,0,zR])import("parts/rj45.stl");
    if(oled)translate([-10,-10,0])rotate([90,90,0])oledCase("cover");
}
module rodmount(){
    h=17;
    difference(){
       rotate([0,90,0])prismoid(size1=[35,30], size2=[15,25], h=h);
       right(h/2-3){
        ycyl(d=10.5,h=200);
        zcyl(d=10.5,h=200);
        right(x/2+10)threaded_rod(d=10, pitch = 2, $slop=0.15,internal=true,l = 30,orient=LEFT,anchor=BOTTOM);
        }

    }
}


module bottom(){
    difference(){
        union(){
            case([x,y,z],part = "bottom",cutheight=cutheight,explode = 0,wall=wall,embossrot=[90,0,-90],embosspos=[-x/2-wall+0.2,0,7],embosstext="Kraftsensor 1 N");
            if(parts)translate([x/2-10,0,z/2])loadcell();
        //innere verstärkung
            translate([x/2,80/2-30/2+3,z/2])cuboid([4,25,20],chamfer=4,except=[FRONT,BACK,RIGHT],anchor=RIGHT);
            translate([x/2+wall,0,z/2])rodmount();
            }//end union
        if(Load_Cell_Version == "Version B"){
            for(i=[22,32]){
            translate([x/2,i,z/2])xcyl(d=4.5,h=40);
            translate([x/2+0.5,i,z/2])xcyl(d1=4.5,d2=8,h=3);
            }//end for
        }//end if
        else if(Load_Cell_Version == "Version A"){
          for(i=[20,35]){
            translate([x/2,i,z/2])xcyl(d=5,h=40);
            translate([x/2+0.5,i,z/2])xcyl(d1=5,d2=9,h=3);
        }
        }else{//Version C
                for(k=[-1,1]){
                translate([x/2-8,20,3*k+25]){
                    #xcyl(d=3.2,h=10,chamfer2=-1,anchor=LEFT);
                 }
                
            }
        }
          left(x/2)up(z/2)xcyl(h=10,d=8);

        }
     if(parts)import("parts/hx711.stl");
}

module loadcell(){
    if(Load_Cell_Version == "Version B"){
    difference(){
         cuboid([12.7,75,12.7]);
        for(i=[-32,-22,22,32]){
            fwd(i)xcyl(d=4,h=13);
            }//end difference
        }//end for
    }//end if
    else if(Load_Cell_Version == "Version C"){
        translate([3.3,0,0])difference(){
            cuboid([6,47,12]);
            for(i=[-1,1]){
                for(k=[-1,1]){
                translate([0,20*i,3*k])xcyl(d=3.2,h=10);
                }
            }
            }
        }
    else{
    color("green")difference(){
        cuboid([12.7,80,12.7]);
        for(i=[-35,-20,20,35]){
            fwd(i)xcyl(d=4,h=13);
        }//end for
        }//end diff
    }
}

module lockscrew(l=10,d = 10,cut = true){
   difference(){
      union(){
         threaded_rod(d=d-0.1, pitch = 2, l = l,orient=LEFT,anchor=TOP,bevel1=true);
         cuboid([d*1.5,d*2.5,d],chamfer = 2, except=[RIGHT],anchor = RIGHT);
         }
      if(cut)down(4)cuboid([80,80,d],anchor=TOP);
}
}
