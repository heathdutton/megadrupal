# A minimal Puppet manifest to provision a basic Debian server

notice("\n
        Running Puppet manifests to install basic packages.\n")

package { [ 'git-core', 'vim' , 'screen', 'htop' ]: }

group { 'puppet': ensure => present, }

file { '/etc/motd':
  content => "Welcome to your Debian virtual machine!
              Built by Vagrant. Managed by Puppet.\n
              Developed and maintained by Ergon Logic Enterprises.\n",
     }
