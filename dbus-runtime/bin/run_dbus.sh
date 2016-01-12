if [ -e /usr/local/bin/dbus-daemon ]
then
mkdir -p /var/run/dbus
mkdir -p /var/lib/dbus
dbus-uuidgen > /var/lib/dbus/machine-id
dbus-daemon --system --fork
fi

if which dbus-launch >/dev/null && test -z "$DBUS_SESSION_BUS_ADDRESS"; then
    eval `dbus-launch --sh-syntax --exit-with-session`
fi

