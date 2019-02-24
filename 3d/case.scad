$fn= 100;
da= 40;

module platine()
{
  difference()
  {
    translate([-32/2,-41/2,0]) cube([32,41,1.5]);
    
    translate([ 13, 17.5,-1]) cylinder(d=3.1, h=3);
    translate([-13, 17.5,-1]) cylinder(d=3.1, h=3);
    translate([ 13,-17.5,-1]) cylinder(d=3.1, h=3);    
  }
  
  color("blue") translate([-23/2,16-41/2,13]) cube([23,15,1]);
  
  color("darkgrey") translate([14.5-32/2,-.5-41/2,1.5]) cube([9,15,11]);
  
  color("blue") translate([-15/2,41/2-9,1.5]) cube([15,8,10]);
}

module gehaeuse()
{
  translate([0, -23,-1]) rotate([-90,0,0]) difference()
  {
    cylinder(d=da,h=50);
    
    translate([0,0,2]) cylinder(d=da-3,h=60);
    translate([-da/2-1,0,-1]) cube([da+2, da/2+2, 60]);
    
    translate([14-32/2,-13,-2]) cube([10,12,15]);
  }
  difference()
  {
    translate([-da/2,-23,-4]) cube([da,50,3.1]);
    
    translate([-34/2,-21,-2]) cube([34,50,4]);
    *translate([-34/2,-21,-1.25]) cube([34,50,2]);
  }
  difference()
  {
    union()
    {
      translate([ 1-da/2,-23,-2]) cube([4,50,4]);
      translate([-5+da/2,-23,-2]) cube([4,50,4]);
    }
    translate([-34/2,-21,-1.25]) cube([34,50,2]);
  }
  
}

module deckel()
{
  translate([0, 27.25,-1]) 
  {
    rotate([-90,0,0]) difference()
    {
      union()
      {
        cylinder(d=da,h=2);
        translate([0,0,-5]) cylinder(d=da-3.5,h=5);
      }
      
      translate([0,0,-6]) cylinder(d=da-6,h=6);
      translate([-da/2-1,0,-10]) cube([da+2, da/2+2, 20]);
      
      translate([-da/2-1,-3.1,-6]) cube([da+2, 8, 6]);
      
      translate([0,-6,-1]) cylinder(d=6.5, h=10);
    }
    translate([-da/2, 0, -3]) cube([da,2,3.1]);
    translate([5.2-da/2, -5, -0.8]) cube([da-10.4,5,1.5]);
  }
}

//rotate([-90,0,0]) 
deckel();

//rotate([90,0,0]) 
difference()
{
gehaeuse();

*translate([-25,-25,-10]) cube([50,48,30]);
}
*translate([0,0,-1]) platine();


