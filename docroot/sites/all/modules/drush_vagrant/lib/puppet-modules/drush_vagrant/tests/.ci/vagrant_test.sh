#!/bin/bash

cd /vagrant
chmod a+x lib/puppet-modules/drush_vagrant/tests/.ci/test.sh
./lib/puppet-modules/drush_vagrant/tests/.ci/test.sh
