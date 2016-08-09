{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/addon.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        ['OS=="mac"', {
          "sources": [ "src/addon_mac.mm" ]
        }],
        ['OS=="win"', {
          "sources": [ "src/addon_win.cc" ]
        }]
      ]
    }
  ]
}
