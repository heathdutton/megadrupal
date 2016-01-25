class aegirvps::remote_import($importer = false, $source = false) inherits aegir::defaults {

  if $importer == true {
    include aegirvps::remote_import::importer
  }

  if $source == true {
    include aegirvps::remote_import::source
  }

}

class aegirvps::remote_import::git_transport {

  # We're using the client git repo to transport SSH keys. Its location depends
  # on whether we're on a production server, or in a local Vagrant VM.
  $dev_prod = $virtual ? {
    'xenu'       => "prod",
    'virtualbox' => "dev",
    # Puppet/Facter seems to think our Xen servers are 'physical'?!
    default      => "prod",
  }
  $repo_dir = $dev_prod ? {
    'prod' => "/etc/aegirvps",
    'dev'  => "/vagrant",
  }
  $files_dir = "${repo_dir}/files"
  $key_dir = $dev_prod ? {
    'prod' => "${files_dir}/keys",
    'dev'  => "${files_dir}/test_keys",
  }
  file {"${files_dir}":
    ensure => directory,
  }
  file {"${key_dir}":
    ensure => directory,
    require => File["${files_dir}"],
  }

}

class aegirvps::remote_import::modules {

  # See http://drupal.org/node/1663066#comment-6227464
  exec {'remote_import':
    command => "git clone --recursive --branch 1663066 http://git.drupal.org/sandbox/ergonlogic/1681684.git remote_import",
    cwd     => "/usr/share/drush/commands/provision/",
    user    => 'root',
    group   => 'root',
    creates => "/usr/share/drush/commands/provision/remote_import",
    require => $aegir_installed,
  }

  exec {'hosting_remote_import':
    command => "git clone --recursive --branch 6.x-1.x http://git.drupal.org/project/hosting_remote_import.git",
    user   => 'aegir',
    group  => 'aegir',
    cwd     => "${aegir_root}/hostmaster-${aegir_version}/sites/${aegir_hostmaster_url}/modules",
    creates => "${aegir_root}/hostmaster-${aegir_version}/sites/${aegir_hostmaster_url}/modules/hosting_remote_import",
    require => [ $aegir_installed,
                 Exec['remote_import'],
               ],
    notify     => Drush::En['hosting_remote_import'],
  }

  drush::en { 'hosting_remote_import':
    site_path  => "${aegir_root}/hostmaster-${aegir_version}/sites/${aegir_hostmaster_url}",
    log        => "${aegir_root}/drush.log",
    require    => Exec['hosting_remote_import'],
  }

}

class aegirvps::remote_import::importer inherits aegirvps::remote_import::git_transport {

  include aegirvps::remote_import::modules

  # Generate an SSH key-pair so that aegir can login to the source server
  file {"/var/aegir/.ssh/":
    ensure => directory,
    owner  => 'aegir',
    group  => 'aegir',
    require => $aegir_installed,
    before => Exec['aegir_id_rsa'],
  }
  exec {"aegir_id_rsa":
    command => "ssh-keygen -t rsa -N '' -C '${client_id}' -f /var/aegir/.ssh/id_rsa",
    user    => 'aegir',
    path    => ["/usr/bin", "/usr/sbin"],
    creates => ["/var/aegir/.ssh/id_rsa", "/var/aegir/.ssh/id_rsa.pub"],
  }

  # put the pub key in the client's repo, so it can be deployed on the source end in auth_keys
  file {"${key_dir}/aegir_id_rsa.pub":
    ensure  => present,
    source  => '/var/aegir/.ssh/id_rsa.pub',
    require => [ Exec['aegir_id_rsa'],
                 File["${key_dir}"],
               ],
  }
  if $dev_prod == 'prod' {
    exec {'git add aegir_id_rsa.pub':
      command     => "git add ${key_dir}/aegir_id_rsa.pub && git commit -m\"Add aegir user's public key, so that it can be added to the source server's authorized keys.\" && git push",
      cwd         => $repo_dir,
      refreshonly => true,
      subscribe   => File["${key_dir}/aegir_id_rsa.pub"],
    }
  }

