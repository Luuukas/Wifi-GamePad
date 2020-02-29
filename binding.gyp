{

  "targets": [

    {

      "target_name": "win_sendkey",

      "sources": [ "win_sendkey.cc" ],

      "include_dirs": [

        "<!(node -e \"require('nan')\")"

      ]

    }

  ]

}