const cms = require('./build/Release/little-cms-wrapper');

function sRGB_to_Lab(r, g, b) {
    let lab = cms.sRGB_to_Lab(r, g, b);
    return {
        L: lab[0],
        a: lab[1],
        b: lab[2]
    };
}

function Lab_to_sRGB(l, a, b) {
    let rgb = cms.Lab_to_sRGB(l, a, b);
    return {
        r: rgb[0],
        g: rgb[1],
        b: rgb[2]
    };
}


function Lab_to_Lch(L, a, b) {
    let lch = cms.Lab_to_Lch(L, a, b);
    return {
        L: lch[0],
        C: lch[1],
        h: lch[2]
    };
}

function Lch_to_Lab(L, C, h) {
    let lab = cms.Lch_to_Lab(L, C, h);
    return {
        L: lab[0],
        a: lab[1],
        b: lab[2]
    };
}



module.exports = {
    getVersion: cms.GetVersion,
    sRGB_to_Lab: sRGB_to_Lab,
    Lab_to_sRGB: Lab_to_sRGB,
    Lab_to_Lch: Lab_to_Lch,
    Lch_to_Lab: Lch_to_Lab,
    sRGB_to_Lab_encoded: cms.sRGB_to_Lab_encoded,
    Lab_encoded_to_sRGB: cms.Lab_encoded_to_sRGB,
    Lab_to_Lab_encoded: cms.Lab_to_Lab_encoded,
    Lab_encoded_to_Lab: cms.Lab_encoded_to_Lab,
};