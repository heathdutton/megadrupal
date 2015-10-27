node "drupal8" {
  include drush-vagrant::user

  warning('Building a VM with Drupal 8.x installed. Please be patient, as this may take some time.')

  exec {'run script':
    command   => '/bin/bash /vagrant/files/install.sh',
    logoutput => true,
    timeout   => 0,
  }

}