  # Remote import uses the hostname of the source server to connect to mysql.
  # In dev env, we need to make sure that resolves locally.
  if $dev_prod == 'dev' {
    exec {'add dev to /etc/hosts':
      command => "cat ${key_dir}/dev_hosts >> /etc/hosts",
      unless  => "grep '### Remote-import source ###' /etc/hosts",
      user    => 'root',
      onlyif  => "ls ${key_dir}/dev_hosts",
    }
  }

  # Add the source server's key to our known_hosts
  sshkey {'add-source-to-known_hosts':
    ensure => present,
    # We add a second file here as a default, since file() will cause the whole
    # Puppet run to fail if the file it's trying to read from doesn't exist yet
    key    => file("${key_dir}/ssh_host_rsa_key.pub", "/etc/ssh/ssh_host_rsa_key.pub"),
    type   => 'ssh-rsa',
    # This seems to balk at spaces in the hostname, hence trimming whitespace.
    name   => regsubst(file("${key_dir}/source_fqdn", "/etc/hostname"),'\s', ''),
    target => '/var/aegir/.ssh/known_hosts',
    # We don't want to actually add the key using the default though, so we
    # depend on an exec that should only run when the file is in place.
    require => Exec['check host key'],
  }
  file {'/var/aegir/.ssh/known_hosts':
    # Despite the 'target' attribute, sshkey runs as root user.
    owner   => 'aegir',
    group   => 'aegir',
    require => Sshkey['add-source-to-known_hosts'],
  }
  exec {'check host key':
    command => "ls ${key_dir}/ssh_host_rsa_key.pub",
    onlyif  => "ls ${key_dir}/ssh_host_rsa_key.pub",
  }

/*
  exec {'add source to known_hosts':
    command => "cat ${key_dir}/ssh_host_rsa_key.pub >> /var/aegir/.ssh/known_hosts",
    unless  => "grep -f - /var/aegir/.ssh/known_hosts << ${key_dir}/ssh_host_rsa_key.pub",
    onlyif  => "ls ${key_dir}/ssh_host_rsa_key.pub",
  }
*/

}


class aegirvps::remote_import::source inherits aegirvps::remote_import::git_transport {

  # We add a second file here as a default, since file() will cause the whole
  # Puppet run to fail if the file it's trying to read from doesn't exist yet.
  $key = regsubst(file("${key_dir}/aegir_id_rsa.pub", "/etc/ssh/ssh_host_rsa_key.pub"),'([\S]+)\s([\S]+)\s([\S]+)','\2')

  # Add the public key from the client's repo to our authorized keys.
  ssh_authorized_key {'aegir-remote-import':
    ensure => present,
    # TODO: fix the regex above, so we don't have to clean up whitespace here.
    key    => regsubst($key,'\s', ''),
    name   => 'importer public key',
    user   => 'aegir',
    type   => 'ssh-rsa',
    require => $aegir_installed,
  }

  # Put server's pub key in the client's repo, so it can be added to the
  # importer's known hosts.
  file {'ssh_host_rsa_key.pub':
    path    => "${key_dir}/ssh_host_rsa_key.pub",
    content => $sshrsakey,
  }
  file {'source_fqdn':
    path    => "${key_dir}/source_fqdn",
    content => $fqdn,
  }
  if $dev_prod == 'prod' {
    exec {'git add ssh_host_rsa_key.pub':
      command     => "git add files/ssh_host_rsa_key.pub && git commit -m'Add the server\'s host public key, so that it can be added to the importer server\'s known hosts.' && git push",
      cwd         => $repo_dir,
      refreshonly => true,
      subscribe   => File['ssh_host_rsa_key.pub'],
    }
  }
  # Remote import uses the hostname of the source server to connect to mysql.
  # In dev env, we need to make sure that resolves locally.
  if $dev_prod == 'dev' {
    file {"${key_dir}/dev_hosts":
      content => "
### Remote-import source ###
${ipaddress_eth1}  ${fqdn}
",
    }
  }
}
