const cms = require('./build/Release/little-cms-wrapper');

console.log(cms.GetVersion());

module.exports = {
    test: cms.test,
    getVersion: cms.GetVersion
};