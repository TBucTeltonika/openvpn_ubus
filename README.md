# openvpn_ubus
Package for RUTOS to manage openvpn through ubus.

# Installation
Your openvpn package needs to have `--management` command option to enable telnet management which this package uses.
You can do so using build configuration menu or recompile package only with parameter enabled manually.

When installing you need to use --force-overwrite command eg. `opkg install openvpn_ubus.ipkg --force-overwrite`.
This will install the package and overwrite openvpn init file.

# Usage
If program was installed correctly all you need to do is add `list management 'IP PORT'`, usually `list management '127.0.0.1 5555'` to your server configuration in the uci openvpn config file config file in `\etc\config\`.

This program will automatically launch as service for each server configuration which has management enabled and is enabled itself.
You can test if it started correctly using `ubus -v list | grep ovpn.`. The ubus interface name should be in the format of ovpn.CONFIGURATION_NAME.

# commands
`get` - Get all connect clients in json array.

`kill client` - Kill client. example: `kill '{"client":"user123"}'`
