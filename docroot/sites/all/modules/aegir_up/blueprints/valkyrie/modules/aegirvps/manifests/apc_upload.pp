class aegirvps::apc_upload {

  Package {
    require => Exec['update_apt'],
    }

  class{['php::pear::common',
         'php::five'
        ]:
  }

  package{'php-apc':
    ensure => 'installed',
  }

  file{'/etc/php5/apache2/conf.d/apc-upload.ini':
    ensure => file,
    content => 'apc.rfc1867 = 1',
    notify => Exec['aegirvps_apache_restart'],
    require => Package['php-apc'],
  }

  exec{'aegirvps_apache_restart':
    command => '/usr/sbin/apache2ctl graceful',
    refreshonly => true,
  }

}
