include<BOSL2/std.scad>
$fn=40;
include<doppellichtschranke.scad>

module ir_holder(){
    a = 10;
    b = 12; //b = 30 for speed sensors
    d = 39;
    h = 4;
    diff = 1.1;
    angle = 0*360/40;
    module sens_holder(l=true,a=10,u=0,holes=true){
        difference(){
            up(u)cuboid([h,b,a]);
            if(holes){
            fwd(10.5)xcyl(d=2.2,h=5);
            back(10.5)xcyl(d=2.2,h=5);
             up(u)if(l){cuboid([3,14,a],anchor=LEFT);}
            else{cuboid([3,14,a],anchor=RIGHT);}
        }
        }//end difference
    }//end sens holder
    module wall(){
        hull(){
        left(d/2+h/2)cuboid([h,h,a]);
        down(diff)rotate([0,angle,0])right(d/2+h/2)cuboid([h,h,a]);
        }//end hull
    }//end wall
    
    left(d/2+h/2)sens_holder();
    down(diff)right(d/2+h/2)sens_holder(0,10,diff);
    right(74.5)sens_holder(holes=false);
     difference(){
    union(){
    for(i=[-1,1]){
    translate([d/2+h/2+5,i*(b/2+h/2),0])cuboid([100,h,10]);
    }
    *fwd(b/2+0.5)wall();
    *back(b/2+0.5)wall();
    }//end union
    //axis
    down(0)ycyl(d=3.7,h=b*2);
    right(50)ycyl(d=3.7,h=b*2);

    }//end difference
    
    
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
   translate([48,0,4])cuboid([50,b,2]);
   translate([50,2,18])rotate([90,90,0])up(10)espholder();
   up(18.1)rotate([0,180,90])lightgate();
}//end ir holder
module wheel(){
    difference(){
      union(){
         cyl(d = 36,h=2.8,$fn=100);
         up(2.8)cyl(d = 8,h=2.8,$fn=100);
         }
        cyl(d=3.2,h=10);
    }
}

module bat(){
 d = 18;
 l = 65;
 difference(){
    down(3)cuboid([l/2,d+2,d]);
    xcyl(d=d+1,h=l);
 }
}
ir_holder();
//wheel();
*translate([50,4,15])bat();