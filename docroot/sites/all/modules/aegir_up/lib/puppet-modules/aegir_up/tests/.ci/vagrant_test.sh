#!/bin/bash

cd /vagrant
chmod a+x lib/puppet-modules/aegir_up/tests/.ci/test.sh
./lib/puppet-modules/aegir_up/tests/.ci/test.sh
