class valkyrie {

  # Clone drush/provision extensions
  exec {'clone devshop_provision':
    command => '/usr/bin/git clone --recursive --branch aegirvps http://git.drupal.org/project/devshop_provision.git /var/aegir/.drush/devshop_provision',
    creates => '/var/aegir/.drush/devshop_provision',
    require => File['/var/aegir'],
  }
  exec {'clone provision_git':
    command => '/usr/bin/git clone --recursive --branch 6.x-1.x http://git.drupal.org/project/provision_git.git /var/aegir/.drush/provision_git',
    creates => '/var/aegir/.drush/provision_git',
    require => File['/var/aegir'],
  }

}

class valkyrie::client {

  # Generate a persistent SSH keypair
  aegirvps::ssh_key { 'client keys' :}

  include valkyrie
  include aegir::queue_runner

}
