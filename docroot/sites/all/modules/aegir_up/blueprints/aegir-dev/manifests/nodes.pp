node "aegir-dev" {

# Optional settings to install Drush from git repos
  $drush_dev_build = true
  $drush_git_branch = '7.x-5.x'
#  $drush_git_tag = '5.7'

# Optional settings for Aegir front-end
#  $aegir_hostmaster_url = '<project>.aegir.local'
#  $aegir_db_host = 'db.aegir.local'
#  $aegir_db_user = 'root'
#  $aegir_db_password = 'password'
#  $aegir_email = 'test@ergonlogic.com'
  $aegir_makefile = '/vagrant/makefiles/custom-aegir.make'
#  $aegir_force_login_link = 'true'    # Print a login link each time the manifest is run

# Additional optional settings available since $aegir_dev_build = TRUE

#  $aegir_version = '6.x-1.9'
#  $aegir_drush_make_version = '6.x-2.3'
#  $aegir_http_service_type = 'apache' 
  $aegir_web_group = 'www-data'
#  $aegir_debug = true

# Build 'manually' using git repos
  $aegir_dev_build = true
  $aegir_provision_repo = 'http://git.drupal.org/project/provision.git'
  $aegir_provision_branch = '6.x-2.x'

  $aegir_profile = 'hostmaster'

  include aegir_up
  include drush-vagrant::users
  drush-vagrant::user {'aegir':
    home_dir => '/var/aegir',
  }
}

