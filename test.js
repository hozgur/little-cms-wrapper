const cms = require('./index.js');

console.log(cms.getVersion());

console.log(cms.sRGB_to_Lab(255,0,0));

console.log(cms.Lab_to_sRGB(50,0,0));

console.log(cms.Lab_to_Lch(50,10,10));

console.log(cms.Lch_to_Lab(50,50,180));