const cms = require('./build/Release/little-cms-wrapper');

function sRGB_to_Lab(r, g, b) {
    if(ArrayBuffer.isView(r) || Array.isArray(r)) {
        return sRGB_to_Lab(r[0], r[1], r[2]);
    }
    if(r.r !== undefined) {
        return sRGB_to_Lab(r.r, r.g, r.b);
    }
    let lab = cms.sRGB_to_Lab(r, g, b);
    return {
        L: lab[0],
        a: lab[1],
        b: lab[2]
    };
}


function Lab_to_sRGB(l, a, b) {
    if(ArrayBuffer.isView(l) || Array.isArray(l)) {
        return Lab_to_sRGB(l[0], l[1], l[2]);
    }
    if(l.L !== undefined) {
        return Lab_to_sRGB(l.L, l.a, l.b);
    }
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

function Lab_to_Lab_encoded(l,a,b) {
    if(ArrayBuffer.isView(l) || Array.isArray(l)) {
        return cms.Lab_to_Lab_encoded(l);
    }
    if(l.L !== undefined) {
        return cms.Lab_to_Lab_encoded(Float64Array.from([l.L, l.a, l.b]));
    }
    let lab = cms.Lab_to_Lab_encoded([l, a, b]);
    return {
        L: lab[0],
        a: lab[1],
        b: lab[2]
    };
}

function Lab_encoded_to_Lab(l,a,b) {
    if(ArrayBuffer.isView(l) || Array.isArray(l)) {
        return cms.Lab_encoded_to_Lab(l);
    }
    if(l.L !== undefined) {
        return cms.Lab_encoded_to_Lab(Float64Array.from([l.L, l.a, l.b]));
    }
    return cms.Lab_encoded_to_Lab(Float64Array.from([l, a, b]));
}

function sRGB_to_Lab_encoded(r,g,b) {
    if(ArrayBuffer.isView(r) || Array.isArray(r)) {
        return cms.sRGB_to_Lab_encoded(Uint8Array.from(r));
    }

    if(r.r !== undefined) {
        return cms.sRGB_to_Lab_encoded(Uint8Array.from([r.r, r.g, r.b]));
    }
    return cms.sRGB_to_Lab_encoded(Uint8Array.from([r, g, b]));
}

function Lab_encoded_to_sRGB(l,a,b) {
    if(ArrayBuffer.isView(l) || Array.isArray(l)) {
        return cms.Lab_encoded_to_sRGB(l);
    }
    if(l.L !== undefined) {
        return cms.Lab_encoded_to_sRGB(Uint16Array.from([l.L, l.a, l.b]));
    }
    let rgb = cms.Lab_encoded_to_sRGB(Uint16Array.from([l, a, b]));
    return {
        r: rgb[0],
        g: rgb[1],
        b: rgb[2]
    };
}
 
module.exports = {
    getVersion: cms.GetVersion,
    sRGB_to_Lab,
    Lab_to_sRGB,
    Lab_to_Lch,
    Lch_to_Lab,
    sRGB_to_Lab_encoded,
    Lab_encoded_to_sRGB,
    Lab_to_Lab_encoded,
    Lab_encoded_to_Lab,
};