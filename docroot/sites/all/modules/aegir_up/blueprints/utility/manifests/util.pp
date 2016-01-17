# A Puppet manifest to provision a utility server

Exec { path  => [ "/bin/", "/sbin/" , "/usr/bin/", "/usr/sbin/" ] }


import "common"
include apt

include drush-vagrant::users

package { ['squid3', 'bind9'] :
  ensure => present;
}

package { 'jenkins':
  ensure  => present,
  require => [ Apt::Sources_list['jenkins'],
               Exec['update_apt'],
             ],
}

apt::sources_list {'jenkins':
  content => 'deb http://pkg.jenkins-ci.org/debian binary/',
}

