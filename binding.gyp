{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/index.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
      ],
      "conditions": [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '-std=c++11',
              '-stdlib=libc++'
            ]
          },
          "sources": [ "src/EventReceiverMac.mm" ]
        }],
        ['OS=="win"', {
          "sources": [ "src/EventReceiverWin.cc" ]
        }]
      ]
    }
  ]
}
