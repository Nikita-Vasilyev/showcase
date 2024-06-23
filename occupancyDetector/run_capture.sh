#! /usr/bin/bash
tshark -i wlan1mon -a duration:300 -T fields -e wlan.sa -e wlan_radio.signal_dbm -e wlan.fc.type_subtype -E header=y -E separator=, -E quote=d -E occurrence=f > packets.csv
