//universal electronic case
include<BOSL2/std.scad>
include<BOSL2/threading.scad>
$fn = 50;

//case([30,52,25],part="bottom",explode = 10,rd=2,lolinspace = 7.4,button = true,reset = 1, thread = true);

//must be larger than 1.5 mm
module case(
   size = [15,15,15],
   clearance = 0.15, 
   wall = 2,
   rd = 2,
   cutheight = 2,
   snap = 2,
   part = "all",
   explode = 0,
   lolin32lite = true,
   lolinspace = 7.4,
   thread = false,
   button = false,
   reset = false)
   {
   x = size[0];
   y = size[1];
   z = size[2];

   ox = x+2*wall;
   oy = y+4*wall;
   oz = z+2*wall;
   // select chamfer OR rounding
   //chamfer = 1;
   //rounding = 1;

   //rd = 2;

   clearance = clearance;

   //overlap of front and back plate into body
   overlap = 1;

   //height of cut between upper and lower half. 0 means in the middle <1 upper half bigger than lower half; >1 lower half bigger than upper half
   //cutheight = snap;

   //snap parts
   d1 = snap;

   if(cutheight>z/2-wall){
      echo("Cuthehgt to large");
   }
   module body(explode = 2,part){
         module both(){
            difference(){
            cuboid([ox,oy,oz],chamfer = rd,except=[FRONT,BACK]);
            cuboid([x,oy+1,z],chamfer = rd,except=[FRONT,BACK]);
            for(i = [-1,1]){
               translate([0,i*(-y/2-wall/2),0])cuboid([x+2*overlap+clearance*2,wall+clearance*2,z+2*overlap+clearance*2],chamfer = rd,except=[FRONT,BACK]);
               }//end for
            }//end difference
            }//end both
            
          //top part of the case
         module upper(){
            difference(){
               both();
               translate([0,0,cutheight])cuboid(x*y,anchor=TOP);
               if(button){
                  translate([0,0,z/2])cuboid([12.2,14.2,5]);
                  }
               //reset button
               if(reset){
                  translate([-0.8,-y/2+5,z/2+0.1])difference(){
                     union(){
                        cyl(d=10,h=4);
                        cuboid([10,10,4],anchor=FRONT);
                        }//end union
                       cyl(d=8.5,h=4);
                       cuboid([8.5,10,4],anchor=FRONT);
                     }//end difference
                     translate([-0.8,-y/2+5,z/2-1])union(){
                        cyl(d=10,h=4);
                        cuboid([10,10,4],anchor=FRONT);
                  }//end union
                  }
            }//end difference
            
            //snap connector male
            for(i = [-1,1]){
               translate([i*(-x/2-d1/4),0,cutheight+d1/2])difference(){
                  ycyl(d = d1, h = y-clearance*2);
                  translate([-i*d1/4,0,0])cuboid([3*d1/4,y-clearance*2,d1]);
                  }
               }//end for
            if(button){
               translate([0,0,z/2+wall])buttoncase();
               }
            if(reset){
                translate([-0.8,-y/2+3.5,z/2+1])cyl(d=3,h=z-lolinspace-2.6,anchor=TOP);

            }
         }//end uppercase
         
         //lower part of the case
         module lower(){
            difference(){
               both();
               translate([0,0,cutheight])cuboid(x*y,anchor=BOTTOM);
               if(thread){
                  translate([0,0,-z/2])cuboid([13,13,15],rounding=2,except=[TOP,BOTTOM]);
               }//end if thread
            }//end difference
            //snap connector female
            for(i=[-1,1]){
               difference(){
                  translate([i*(x/2),0,cutheight+d1/8])rotate([0,i*90,0])prismoid(size1=[d1*2,y-clearance*2], size2=[d1*3,y-clearance*2], h=d1,anchor=TOP);
                  translate([i*(x/2+d1/4),0,cutheight+d1/2])ycyl(d = d1+clearance*2, h = y-clearance*2);
                  translate([i*(x/2-clearance/2),0,cutheight])cuboid([clearance,y,d1*4/2],anchor = BOTTOM);
                  }//end difference
               }//end for
              if(lolin32lite){
                  difference(){
                     union(){
                        translate([0,-23,-z/2])cuboid([26,8,lolinspace],anchor = BOTTOM+FRONT);
                        translate([0,24,-z/2])cuboid([26,5.5,lolinspace],anchor = BOTTOM+BACK);
                     }//end union
                  translate([10.8,-17.6,-z/2])cyl(d=2,h=lolinspace+1,anchor = BOTTOM);
                  translate([-10.3,21,-z/2])cyl(d=2,h=lolinspace+1,anchor = BOTTOM);
                 }//end difference
               }//end if lolin32lite
               
               //threadmount
               if(thread){
                  translate([0,0,-z/2-wall])rodmount();
               }//end if thread
         }//end lowercase
         if(part == "upper"){
            up(explode/2)upper();
            }else{
            down(explode/2)lower();
            }
         
         
   }//end body
   module frontplate(){   
      cuboid([x+2*overlap,wall,z+2*overlap],chamfer = rd,except=[FRONT,BACK]);
      }//end frontplate
      
      
   module backplate(){   
      translate([0,-1*(-y/2-wall/2),0])cuboid([x+2*overlap,wall,z+2*overlap],chamfer = rd,except=[FRONT,BACK]);
      }//end backplate

   if(part == "all"){
      body(explode,"upper");
      body(explode,"lower");
      fwd(explode+y/2+wall/2)frontplate();
      back(explode)backplate();
      }
   if(part == "top"){
      down(z/2)body(explode,"upper");

   }
   if(part == "bottom"){
      up(z/2)body(explode,"lower");

   }
   if(part == "plate"){
      back(wall/2)fwd(explode)frontplate();

   }
}//end case

module rodmount(){
      difference(){
      cuboid([15,15,8],rounding=2,except=[TOP,BOTTOM],anchor=BOTTOM);
      threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = 30,orient=TOP,anchor=BOTTOM);
      }//end difference
}//end rodmount

module buttoncase(cap = true){
   x = 14;
   y = 17;
   z = 14;
   xi = 12;
   yi = 14.2;
   tol = 0.8;
   difference(){
      cuboid([x,y,z],anchor=TOP);
      up(0.1)cuboid([xi,yi,10],anchor=TOP);
      fwd(1.4)down(6)cuboid([7,y,6],anchor=TOP);
      down(6)cuboid([xi,1.5,6],anchor=TOP);
   }
   // button cap
   if(cap){
      down(0.25)difference(){
         cuboid([xi-tol,yi-tol,5],anchor = BOTTOM);
         down(0.05)cylinder(d=7,h=3.5/2-0.1,anchor = BOTTOM);
         }//end difference
      for(i=[1:90:279]){
         rotate([0,0,i])translate([-4/2-0.75/2,0,0])union(){
            cuboid([0.75,2.7,3.5]);
            translate([-0.75/2,0,0.25])cuboid([1,1,3]);
            translate([0.75/2,2.7/2,-3.5/2+0.7])rotate([90,90,0])linear_extrude(2.7)polygon([[0,0],[0.7,0.5],[0.7,0]]);
         }//end union
      }//end for
      }
     
}

module standscrew(l=10,d = 10, h = 50){
   difference(){
      union(){
         threaded_rod(d=d-0.1, pitch = 2, l = l,orient=LEFT,anchor=TOP);
         xcyl(d=10,h=h,anchor=RIGHT);
      }//end union
      translate([-(h+l)/2+l,0,-d+1.5])cuboid([h+l,d,d]);
      }
}//end adjscrew

