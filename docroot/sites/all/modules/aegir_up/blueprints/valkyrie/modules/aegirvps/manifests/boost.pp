class aegirvps::boost {

  exec {'Download provision_boost':
    path        => [ "/bin/", "/sbin/" , "/usr/bin/", "/usr/sbin/" ],
    cwd     => '/var/aegir',
    user    => 'aegir',
    group   => 'aegir',
    environment => "HOME=/var/aegir",
    command => 'drush -y dl provision_boost --destination=/var/aegir/.drush',
    creates => '/var/aegir/.drush/provision_boost',
  }

}
