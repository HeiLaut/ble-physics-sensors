include<CASE/Case2.scad>


x = 45;
y = 85;
z = 50;
wall=2;
cutheight = 17.5;
parts = 0;

bottom();

up(z+20)case([x,y,z],part = "top",cutheight=cutheight,button = 1,wall=wall);
translate([60,43,23])plate1();

!translate([-60,43,23])plate2();

module plate2(){
case([x,y,z],part = "plate",cutheight=cutheight,switch=0, jst=0,usb = 0,wall=wall);
}


module plate1(){
    xR=-8;
    zR=5;
    difference(){
        down(12)case([x,y,z],part = "plate",cutheight=cutheight,button = 1,wall=wall);
        translate([xR,0,zR+2])cuboid([15.3,30,25.4]);
    }
    translate([0.55+xR,-70.4,55+zR])rotate([0,0,-90])import("parts/keystone.stl");
    mirror([0,1,0])if(parts)translate([xR,0,zR])import("parts/rj45.stl");

}
module rodmount(){
    h=15;
    difference(){
       rotate([0,90,0])prismoid(size1=[35,30], size2=[15,25], h=h);
       right(h/2-2){
        ycyl(d=10.5,h=200);
        zcyl(d=10.5,h=200);
        right(x/2+10)threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = 30,orient=LEFT,anchor=BOTTOM);
        }

    }
}


module bottom(){
    difference(){
        union(){
            case([x,y,z],part = "bottom",cutheight=cutheight,explode = 0,wall=wall);
            if(parts)translate([x/2-10,0,z/2])loadcell();
        //innere verstärkung
            translate([x/2,80/2-30/2+3,z/2])cuboid([4,25,20],chamfer=4,except=[FRONT,BACK,RIGHT],anchor=RIGHT);
            translate([x/2+wall,0,z/2])rodmount();
            }//end union
        for(i=[20,35]){
            translate([x/2,i,z/2])xcyl(d=5,h=40);
            translate([x/2+0.5,i,z/2])xcyl(d1=5,d2=9,h=3);
        }//end for
            left(x/2)up(z/2)xcyl(h=10,d=8);
     }//end difference
     if(parts)import("parts/hx711.stl");
}

module loadcell(){
    color("green")difference(){
    cuboid([12.7,80,12.7]);
    for(i=[-35,-20,20,35]){
        fwd(i)xcyl(d=4,h=13);
    }//end for
    }//end difference
}