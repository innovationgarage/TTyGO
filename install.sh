#! /bin/bash

argparse() {
   export ARGS=()
   for _ARG in "$@"; do
       if [ "${_ARG##--*}" == "" ]; then
           _ARG="${_ARG#--}"
           if [ "${_ARG%%*=*}" == "" ]; then
               _ARGNAME="$(echo ${_ARG%=*} | tr -_ )"
               eval "export ARG_${_ARGNAME}"='"${_ARG#*=}"'
           else
               eval "export ARG_${_ARG}"='True'
           fi
       else
           ARGS+=($_ARG)
       fi
   done
}

ARG_stationid="unknown"
ARG_device="/dev/ttyUSB0"
ARG_commparams="38400 cs8 -cstopb -parenb"
ARG_server="elcheapoais.innovationgarage.tech"
ARG_port="1024"
ARG_msgspersec="100"
ARG_msgspersecpermmsi="10"
ARG_manholeurl="http://elcheapoais.innovationgarage.tech:1026/manhole"

argparse "$@"

if [ "$ARG_help" ]; then
    cat <<EOF
Usage: install.sh OPTIONS
  Where options are any of

    --stationid=MMSI
    --device=/dev/ttyUSB0
    --commparams="38400 cs8 -cstopb -parenb"
      This uses stty syntax
    --server=elcheapoais.innovationgarage.tech
    --port=1024
    --msgspersec=100
    --msgspersecpermmsi=10
    --manholeurl="http://elcheapoais.innovationgarage.tech:1026/manhole"
EOF
    exit 1
fi


# General dependencies
echo Installing dependencies
apt install -y python python-pip python-dev python-setuptools

# Somehow, setuptools fails to install this dependency of the downsampler
pip install click-datetime

# downsampler
(
        cd /tmp
        git clone https://github.com/innovationgarage/ElCheapoAIS-downsampler.git
        cd ElCheapoAIS-downsampler

        echo Installing...
        python setup.py install
)

(
        cd /tmp
        git clone https://github.com/innovationgarage/ElCheapoAIS-manhole.git
        cd ElCheapoAIS-manhole

        cp manhole.sh /usr/local/bin/manhole.sh
        chmod ugo+x /usr/local/bin/manhole.sh

        cp crontab /etc/cron.d/manhole
)

cat > /tmp/elcheapoais-config <<EOF
stationid="${ARG_stationid}"
device="${ARG_device}"
commparams="${ARG_commparams}"
server="${ARG_server}"
port="${ARG_port}"
msgspersec="${ARG_msgspersec}"
msgspersecpermmsi="${ARG_msgspersecpermmsi}"
manholeurl="${ARG_manholeurl}"
EOF
	
mkdir -p /etc/elcheapoais
mkdir -p /var/log/elcheapoais

mv /tmp/elcheapoais-config /etc/elcheapoais/config
cp elcheapoais-receiver.sh /usr/local/bin/elcheapoais-receiver.sh
cp elcheapoais-downsampler.sh /usr/local/bin/elcheapoais-downsampler.sh
chmod a+x /usr/local/bin/elcheapoais-receiver.sh /usr/local/bin/elcheapoais-downsampler.sh

cp elcheapoais-receiver.service /lib/systemd/system/elcheapoais-receiver.service
cp elcheapoais-downsampler.service /lib/systemd/system/elcheapoais-downsampler.service
chmod 644 /lib/systemd/system/elcheapoais-receiver.service /lib/systemd/system/elcheapoais-downsampler.service

systemctl daemon-reload
systemctl enable elcheapoais-receiver.service
systemctl enable elcheapoais-downsampler.service
