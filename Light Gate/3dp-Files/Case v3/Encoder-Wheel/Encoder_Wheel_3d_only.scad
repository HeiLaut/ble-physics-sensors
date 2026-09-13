include<BOSL2/std.scad>
include <BOSL2/threading.scad>
$fn = 60;
//$slop = 0.13;
dS = 7;
pitch = 2;
starts=2;
h = 15;


module rodmount(mount = true,hole=0,l = 30){
      difference(){
      if(!hole)cuboid([15,15,8],rounding=2,except=[TOP,BOTTOM],anchor=BOTTOM);
      if(mount){
         threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = l,orient=TOP,anchor=BOTTOM);
      }
      }//end difference
}//end rodmount

module base(){
difference(){
   cuboid([28,120,h],chamfer=2,except=[TOP,BOTTOM],anchor = FRONT);
   cuboid([13,60,h+.2],chamfer=2,except=[TOP,BOTTOM,FRONT],anchor = FRONT);
   for(i=[1,-1])translate([-5.3*i,15,0]){
            rotate([-90*i,0,90])cyl(d1 = 20, h =5, d2 = 0.5,anchor = BOTTOM);
            up(8)rotate([-90*i,90,90])prismoid(size1=[20,15], size2=[20,0.5], h=3.5,anchor=BOTTOM);
            }
   translate([0,110,0])cyl(d=10.5,h = 30);
   #translate([0,110,0])rotate([-90,0,0])rodmount(hole=1,l=10);
   }
}

 
module rod(internal=false,l=20){
         threaded_rod(l=l, internal = internal,pitch=pitch, d=dS,starts=starts,anchor=BOTTOM);

}
module wheel(expld = 0){
   a = 360/20;
   d = 55;
difference(){
   union(){
      xcyl(d =d,h=4)attach(RIGHT)cyl(d= 15, h =3,anchor=BOTTOM)attach(TOP)cyl(d1 = 15, h =5, d2 = 0.5,anchor=BOTTOM);
      }
   rotate([0,90,0])rotate_extrude()translate([d/2+2,0,0])scale([1.5,0.8,1])circle(2);
   left(2)threaded_rod(h = 2.5, d = 15, pitch = 1, internal = true,anchor=BOTTOM,$slop=0.01,orient=RIGHT);

   for(i=[0:a:360]){
      left(5)rotate([i,0,0])rotate([0,90,0])rotate_extrude(angle=a/2)translate([12,0,0])square([d/2-14,10]);
   //rotate([i,0,0])scale([1,0.3,1])translate([0,0,12])xcyl(d=10,h=20);
   
  }
 }
 //left(expld/2)threaded_rod(h = 7, d = 8, pitch = 0.75, anchor=BOTTOM,orient=RIGHT);
 left(expld)difference(){
    xcyl(d= 15, h =3){
        attach(LEFT)cyl(d1 = 15, h =5, d2 = 0.5,anchor=BOTTOM);
        attach(RIGHT)threaded_rod(h = 2, d = 15, pitch = 1, anchor=BOTTOM);
        }
    *left(expld)threaded_rod(h = 4, d = 8, pitch = 1, internal = true,anchor=BOTTOM,$slop=0.1,orient=RIGHT);}

}
module screw(l = 50,h=5,d=10,head = 0,orient= TOP){
   difference(){
   union(){
      threaded_rod(d=d-0.1, pitch = 2, l = l,orient=orient,anchor=TOP,bevel2=1);
      if(head){
   sphere(d=d-1.7,anchor=BOTTOM);
   down(0.1)cyl(d=d-3,h=9/2,anchor=BOTTOM);
   }
   down(l-0.2)cuboid([d,d*2.5,d*1.5],chamfer = 2, except=[TOP],anchor = TOP);
   
   }
   translate([-d/2,0,10])cuboid([2,d*2.5,l*4],anchor=TOP);
   }//end difference
   
   if(head){
   wall=3;
   up(12)rotate([180,0,0])difference(){
   union(){
      cyl(d=d-1.6+wall,h=4.2,anchor=BOTTOM);
      cyl(d=18,h=3,anchor=TOP);
        }
      down(2)sphere(d=d-1.6,anchor=BOTTOM);
      //up(7)cuboid([15,15,10]);
      for(i=[0,90]){
         rotate([0,0,i])cuboid([2,15,7.5],anchor=BOTTOM);
         }
      }
      }//end head
   }

   
difference(){
union(){
    base();
    fwd(10)wheel(expld = 10);
    }
*back(15)cuboid(100,anchor=BACK);
}

translate([0,130,0])rotate([90,-90,0])screw(l=10);