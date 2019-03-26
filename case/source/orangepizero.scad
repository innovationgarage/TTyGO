// Orange pi zero mount for the tandydrone
// By: R. Sanford
// Date: 4 Feb, 2019

OPI();

// Modules =====================================================================

module OPI(){
  // main board, shape and holes
  difference(){
    color([0,0.6,.9]) minkowski(){
      dia = 3;
      cube([48.25-dia, 46.25-dia, 1.65], center = true); // board size
      cylinder(d = dia, h = 0.1, center = true, $fn = 24); // shaping
    }
    for(i = [-1:0], j = [-1:2:1]) translate([21*cos(180*i), 20.25*sin(90*j), 0]) cylinder(d = 3, h = 3, center = true, $fn = 36);//holes, with sinusoidal positioning
  }
  // Eth Connector
  translate([19.725, -0.5, 1.65/2+6.5]) cube([16, 16.5, 13], center = true);
  // USB-A Connector
  translate([20.7, 13.45, 13.6/2+1.65/2]) cube([14, 6.75, 13.6], center = true);
  // USB & System Headers
  translate([0, 21.855, 0]) headers254(13, 0);
  // USB miniB: Power in
  translate([-21.725, 12.9, 2.35]) color([0.7,0.7,0.7]) cube([5.82, 7.75, 3.04], center=true);
  // GPIO Headers
  translate([0, -20.585, 0]) headers254(13, 1);
  // Serial Connector
  translate([20.35, -13.625, 0]) headers254(2, 0);
  // SD Card & Holder
  translate([-16.824, -4.05, -1.85]) cube([14.65, 14.6, 2.05], center=true);
  translate([-19.574, -4.91, -1.85]) color([.35, .35, .35]) cube([15, 11.5, 1], center=true);
  // CPU:Allwinner H2, 7x9
  translate([-10.075, 7.075, 1.465]) color([.25,.25,.25]) cube([14.1,14.1,1.25],center=true);
  // RAM module
  translate([-11.3125, -7.565, 1.465]) color([.25,.25,.25]) cube([13.3, 7.55, 1.25],center=true);
  // Isolator
  translate([5.875, -2.4, 3.575]) color([.25,.25,.25]) cube([6.87,12.5,5.5],center=true);
}

module headers254(m=0,n=0){
  translate([-(m*2.54)/2, -(n*2.54)/2, 0]){
    for(i=[0:m],j=[0:n]){
      translate([i*2.54, j*2.54]){
        translate([0,0,3.3]) cube([0.65,0.65,11.75], center = true);
        translate([0,0,2.1]) rotate([0, 0, 22.5]) color([0.4,0.4,0.4]) cylinder(d=2.75, h=2.54, $fn=8, center=true);
      }
    }
  }
}
