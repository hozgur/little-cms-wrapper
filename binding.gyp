{
  'targets': [

    {
      'target_name': 'binding',
      'type': 'static_library',            
      'sources': [ 
        './src/cmsalpha.c',
        './src/cmscam02.c',
        './src/cmscgats.c',
        './src/cmscnvrt.c',
        './src/cmserr.c',
        './src/cmsgamma.c',
        './src/cmsgmt.c',
        './src/cmshalf.c',
        './src/cmsintrp.c',
        './src/cmsio0.c',
        './src/cmsio1.c',
        './src/cmslut.c',
        './src/cmsmd5.c',
        './src/cmsmtrx.c',
        './src/cmsnamed.c',
        './src/cmsopt.c',        
        './src/cmspack.c',
        './src/cmspcs.c',
        './src/cmsps2.c',
        './src/cmsplugin.c',
        './src/cmssamp.c',
        './src/cmssm.c',
        './src/cmstypes.c',
        './src/cmsvirt.c',
        './src/cmswtpnt.c',
        './src/cmsxform.c',
         ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    },


    {
      'target_name': 'little-cms-wrapper',
      'sources': [ 
        'main.cpp',
         ],
      'include_dirs': [ './src' ],
      'libraries': [ 'Release/binding.lib' ],
      'dependencies': [
          'little-cms',
        ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}