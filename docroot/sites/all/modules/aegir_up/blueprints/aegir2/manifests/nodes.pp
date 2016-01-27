node "aegir2" {

  class { 'drush' :
    api  => 5,
  }

  class { 'aegir_up' :
    api     => 2,
    require => Class['drush'],
  }

}
