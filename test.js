const cms = require('./index.js');

console.log(cms.getVersion());

console.log(cms.sRGB_to_Lab(255,0,0));

console.log(cms.Lab_to_sRGB(54.29,80.8,70.0));

console.log(cms.Lab_to_Lch(50,10,10));

console.log(cms.Lch_to_Lab(50,50,180));

let rgbData = [
    255,0,0,
    0,255,0,
    0,0,255,
    255,255,255,
    254,254,254,
    0,0,0,
    1,1,1,
    255,255,0,
    0,255,255,
    255,0,255,
];

console.log("sRGB_to_Lab_encoded");
let labEncoded = cms.sRGB_to_Lab_encoded(Buffer.from(rgbData));
for(let i=0;i<labEncoded.length;i+=3){
    console.log(labEncoded[i],labEncoded[i+1],labEncoded[i+2]);
}

console.log("Lab_encoded_to_Lab: ");
let lab = cms.Lab_encoded_to_Lab(labEncoded);
for(let i=0; i<lab.length/3; i++){
    console.log(lab[i*3].toPrecision(3),lab[i*3+1].toPrecision(3),lab[i*3+2].toPrecision(3));
}

let rgbData2 = cms.Lab_encoded_to_sRGB(labEncoded);
console.log("Lab_encoded_to_sRGB: ");
for(let i=0; i<rgbData2.length/3; i++){
    console.log(rgbData[i*3],rgbData[i*3+1],rgbData[i*3+2], " - ",rgbData2[i*3],rgbData2[i*3+1],rgbData2[i*3+2]);
}



