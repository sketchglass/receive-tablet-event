{
  "targets": [
    {
      "target_name": "receive_tablet_event",
      "sources": [ "addon.mm" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
