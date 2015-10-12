node "aegir2" {

  class { 'drush' :
    api  => 5,
  }

  class { 'aegir_up' :
    api     => 2,
    dist    => 'unstable',
    require => Class['drush'],
  }

}
