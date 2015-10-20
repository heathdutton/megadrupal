node "aegir3-dev" {

  class { 'aegir::dev' :
    hostmaster_ref  => '7.x-3.x',
    provision_ref   => '7.x-3.x',
    #hostmaster_repo => 'http://git.drupal.org/project/hostmaster.git',
    #provision_repo  => 'http://git.drupal.org/project/provision.git',
    #frontend_url    => 'aegir.local',
    #db_host         => 'localhost',
    #db_user         => 'root',
    #db_password     => 'pwd',
    #admin_email     => 'test@ergonlogic.com',
    #admin_name      => 'aegir_admin',
    #makefile        => '/vagrant/makefiles/aegir-dev.make',
    #aegir_user      => 'aegir',
    #aegir_root      => '/var/aegir',
    #web_group       => 'www-data',
    #db_server       => 'mysql',
    #web_server      => 'nginx',
    #update          => true,
    platform_path   => '/var/aegir/hostmaster-7.x-3.x',
  }

  include drush_vagrant::users
  drush_vagrant::user { 'aegir':
    home_dir => '/var/aegir',
    require  => Class['aegir::dev'],
    notify   => Exec['chsh aegir -s /bin/bash'],
  }
  exec { 'chsh aegir -s /bin/bash':
    refreshonly => true,
    path        => ['/bin', '/usr/bin']
  }

}
