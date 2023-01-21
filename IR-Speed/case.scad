include <BOSL2/std.scad>
$fn=30;
x = 53;
y = 35;
z = 20;
w = 1.6;

modules = false;
module screwshaft(h=4){
    difference(){
    cylinder(d=6,h=h);
    cylinder(d=2,h=h+1);
    }
}
module esp32(){
    translate([42.5,24.3,0])rotate([0,180,0])import("parts/lolin32_lite.stl");
}

module oled(){
    import("parts/OLED.stl");
}

module mh(){
    translate([-12.8,8.4,-1.4])rotate([90,0,0])translate([0,-50,0])import("parts/mh-sensor.stl");
}

module case(){
    pos_mh=[7,0,3];
difference(){
    down(w/2)cuboid([x+2*2,y+2*2,z+w], rounding = 2, edges=[FRONT+RIGHT, FRONT+LEFT, BACK+RIGHT, BACK+LEFT]);
    cuboid([x,y,z+0.1],rounding = 2, edges=[FRONT+RIGHT, FRONT+LEFT, BACK+RIGHT, BACK+LEFT]);
    
    //slots esp
    translate([-x/2-w/2,-7.8,-4.5])cuboid([2*w,8,4]);
    translate([-x/2-w/2,7.9,-3.5])cuboid([2*w,8,7]);
    //slots mh
    translate(pos_mh)translate([10,-y/2-w/2,0])cuboid([6.5,2*w,11]);
}
    //screwshaft esp
    for(i = [[-17.6,-11.4,-z/2-0.1],[21,9.8,-z/2-0.1]]){
        translate(i)screwshaft(3);
    }
    //screwshaft mh
    translate(pos_mh)for(i=[0,-12]){
        left(i)translate([-10.5,-y/2,0])rotate([-90,0,0])screwshaft(2);
    }
    if(modules)translate(pos_mh)translate([10,-y/2+2,0])rotate([90,0,0])mh();

}
difference(){
    case();
    *translate([-50,0,-50])cube(100);
}
if(modules)color("blue")translate([-26,-13.5,-7])esp32();
