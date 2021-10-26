const cms = require('./build/Release/little-cms-wrapper');

function sRGB_to_Lab(r, g, b) {
    return Float64Array.from(cms.sRGB_to_Lab(r, g, b));
}

function XYZ_to_sRGB(x, y, z) {
    return cms.XYZ_to_sRGB(x, y, z);
}

function Lab_to_sRGB(l, a, b) {
    return cms.Lab_to_sRGB(l, a, b);
}

function Lab_to_Lch(L, a, b) {
    return cms.Lab_to_Lch(L, a, b);
}

function Lch_to_Lab(L, C, h) {
    return cms.Lch_to_Lab(L, C, h);
}

module.exports = {
    getVersion: cms.GetVersion,
    sRGB_to_Lab: sRGB_to_Lab
};