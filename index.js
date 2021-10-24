const cms = require('./build/Release/little-cms-wrapper');

function sRGB_to_Lab(r, g, b) {
    return cms.sRGB_to_Lab(r, g, b);
}

function XYZ_to_sRGB(x, y, z) {
    return cms.XYZ_to_sRGB(x, y, z);
}

function Lab_to_sRGB(l, a, b) {
    return cms.Lab_to_sRGB(l, a, b);
}

function XYZ_to_sRGB(inBuffer, outBuffer) {
    return cms.XYZ_to_sRGB(inBuffer, outBuffer);
}

function sRGB_to_Lab(inBuffer, outBuffer) {
    return cms.sRGB_to_Lab(inBuffer, outBuffer);
}

function Lab_to_sRGB(inBuffer, outBuffer) {
    return cms.Lab_to_sRGB(inBuffer, outBuffer);
}

function Lab_to_Lch(L, a, b) {
    return cms.Lab_to_Lch(L, a, b);
}

function Lch_to_Lab(L, C, h) {
    return cms.Lch_to_Lab(L, C, h);
}

function Lab_to_Lch(inBuffer, outBuffer) {
    return cms.Lab_to_Lch(inBuffer, outBuffer);
}

function Lch_to_Lab(inBuffer, outBuffer) {
    return cms.Lch_to_Lab(inBuffer, outBuffer);
}

module.exports = {
    getVersion: cms.GetVersion,
    sRGB_to_Lab: cms.sRGB_to_Lab
};