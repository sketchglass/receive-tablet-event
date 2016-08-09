{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/addon.cc", "src/addon_mac.mm" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
