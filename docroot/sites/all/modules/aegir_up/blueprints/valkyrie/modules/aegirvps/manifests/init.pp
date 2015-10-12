class aegirvps {

  include aegir
  include aegir::queue_runner
  include aegirvps::utilities
  include aegirvps::apc_upload

  file { '/etc/motd':
    content => "
      Welcome to your Koumbit AegirVPS.

      AegirVPS documentation can be found at:
        https://redmine.koumbit.net/projects/aegirvps-clients.

      This AegirVPS is managed via Puppet using the manifest found in your
      private AegirVPS Git repository, located at:
        https://redmine.koumbit.net/projects/${client_id}.

      To manually force an update based on changes you've made to your AegirVPS
      Git repo, run the following command as root:
        # aegirvps_update\n\n"
  }

  # Since we're installing plugins and such from the puppetmaster, we need
  # to declare these directories, so that common::moduledir recognizes them
  # as managed, and thus doesn't remove them recursively.
  file {[
    '/var/lib/puppet/modules/munin',
    '/var/lib/puppet/modules/virtual',
    '/var/lib/puppet/modules/apt',
    ]:
    ensure => directory,
  }

}

