include<CASE/Case2.scad>
wall = 4;
showLoadCell = true;
dimension = [80,100,40];
down(20)bottom();
up(8.1)top();
translate([0,-14,32])pan();
translate([0,55,-6])frontPanel();
!backPanel();
module backPanel(){
           case([80,100,40-12],part="plate",usb=1,lolinx=-23,lolinspace=4);

}


module frontPanel(){
    difference(){
        case([80,100,40-12],part="plate",lolin32lite=true);
        left(25)rotate([90,90,0])switchcase(1);
        translate([28,1.5,0])cuboid([15,4,13]);
        }
        left(25)rotate([90,90,0])switchcase();
        color("red")translate([28,2,0])rotate([-90,-90,0])buttoncase(0,1);

}

module switchcase(hole = false){
   a = [4.5,4.4,3];//top cut
   b=[5,6,2];
   c= [3.5,4.4,0.5];
   if(hole){
   cuboid([7,3.7,wall*2]);
   }else{
   left(10.7/2)difference(){
      cuboid(b,anchor=RIGHT+BOTTOM);
      cuboid(a,anchor=RIGHT+BOTTOM);
      cuboid(c,anchor=RIGHT+BOTTOM);
      }
    right(10.7/2)mirror([1,0,0])difference(){
      cuboid(b,anchor=RIGHT+BOTTOM);
      cuboid(a,anchor=RIGHT+BOTTOM);
      cuboid(c,anchor=RIGHT+BOTTOM);
      }  
   left(12.5/2+1.25/2+0.4)cyl(d=2.3,h=2,anchor=BOTTOM);
   right(12.5/2+1.25/2+0.4)cyl(d=2.3,h=2,anchor=BOTTOM);
   }
}

module buttoncase(cap = true,case =true){
   x = 14;
   y = 17;
   z = 14;
   xi = 12;
   yi = 14.2;
   tol = 0.8;
   if(case)
   difference(){
      cuboid([x,y,z],anchor=TOP);
      up(0.1)cuboid([xi,yi,10],anchor=TOP);
      fwd(1.4)down(6)cuboid([7,y,6],anchor=TOP);
      down(6)cuboid([xi,1.5,6],anchor=TOP);
   }
   // button cap
    if(cap){
      down(0.25)difference(){
         cuboid([xi-tol,yi-tol,4],anchor = BOTTOM);
         down(0.05)cylinder(d=8,h=3.5/2-0.1,anchor = BOTTOM);
         }//end difference
      for(i=[1:90:279]){
         rotate([0,0,i])translate([-4/2-0.75/2,0,0])union(){
            cuboid([0.75,2.7,3.5]);
            translate([-0.75,0,0.25])cuboid([1.5,1,3]);
            translate([0.75/2,2.7/2,-3.5/2+0.7])rotate([90,90,0])linear_extrude(2.7)polygon([[0,0],[0.7,0.5],[0.7,0]]);
         }//end union
      }//end for
      }
  }
module m4Nut(){
    cyl(d=8.2,h=3.3,$fn=6);
    cuboid([20,7.1,3.3],anchor=LEFT);
    }
    
module pan(){
    x=60;
    y=60;
    z=6;
    
   module actualPan(){
       difference(){
        cuboid([x,y,z],chamfer=4, except=[TOP],anchor=BOTTOM);
        up(3)cuboid([x-2,y-2,z],chamfer=4, except=[TOP],anchor=BOTTOM);
        back(10)prismoid([15,x-5],[18,x-5],h=2);
       }//end difference

   }//end actualpan
   
   module panMount(){
   
    difference(){
        union(){
            prismoid([15,25], [15,35], chamfer=2, h=14,anchor=TOP);
            prismoid([14.5,35],[17.5,35],h=2,chamfer= 2, anchor=BOTTOM);
            }//end union
         down(20)back(25)for(i=[65/2,45/2]){
            fwd(i)cyl(d=4,h=50);
            up(18)fwd(i)cyl(d=8,h=20);
            //up(10)fwd(i)m4Nut();
            }//end for
        }//end difference
   }//end panMount
   
   actualPan();
   color("green")down(10)panMount();

}

module top(){
union(){
    difference(){

        case(dimension,part="top",cutheight=14);
        translate([0,-14,0])cuboid([25,40,5],chamfer=4,except=[TOP,BOTTOM]);
         translate([0,35,-1.4])cuboid([40,14,30]);
        }
        *color("green")translate([0,35,-1.35])rotate([-90,0,0])import("OLED091MOUNT002.stl");
        }

}//end module


module bottom(){
    difference(){
    union(){
    case(dimension,lolinspace=4,lolinx=-23,part="bottom");
    color("red")translate([0,33,-0.1])difference(){
        cuboid([20,30,10],anchor=BOTTOM,rounding=-2,edges=BOTTOM);
            up(5)cuboid([13,80,13],anchor=BOTTOM);
           
        }//end difference
    }//end union
    up(0.5)back(11)for(i=[-65/2,-45/2]){
            fwd(i)cyl(d=4,h=20);
            fwd(i)cyl(d=7,h=5);
        }
    }//end difference
    if(showLoadCell)color("blue")translate([0,11,11.5])loadcell5();
}

module loadcell5(){
difference(){
    cuboid([12.8,75,12.8]);
    for(i=[65/2,45/2,-65/2,-45/2]){
        fwd(i)cyl(d=4,h=13);
    }
    }
}


