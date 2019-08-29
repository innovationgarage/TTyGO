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

if which apt > /dev/null; then
    apt install -y python3 python3-pip python3-setuptools python3-dev git gcc openssh-server openssh-client bash
else
  if which opkg > /dev/null; then
    opkg install python3 python3-pip python3-setuptools python3-dev git-http gcc openssh-server openssh-client bash
  else
    echo "Unknown platform"
    exit 1
  fi
fi

mkdir -p /etc/elcheapoais
mkdir -p /var/log/elcheapoais
mkdir -p /usr/local/bin
mkdir -p /lib/elcheapoais
mkfifo /lib/elcheapoais/notifier

# Somehow, setuptools fails to install this dependency of the downsampler
python3 -m pip install click-datetime

# downsampler
(
        echo "Installing downsampler..."
        cd /tmp
        git clone https://github.com/innovationgarage/ElCheapoAIS-downsampler.git
        cd ElCheapoAIS-downsampler

        python3 setup.py install
)

(
	echo "Installing manhole..."
        cd /tmp
        git clone https://github.com/innovationgarage/ElCheapoAIS-manhole.git
        cd ElCheapoAIS-manhole

        cp manhole.sh /usr/local/bin/manhole.sh
        chmod ugo+x /usr/local/bin/manhole.sh

        if [ -e /etc/cron.d ]; then
          cp crontab /etc/cron.d/manhole
        else
          if which crontab > /dev/null; then
            crontab -u root - <<EOF
* * * * * /usr/local/bin/manhole.sh /etc/elcheapoais > /tmp/manhole.txt 2>&1
EOF
          else
            echo "Unknown system"
            exit 1
          fi
        fi
)

(
        echo "Installing notifier..."

	cd /tmp
	git clone https://github.com/innovationgarage/ElCheapoAIS-notifier.git
	cd ElCheapoAIS-notifier
	python3 setup.py install
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

mv /tmp/elcheapoais-config /etc/elcheapoais/config
cp elcheapoais-receiver.sh /usr/local/bin/elcheapoais-receiver.sh
cp elcheapoais-downsampler.sh /usr/local/bin/elcheapoais-downsampler.sh
cp elcheapoais-notifier.sh /usr/local/bin/elcheapoais-notifier.sh
chmod a+x /usr/local/bin/elcheapoais-receiver.sh /usr/local/bin/elcheapoais-downsampler.sh
cp notifier.json /etc/elcheapoais/notifier.json


if [ -e /lib/systemd ]; then
  cp elcheapoais-receiver.service /lib/systemd/system/elcheapoais-receiver.service
  cp elcheapoais-downsampler.service /lib/systemd/system/elcheapoais-downsampler.service
  cp elcheapoais-notifier.service /lib/systemd/system/elcheapoais-notifier.service
  chmod 644 /lib/systemd/system/elcheapoais-receiver.service /lib/systemd/system/elcheapoais-downsampler.service /lib/systemd/system/elcheapoais-notifier.service
else
  if [ -e /etc/rc.d ]; then
     cp elcheapoais-receiver.procd /etc/init.d/elcheapoais-receiver
     cp elcheapoais-downsampler.procd /etc/init.d/elcheapoais-downsampler
     cp elcheapoais-notifier.procd /etc/init.d/elcheapoais-notifier
  else
    echo "Unknown platform"
    exit 1
  fi
fi

# Some generic system config
# - autologin on serial console
# - Forbid password login over ssh
if [ -e /etc/systemd ]; then
  mkdir -p /etc/systemd/system/serial-getty@.service.d
  cat > /etc/systemd/system/serial-getty@.service.d/20-autologin.conf <<EOF
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin root --noclear %I $TERM
EOF
fi

sed -i -e "s+#\? *PasswordAuthentication *yes+PasswordAuthentication no+g" /etc/ssh/sshd_config

if which systemctl > /dev/null; then
  systemctl daemon-reload
  systemctl enable elcheapoais-receiver.service
  systemctl enable elcheapoais-downsampler.service
  systemctl enable elcheapoais-notifier.service
else
  if which service > /dev/null; then
    service elcheapoais-receiver enable
    service elcheapoais-downsampler enable
    service elcheapoais-notifier enable
    service cron enable
  fi
fi
