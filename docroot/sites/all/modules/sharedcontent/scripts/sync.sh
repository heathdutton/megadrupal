#!/bin/sh
# Triggers the synchronization on all clients.

IFS=',' ;for client in `echo "${SC_CLIENTS}"`
do
  cd ${BUILD_TARGET}/sites/${client}

  # Sync client
  drush sc-sync
  drush cron
done
