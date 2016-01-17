class aegir (
  $frontend_url = $aegir::defaults::frontend_url,
  $db_host      = $aegir::defaults::db_host,
  $db_user      = $aegir::defaults::db_user,
  $db_password  = $aegir::defaults::db_password,
  $admin_email  = $aegir::defaults::admin_email,
  $makefile     = $aegir::defaults::makefile,
  $api          = $aegir::defaults::api,
  $dist         = $aegir::defaults::dist,
  $db_server    = $aegir::defaults::db_server,
  $secure_mysql = $aegir::defaults::secure_mysql,
  $web_server   = $aegir::defaults::web_server,
  $ensure       = $aegir::defaults::ensure
  ) inherits aegir::defaults {


  case $api {
    2: {
      $real_api = 2
      package { 'aegir':
        ensure => absent;
      }
      # Ensure we allow Drush to upgrade. This is essentially here to remove
      # files (since deprecated) from the old puppet-drush module.
      file { [
        "/etc/apt/preferences.d/drush.pref",
        "/etc/apt/preferences.d/drush-squeeze.pref",
        ] :
        ensure => absent,
        notify  => Exec['aegir_update_apt'],
      }
      # Only install Drush if it hasn't already been.
      if !defined(Class['drush']) and !defined(Class['drush::git::drush']) {
        include drush
      }
      # While the .deb intalls the init script for hosting_queued, it doesn't
      # enable it by default
      drush::en { 'hosting_queued':
        refreshonly => true,
        subscribe   => Package["aegir${real_api}"],
        before      => Service['hosting-queued'],
      }
      service { 'hosting-queued':
        ensure  => running,
        require => Package["aegir${real_api}"],
      }
    }
    1, '': {
      $real_api = ''
      class{ 'drush':
        api => 4,
      }
    }
    default: {
      warning("'${api}' is not a valid Aegir API version. Values can be '1' or '2'. Defaulting to '1'.")
      $real_api = ''
    }
  }

  if $dist {
    class { 'aegir::apt' :
      dist => $dist,
    }
  }

  # Install database server.
  if $db_server != false {
    case $db_server {
      'mariadb': { /* To do */ }
      'mysql': {
        # While mysql would be installed by default anyway, we do it here to
        # allow us to secure it before installing Aegir.
        package {'mysql-server': ensure => present, }
        service {'mysql':
          ensure  => running,
          require => Package['mysql-server'],
        }
      }
      default: { /* Do nothing. */ }
    }
  }

  if $web_server != false {
    case $web_server {
      'nginx': {
        package { ['nginx', 'php5-fpm']:
          ensure  => present,
          require => Exec["aegir_update_apt"],
        }
        service { 'nginx' :
          ensure => running,
          require => Package['nginx'],
          before  => Package["aegir${real_api}"],
        }
      }
      'apache2', default: { /* apache2 will be installed as a dependency of the aegir packages. */ }
    }
  }

  Aegir::Apt::Debconf { before => Package["aegir${real_api}"] }
  if $frontend_url { aegir::apt::debconf { "aegir/site string ${frontend_url}": } }
  if $db_host      { aegir::apt::debconf { "aegir/db_host string ${db_host}": } }
  if $db_user      { aegir::apt::debconf { "aegir/db_user string ${db_user}": } }
  if $db_password  { aegir::apt::debconf { "aegir/db_password string ${db_password}": } }
  if $admin_email  { aegir::apt::debconf { "aegir/email string ${admin_email}": } }
  if $makefile     { aegir::apt::debconf { "aegir/makefile string ${makefile}": } }
  if $web_server   { aegir::apt::debconf { "aegir/webserver string ${web_server}": } }

  if $secure_mysql {
    # Equivalent to /usr/bin/mysql_secure_installation without providing or setting a password
    # From: http://matthewturland.com/2012/02/13/setting-up-ec2-for-drupal-with-puppet/
    exec { 'mysql_secure_installation':
        command => '/usr/bin/mysql -uroot -e "DELETE FROM mysql.user WHERE User=\'\'; DELETE FROM mysql.user WHERE User=\'root\' AND Host NOT IN (\'localhost\', \'127.0.0.1\', \'::1\'); DROP DATABASE IF EXISTS test; FLUSH PRIVILEGES;" mysql',
      subscribe   => Package['mysql-server'],
      before      => Package["aegir${real_api}"],
    }
  }

  package { "aegir${real_api}":
    ensure       => $ensure,
    responsefile => template('aegir/aegir.preseed.erb'),
    require      => Class['aegir::apt'],
  }

}
