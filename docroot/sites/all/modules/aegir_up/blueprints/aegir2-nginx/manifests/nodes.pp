node "aegir2-nginx" {

  class { 'drush' :
    api  => 5,
  }

  class { 'aegir_up' :
    api        => 2,
    web_server => 'nginx',
    require    => Class['drush'],
  }

}
