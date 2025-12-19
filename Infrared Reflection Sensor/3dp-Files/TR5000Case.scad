include<CASE/Case2.scad>
include<CASE/oled_case.scad>


PARTS = 0;


case([50,70,40],part="bottom",thread = 1);
translate([0,45,20])rotate([90,0,0])sidewall();

up(50)case([50,70,40],part = "top");
fwd(50)case([50,70,40],part = "plate", usb = true, switch = 1);


module sidewall(){
casesize = [31.5,14,17];
x = -5;

translate([x,0,0]){
if(PARTS)color("tomato")translate([-31.5/2,-7.1,6])import("TCRT5000.stl");


    
        oledCase("snap",casesize); 
        up(20)
        difference(){
            oledCase("cover",casesize);
        
        translate([20,0,15])cuboid([10,12,10]);
        }

translate([6,0,0])difference(){
    cuboid([12,12,9.4],anchor = BOTTOM);
    right(2.7)cyl(d = 3, h = 10,anchor=BOTTOM);

}
difference(){
    translate([-x,5,0])rotate([90,0,0])case([50,90,40], part = "plate");
    translate([-12,0,-2])cuboid([8,12,4],anchor = BOTTOM);
    }
    }
}