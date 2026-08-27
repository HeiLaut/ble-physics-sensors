include<CASE/Case2.scad>
include<CASE/oled_case.scad>
include <BOSL2/screws.scad>

*translate([0,-5,57.4])screw("M3x0", length=16, thread=0.35, anchor=TOP,  head="socket", drive="hex");
*color("red")translate([0,-5,38.1])nut("M3",thickness="normal");
x = 48;
y = 100;
z = 40;
wall = 2;


//choose between OLED- or NON-OLED-Version
OLED = 1;

BOTTOM_PLATE = 0; //EMITTER AND RECVEIVER
FRONT_COVER= 1; //EMMITTER AND RECEIVER

RECEIVER_BACK_COVER = 0;
EMITTER_BACK_COVER = 0;


RECEIVER_TOP = 0;
EMITTER_TOP = 0;

button = OLED ? true : false;

gap_width = 7.7; //6.5

explode = 10;

//up(9)rotate([180,0,0])sender_module();
intersection(){
    union(){
        up(39.8+1.5)slider();
        if(FRONT_COVER)up(z+explode)front();
        }
        *up(39.8+2.5)cuboid([30,50,50]);
        }
if(RECEIVER_BACK_COVER)backp();
if(RECEIVER_TOP)fwd(y/2+explode)up(z/4+wall)frontplate();
if(BOTTOM_PLATE)back(y/2+explode)up(z/4+wall)bottom();

if(EMITTER_TOP)fwd(y/2+explode)up(z/4+wall)frontplate(0);
if(EMITTER_BACK_COVER)backp2();


module sender_top(){
    case([x,y,z],part = "plate",switch = true);

}

module sender_module(){
    difference(){
        cuboid([18,10.9,7],anchor=BOTTOM)
            attach(TOP)right(-18/2+14)cyl(d = 6, h  = 1,anchor= BOTTOM)
                attach(TOP)cyl(d = 5, h  = 1.5,anchor= BOTTOM)
                attach(TOP)sphere(d=5);
        translate([18/2-12,0,-0.1])cyl(d=3.2,h=10,anchor=BOTTOM);
        }
}


module sender_snap(hole  = 0){
    if(!hole)difference(){
        cuboid([25,20,wall],anchor=BOTTOM, rounding =2, except = [TOP,BOTTOM])
            attach(TOP){
                translate([18/2-12,0,0])
                    {
                    cyl(d = 3,h = 8,anchor=BOTTOM);
                    for(i=[-1,1])translate([2,i*(5.5+0.6),0])cuboid([10,1.2,7.8],anchor=BOTTOM)
                    attach(TOP)xcyl(d=2,h=10);
                    
                    }

                    
                }
        
        }
        if(hole)down(0.1)right(-18/2+14)cyl(d = 6.2, h  = 3,anchor= BOTTOM);
    
}

module front(versionA = false){
    module_height = 8.5;
    if(versionA)difference(){
        union(){
            case([x,y,z],part = "top", wall = wall);
            back(-y/2+20)up(wall)rotate([180,0,-90])sender_snap();
            }

        back(-y/2+20)up(wall)rotate([180,0,-90])sender_snap(1);

        }
        else{
        l = y-20;
     difference(){
            case([x,y,z],part = "top", wall = wall);
            up(wall/2)cuboid([gap_width,l,wall*2]);
            for(i=[-3:3])translate([0,i*10,wall])cuboid([20,1,0.2],anchor=TOP);
        }
      difference(){
        union(){
            cuboid([20,l,module_height+7],anchor=TOP,rounding=-2,edges=[TOP+LEFT,TOP+RIGHT]);
            cuboid([x,wall,14.5],anchor=TOP);
            }
        cuboid([11,y-9,module_height+6.5],anchor=TOP);
        down(module_height+2.5)cuboid([14,y-9,5],chamfer=2,edges=[LEFT+BOTTOM,RIGHT+BOTTOM]);
      }
       
}//end else
}


