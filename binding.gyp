{
  "targets": [
    {
      "target_name": "tablet_event_interceptor",
      "sources": [ "addon.mm" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
