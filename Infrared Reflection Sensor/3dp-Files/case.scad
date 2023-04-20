include <BOSL2/std.scad>
$fn=30;
x = 53;
y = 35;
z = 25;
w = 1.2;

modules = 0;

module mount(){
       dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
       translate([x/2,0,x/2])rotate([0,0,-90])difference(){
         cuboid([x,y,z],rounding=3,edges=[LEFT+FRONT,LEFT+BACK]);
          scale([1,1,1.05])translate([2.5,y/2+0.5,0])rotate([90,0,0])cylinder(d = dRod, h = y+1);
          translate([2.5,0,-y/2])rotate([0,0,0])cylinder(d = dRod, h = y+1);

          translate([-x/2+2,0,0])rotate([90,0,90]){
            rotate([0,0,90])cylinder(h = 3.5, d=8, $fn = 6);
            translate([0,l/2,1.7])cube([7,l,3.5],center = true);
           translate([0,0,-2.5])cylinder(d = 5, h =10);
          }
       }
    }


module screwshaft(h=4,d1=2,d2=6){
    difference(){
    cylinder(d=d2,h=h);
    cylinder(d=d1,h=h+1);
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
    pos_mh=[8,0.5,3];
    pos_esp=[0,3,-3.5];
difference(){
    down(w/2)cuboid([x+2*w,y+2*w,z+w], rounding = 2, edges=[FRONT+RIGHT, FRONT+LEFT, BACK+RIGHT, BACK+LEFT]);
    cuboid([x,y,z+0.1],rounding = 2, edges=[FRONT+RIGHT, FRONT+LEFT, BACK+RIGHT, BACK+LEFT]);
    
    //slots esp
    translate(pos_esp)translate([-x/2-w/2,-7.8,-3.5])cuboid([2*w,8,4],rounding=1);
    translate(pos_esp)translate([-x/2-w/2,7.9,-2.5])cuboid([2*w,8,7],rounding=1);
    //slots mh
    translate(pos_mh)translate([10,-y/2-w/2-pos_mh[1],0])cuboid([6.5,2*w,10]);
}
    //screwshaft esp
   for(i = [[-17.6+pos_esp[0],-11.4+pos_esp[1],-z/2-0.1],[21+pos_esp[0],9.8+pos_esp[1],-z/2-0.1]]){
        translate(i)screwshaft(3);
    }
    //screwshaft mh
    translate(pos_mh)for(i=[0]){
        left(i)translate([-10.5,-y/2-pos_mh[1],0])rotate([-90,0,0])screwshaft(pos_mh[1]+2,1.5);
    }
    if(modules)translate(pos_mh)translate([10,-y/2+2,0])rotate([90,0,0])mh();
    #if(modules)translate(pos_esp)color("blue")translate([-26,-13.5,-6])esp32();
        
    translate([-10,y/2+w+10,-w-z/2])mount();

}

module lid(){
    difference(){
    cuboid([x+2*w,y+2*w,w], rounding = 2, edges=[FRONT+RIGHT, FRONT+LEFT, BACK+RIGHT, BACK+LEFT]);
    cuboid([29.3,30.3,5]);
     
    }
    down(w+3/2)rect_tube(size=[x-0.2,y-0.2], wall=w, rounding=2, irounding=2, h=3);
}


module oled_bezel(){
    th = 1.6;//thickness of panel
    difference(){
        down(th/2)prismoid([35,35],[33,33],chamfer=1,h=th);
        //cuboid([33,33,th]);
        back(1)down(th/2)prismoid([24,14],[26,16], chamfer=1, h=th);
    }
    a = 10.6;
    b = 11;
    h=1.9; //heigth of screwshaft
    down(h+th/2)for(i=[[a,b,0],[a,-b,0],[-a,b,0],[-a,-b,0]]){
        translate(i)screwshaft(h,2,4);
    }
    down(th/2+4)rect_tube(isize=[26.5,27.5], wall=1.2, h=4);
    if(modules)down(4.6)color("green")oled();
}


difference(){
    union(){
    up(z/2+w)lid();
    case();
    *up(16)oled_bezel();

    }
    *translate([0,-50,-50])cube(100);
}




