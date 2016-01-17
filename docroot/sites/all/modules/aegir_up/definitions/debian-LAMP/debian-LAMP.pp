# A minimal Puppet manifest to provision a basic LAMP server

notice("\n
        Running Puppet manifests to install LAMP stack.\n")

package { [ 'apache2', 'mysql-server', 'php5', 'php5-mysql', 'php5-cli', 'php5-gd', 'git-core', 'rsync' ]: }

group { 'puppet': ensure => present, }

file { '/etc/motd':
  content => "Welcome to your LAMP virtual machine!
              Built by Vagrant. Managed by Puppet.\n
              Developed and maintained by Ergon Logic Enterprises.\n",
     }
