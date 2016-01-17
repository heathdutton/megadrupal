# Global variables
# TODO: re-factor puppet-aegir so as to pass these as parameters
$aegir_makefile = '/vagrant/makefiles/valkyrie_hostmaster.make'
$aegir_web_group = 'www-data'
$aegir_dev_build = true
$aegir_provision_repo = 'http://git.drupal.org/project/provision.git'
$aegir_provision_branch = '6.x-1.x'
$aegir_profile = 'hostmaster'
$local_dev = true

node 'client' {

  include aegir_up
  include drush-vagrant::users
  drush-vagrant::user {'aegir':
    home_dir => '/var/aegir',
  }

  include valkyrie::client

  drush::en { 'valkyrie' :
    site_path => "/var/aegir/hostmaster-6.x-1.x/sites/${fqdn}",
  }

}

node 'server' {

  include aegir_up
  include drush-vagrant::users
  drush-vagrant::user {'aegir':
    home_dir => '/var/aegir',
  }

  include valkyrie::client

  drush::en { 'valkyrie' :
    site_path => "/var/aegir/hostmaster-6.x-1.x/sites/${fqdn}",
  }

}
