class aegirvps::utilities {

  package { ['htop',
            ]:
    ensure => "installed"
  }

}
