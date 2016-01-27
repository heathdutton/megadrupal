# A Puppet module to add some Aegir-up-specific config

class aegir_up (
  $api          = 1,
  $dist         = 'squeeze',
  $web_server   = 'apache',
  $quiet        = false
  ) {
#  $frontend_url = $aegir::defaults::frontend_url,
#  $db_host      = $aegir::defaults::db_host,
#  $db_user      = $aegir::defaults::db_user,
#  $db_password  = $aegir::defaults::db_password,
#  $admin_email  = $aegir::defaults::admin_email,
#  $makefile     = $aegir::defaults::makefile,
#  $apt          = $aegir::defaults::apt,
#  $db_server    = $aegir::defaults::db_server,
  notice("\n
          Running Puppet manifests to install and/or update Aegir.\n
          This may take awhile, so please be patient.
          For more detail on the operations being run, edit settings.rb,
          and set 'verbose = 1'.")

  # Set some defaults, and make output less verbose
  if $quiet {
    Group { loglevel => 'info' }
    Package { loglevel => 'info' }
    Notify { loglevel => 'info' }
    User { loglevel => 'info' }
    File { mode => 0644, loglevel => 'info' }
  }

  Exec { path => [ "/bin/", "/sbin/" , "/usr/bin/", "/usr/sbin/" ] }

  file { '/etc/motd':
    content => "\n
                Welcome to your Aegir virtual machine!\n
                Built by Vagrant. Managed by Puppet.\n
                Developed and maintained by Praxis Labs Coop.\n"
  }

  class { 'aegir' :
#    frontend_url => $frontend_url,
#    db_host      => $db_host,
#    db_user      => $db_user,
#    db_password  => $db_password,
#    admin_email  => $admin_email,
#    makefile     => $makefile,
    api          => $api,
#    apt          => $apt,
    dist         => $dist,
    db_server    => $db_server,
    web_server   => $web_server,
    secure_mysql => true,
  }

  if $api == 1 {
    class { 'aegir::queue_runner':
      before => Exec['aegir-up login'],
    }
  }
  include drush_vagrant::users
  drush_vagrant::user { 'aegir':
    home_dir => '/var/aegir',
    require  => Class['aegir'],
    notify   => Exec['chsh aegir -s /bin/bash'],
  }
  exec { 'chsh aegir -s /bin/bash':
    refreshonly => true,
    path        => ['/bin', '/usr/bin'],
  }

  exec {'aegir-up login':
    command     => "\
echo '*******************************************************************************'\n
echo '* Open the link below to access your new Aegir site:'\n
echo '*' `drush @hostmaster uli`\n
echo '*******************************************************************************'\n
",
    loglevel    => 'alert',
    logoutput   => true,
    user        => 'aegir',
    environment => 'HOME=/var/aegir',
  }

}
