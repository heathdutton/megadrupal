class aegirvps::client {

## Class for building a local client with drush-vagrant/aegir-up.

  # Clone drush/provision extensions
  exec {'clone devshop_provision':
    command => '/usr/bin/git clone --recursive --branch aegirvps http://git.drupal.org/sandbox/ergonlogic/1901286.git /var/aegir/.drush/devshop_provision',
    creates => '/var/aegir/.drush/devshop_provision',
    require => File['/var/aegir'],
  }
  exec {'clone provision_git':
    command => '/usr/bin/git clone --recursive --branch 6.x-1.x http://git.drupal.org/project/provision_git.git /var/aegir/.drush/provision_git',
    creates => '/var/aegir/.drush/provision_git',
    require => File['/var/aegir'],
  }

  # Generate a persistent SSH keypair
  aegirvps::ssh_key { 'client keys' :}

  include aegir::queue_runner

  # TODO: look into hosting_site_git
}
