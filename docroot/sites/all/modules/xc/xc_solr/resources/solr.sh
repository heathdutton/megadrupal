#!/bin/sh
#
# chkconfig: - 80 45
# description: Starts and stops Solr

start() {
        echo -n "Starting Solr... "
        nohup ./solr.start
        echo "OK"
        return 0
}

stop() {
        echo -n "Stopping Solr... "
        ./solr.stop
        echo "OK"
        return 0
}

case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  restart)
        stop
        start
        ;;
  *)
        echo $"Usage: $0 {start|stop|restart}"
        exit 1
esac

exit $?
