node "drupal7" {
  include drush-vagrant::user

  exec {'run script':
    command   => '/bin/bash /vagrant/files/install.sh',
    logoutput => true,
  }


}
