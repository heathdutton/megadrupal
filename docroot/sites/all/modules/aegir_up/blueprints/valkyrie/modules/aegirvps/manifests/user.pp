define aegirvps::user (
  $username,
  $ssh_key = undef,
  $sudoaegir = false,
  $root = false,
  $shell = '/bin/bash') {

  user {$username:
    ensure  => present,
    comment => $title,
    name    => $username,
    home    => "/home/${username}",
    shell   => $shell,
    require => Group['aegir'],
  }

  file {"/home/${username}":
    ensure  => directory,
    owner   => $username,
    group   => $username,
    recurse => true,
    require => User[$username],
  }

  if $ssh_key != undef {
    file {"/home/${username}/.ssh":
      ensure  => directory,
      owner   => $username,
      group   => $username,
      require => File["/home/${username}"],
    }

    file {"/home/${username}/.ssh/authorized_keys":
      ensure  => file,
      owner   => $username,
      group   => $username,
      content => $ssh_key,
      mode    => 0600,
      require => File["/home/${username}/.ssh"],
    }
  }

  if $sudoaegir == true {
    aegirvps::sudoaegir {$username:
      username => $username,
    }
    if !defined(Group['aegir']) {
      group {'aegir':
        ensure => present,
      }
    }
    # Add user to the 'aegir' group
    User["$username"] {
      groups  => 'aegir',
    }
  }

  if $root == true {
    aegirvps::root {$username:
      username => $username,
    }
  }

}

define aegirvps::sudoaegir ($username = $title) {
  file {"/etc/sudoers.d/sudoaegir-${username}":
    ensure  => file,
    mode    => '0440',
    content => "${username} ALL=(aegir) NOPASSWD: ALL",
  }
}

define aegirvps::root ($username = $title) {
  file {"/etc/sudoers.d/root-${username}":
    ensure  => file,
    mode    => '0440',
    content => "${username} ALL=(ALL) NOPASSWD: ALL",
  }
}

# TODO: add sftp
