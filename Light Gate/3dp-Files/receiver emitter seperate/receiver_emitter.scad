include<CASE/Case2.scad>
include<CASE/oled_case.scad>

x = 48;
y = 100;
z = 40;
wall = 2;


//choose between OLED- or NON-OLED-Version
OLED = 1;


BOTTOM_PLATE = 1; //EMITTER AND RECVEIVER
FRONT_COVER= 1; //EMMITTER AND RECEIVER

RECEIVER_BACK_COVER = 1;
EMITTER_BACK_COVER = 0;


RECEIVER_TOP = 1;
EMITTER_TOP = 0;

button = OLED ? true : false;


explode = 10;

//up(9)rotate([180,0,0])sender_module();
if(FRONT_COVER)up(z+explode)front();
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

module front(){
    difference(){
        union(){
            case([x,y,z],part = "top", wall = wall);
            back(-y/2+20)up(wall)rotate([180,0,-90])sender_snap();
            }

        back(-y/2+20)up(wall)rotate([180,0,-90])sender_snap(1);

        }
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