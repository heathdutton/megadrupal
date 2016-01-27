define aegir_up::ssh_key (
  $ssh_user       = 'aegir',
  $key_dir        = '/var/aegir/.ssh',
  $key_name       = 'id_rsa',
  $ssh_username   = 'gitolite',
  $ssh_host_alias = 'redmine',
  $ssh_hostname   = '209.44.112.115',
  $known_hosts    = '|1|xMV1p7vtKn7P1VW2zgTju0WhKHU=|QbgnBUQlXd5+ldB0C02kpkKy3yU= ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEAuD+ASF/UhI/pOhwt2KRN/I7xHYuCtd+mufQP2s7UJBBIMK5f8oN5CXV5AAjmYS3prsDnFM66/i3jjdL8XvYWJGcoIOdcIYi7luy8XxSHda69wuMhgruvdIEs6DJCMra91GmLB7a361YcwrIEqFW9+txaq1pY9TXdFs5qQoEp0thgbsV2GRbFhIXHZ9DJmxEplrwF6NG76MBXJwPdAceHDwomG36fyuKN4VjcUJu1pbzuGh9ECZokje1XNce5mc8Tt7jyZzfN/oUG1tUegSI0dIal/Y4p+T3P7YoDACcccIehp256sVqs/zyXhNWksHwXI16tcSL0Di9JueOyTQP9cw==
|1|7SQAQF7KHH8Z2FjOeOc9NxwHlRU=|V9UQYEaBOZ1jCM/fj0z+YLmHwB0= ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEAuD+ASF/UhI/pOhwt2KRN/I7xHYuCtd+mufQP2s7UJBBIMK5f8oN5CXV5AAjmYS3prsDnFM66/i3jjdL8XvYWJGcoIOdcIYi7luy8XxSHda69wuMhgruvdIEs6DJCMra91GmLB7a361YcwrIEqFW9+txaq1pY9TXdFs5qQoEp0thgbsV2GRbFhIXHZ9DJmxEplrwF6NG76MBXJwPdAceHDwomG36fyuKN4VjcUJu1pbzuGh9ECZokje1XNce5mc8Tt7jyZzfN/oUG1tUegSI0dIal/Y4p+T3P7YoDACcccIehp256sVqs/zyXhNWksHwXI16tcSL0Di9JueOyTQP9cw==
'){

  # All files should default to the ssh user
  File { owner => $ssh_user, group => $ssh_user,}
  # For debugging
  Exec { logoutput => on_failure,}

  if !defined(File[$key_dir]) {
    file { $key_dir :
      ensure => directory,
    }
  }

  # Generate an SSH key-pair so that root can identify to gitolite
  exec { "Generate '${key_name}' keypair" :
    command => "ssh-keygen -t rsa -N '' -C '${client_id}' -f ${key_dir}/${key_name}",
    path    => ["/usr/bin", "/usr/sbin"],
    creates => ["${key_dir}/${key_name}", "${key_dir}/${key_name}.pub"],
    require => File[$key_dir],
  }
  # SSH private keys have to be secured
  file { "Secure '${key_name}' private key" :
    path    => "${key_dir}/${key_name}",
    ensure  => file,
    mode    => '0600',
    require => Exec["Generate '${key_name}' keypair"],
  }
  if !defined(File["${key_dir}/${key_name}.pub"]) {
    file {"${key_dir}/${key_name}.pub":
      ensure  => file,
      require => Exec["Generate '${key_name}' keypair"],
    }
  }


  # Set some SSH defaults
  if !defined(File["${key_dir}/config.d"]) {
    file {"${key_dir}/config.d":
      ensure  => directory,
      require => File[$key_dir],
    }
  }
  file {"${key_dir}/config.d/${key_name}":
    ensure   => file,
    content => "Host ${ssh_host_alias}
        Hostname ${ssh_hostname}
        User ${ssh_username}
        IdentityFile ${key_dir}/${key_name}",
    require => File["${key_dir}/config.d"],
  }
  # Concatenate these fragments
  exec { "Generate ${key_dir}/config" :
    command => "/bin/cat ${key_dir}/config.d/* > ${key_dir}/config",
    require => File["${key_dir}/config.d/${key_name}"],
  }
  if !defined(File["${key_dir}/config"]) {
    file { "${key_dir}/config":
      ensure  => file,
      require => Exec["Generate ${key_dir}/config"],
    }
  }


  if $local_dev {
    # To avoid prompt to accept the remote's signature, we provide the signatures
    # of the servers we need to connect to (git.k.n & ???)
    # In production, this is handled by sshd::base, which collects keys from all
    # the Kt servers and realizes them in the /etc/sshd/ssh_known_hosts
    # TODO: manage signatures with 'line {}'
    file {"${key_dir}/known_hosts":
      ensure  => file,
      content => $known_hosts,
      require => Exec["Generate ${key_dir}/config"],
    }

    # Use existing keys, if they exist.
    if !defined(File['Restore keys script']) {
      file { 'Restore keys script' :
        content => 'if [ -e /vagrant/keys/$1 ]; then /bin/cp /vagrant/keys/$1* . ; fi',
        path    => '/usr/local/sbin/restore_ssh_keys.sh',
        mode    => 755,
      }
    }
    exec { "Use existing ${key_name} keys":
      command => "/usr/local/sbin/restore_ssh_keys.sh ${key_name}",
      cwd     => $key_dir,
      provider => 'shell',
      creates => ["${key_dir}/${key_name}", "${key_dir}/${key_name}.pub"],
      before  => Exec["Generate '${key_name}' keypair"],
      require => [ File[$key_dir],
                   File['Restore keys script'],
                 ],
    }

    # Store backups of keys.
    if !defined(File['/vagrant/keys']) {
      file { '/vagrant/keys' :
        ensure => directory,
        owner  => 'vagrant',
        group  => 'vagrant',
      }
    }
    if !defined(File['Backup keys script']) {
      file { 'Backup keys script' :
        content => 'if ! [ -e /vagrant/keys/$1 ]; then /bin/cp ./$1* /vagrant/keys/; fi',
        path    => '/usr/local/sbin/backup_ssh_keys.sh',
        owner   => 'root',
        group   => 'root',
        mode    => 755,
      }
    }
    exec { "Backup ${key_name} keys" :
      command  => "/usr/local/sbin/backup_ssh_keys.sh ${key_name}",
      cwd      => $key_dir,
      provider => 'shell',
      creates  => ["/vagrant/keys/${key_name}", "/vagrant/keys/${key_name}.pub"],
      require  => [ Exec["Generate '${key_name}' keypair"],
                   File['/vagrant/keys'],
                   File['Backup keys script'],
                 ],
    }
  }
}