module slider(){
    l = 20;
  
    color("green")difference(){
        union(){
            cuboid([13.5,l,4.5],chamfer=1.75,edges=[LEFT+BOTTOM,RIGHT+BOTTOM],anchor=TOP)attach(BOTTOM){
            cuboid([10,l,1.25],anchor=BOTTOM);
            }//end attach
    }//end union
    fwd(10-5)cyl(d=3.25,h=6.75,anchor=TOP)attach(BOTTOM){
    down(2.5)rotate([0,0,90])cyl(d=6.3,h=2.5,$fn=6,anchor=TOP)attach(LEFT)
        down(2)cuboid([5.6,2.5,10],anchor=BOTTOM);
    };
   }//end difference
   
   //"nut" starts here
   module flat_nut(){
    difference(){
        cuboid([gap_width-0.2,9,wall],anchor=TOP)attach(TOP){
        cuboid([10,9,wall],anchor=BOTTOM);
        }
        cyl(d = 4, h = 10);
    }
    }
    module round_nut(){
        difference(){
            cyl(d=gap_width-0.2,h=wall,anchor=TOP)attach(TOP)cyl(d=10,h=7.5,anchor=BOTTOM,chamfer2=1);
            for(i=[0:45:360])rotate([0,0,i])left(10/2+0.5)cyl(d=2,h=20);
            up(wall-1)cyl(d=5.4, h = 7.5,anchor=BOTTOM,chamfer1=1);
            up(wall)cyl(d=3.4, h = wall*2,anchor=TOP);

            }
    }
    //translate([0,-5,9.75])flat_nut();
    translate([0,-5,10.75])round_nut();
    
}

module backp(etext= "IR Receiver"){
    difference(){
        union(){
            case([x,y,z],part = "bottom",embosstext = etext,button = button,buttonpos = [0,40],embosspos=[0,-10],wall = wall);
            if(OLED)back(20)oledCase("snap");
            if(OLED)up(10)back(20)oledCase("cover");

            }
            if(OLED)back(20)oledCase("cutout");

            }

}

module backp2(etext= "IR Emitter"){
            case([x,y,z],part = "bottom",lolin32lite = 0,embosstext = etext,button = 0,buttonpos = [0,40],embosspos=[0,-10],wall = wall);
         
}

module frontplate(receiver = true){
   difference(){
    union(){
        case([x,y,z],part = "plate", usbC = receiver, switch = 1,switchpos = [10,5], wall = wall);
        translate([-12,wall,12])ycyl(d = 7, h = 4,anchor=FRONT);
        }
        translate([-12,-0.1,12])ycyl(d = 5, h = 8,anchor=FRONT);
        }
    if(!receiver){
        wall1 = 2;
        translate([0,wall,0])difference(){
            cuboid([20+2*wall1,14,3.2+2*wall1],anchor = FRONT,chamfer = 4, except = [TOP,BOTTOM,FRONT])
                attach(TOP)fwd(4)text3d("+",h = 0.2,anchor=BOTTOM);
            cyl(d = 20, h = 3.2,anchor = FRONT);
            translate([5,10,0])cyl(d=2,h=3.2+2*wall1)
                attach(TOP){
                    down(5.4)cuboid([2,10,0.5],anchor=FRONT);
                    down(1.8)cuboid([2,10,0.5],anchor=FRONT);
                    }
            }
    }
    }

module bottom(){
    difference(){
        case([x,y,z],part = "plate",wall=wall);
        up(z/4-wall)back(wall)ycyl(d=15,h=wall,anchor=BACK);
        }
    up(z/4-wall)back(wall)rotate([90,0,0])rodmount();

}
/*
case([x,y,z],part = "plate",wall = wall,usbC = 1);
case([x,y,z],part = "bottom",wall = wall);
up(z+10)case([x,y,z],part = "top",wall = wall);
*/