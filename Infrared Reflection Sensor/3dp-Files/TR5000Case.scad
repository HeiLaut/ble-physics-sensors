include<CASE/Case2.scad>


PARTS = 0;
casesize = [50,65,30];


case(casesize,part="bottom",thread = 1,embosstext = "IR-Sensor",embossheight = 8,);
translate([0,40,14])rotate([90,0,0])sidewall();

up(50)case(casesize,part = "top",charge_view = 1, power_view = 1);
fwd(50)case(casesize,part = "plate", usb = true, switch = 1);


module sidewall(){
x = -5;
z = 8;

translate([x,z,0]){
if(PARTS)color("tomato")translate([-31.5/2,-7.1,6])import("TCRT5000.stl");



translate([8.5,0,0])difference(){
    cuboid([9,12,8.4],anchor = BOTTOM);
    right()cyl(d = 2, h = 20,anchor=BOTTOM);

}
difference(){
    translate([-x,5-z,0])rotate([90,0,0])case(casesize, part = "plate");
    translate([-12,0,-2])cuboid([8,12,4],anchor = BOTTOM,chamfer = 1, except=[TOP,BOTTOM]);
    }
    }
}