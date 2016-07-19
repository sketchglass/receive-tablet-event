{
  "targets": [
    {
      "target_name": "intercept_tablet_event",
      "sources": [ "addon.mm" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
