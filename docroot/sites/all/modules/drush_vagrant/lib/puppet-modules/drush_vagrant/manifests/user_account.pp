define drush_vagrant::user_account (
  $home_dir,
  $git_name = undef,
  $git_email = undef
  ) {

  User { ensure => present,
         groups => 'sudo',
         shell  => '/bin/bash',
  }

  if !defined(User[$name]) {
    user {$name:
      home   => $home_dir,
    }
  }

  # Various dotfiles
  File { ensure  => present,
         owner   => $name,
         group   => $name,
         replace => false,
  }
  if !defined(File[$home_dir]) {
    file { $home_dir:
             ensure => directory,
             before => [
               File["${home_dir}/.profile"],
               File["${home_dir}/.bashrc"],
               File["${home_dir}/.bash_aliases"],
               File["${home_dir}/.vimrc"],
               File["${home_dir}/.ssh"]
             ];
    }
  }
  if !defined(File["${home_dir}/.ssh"]) {
    file { "${home_dir}/.ssh":
      ensure => directory;
    }
  }
  file { "${home_dir}/.profile":
           source => ["/vagrant/.config/files/.profile",
                      "puppet:///modules/drush_vagrant/profile.example"];
         "${home_dir}/.bashrc":
           source => ["/vagrant/.config/files/.bashrc",
                      "puppet:///modules/drush_vagrant/bashrc.example"];
         "${home_dir}/.bash_aliases":
           source => ["/vagrant/.config/files/.bash_aliases",
                      "puppet:///modules/drush_vagrant/bash_aliases.example"];
         "${home_dir}/.vimrc":
           source => ["/vagrant/.config/files/.vimrc",
                      "puppet:///modules/drush_vagrant/vimrc.example"];
  }

  if $name != 'vagrant' {
    file { "${home_dir}/.ssh/authorized_keys":
             source  => ["/vagrant/.config/files/authorized_keys",
                         "puppet:///modules/drush_vagrant/authorized_keys.example"],
             require => File["${home_dir}/.ssh"];
    }
  }

  #git username & email
  if !defined(Package['git']) {
    package { 'git': }
  }

  Exec { user        => $name,
         group       => $name,
         environment => "HOME=${home_dir}",
         path        => '/usr/bin',
         require     => Package['git'],
  }

  if $git_name { $real_git_name = $git_name }
  elsif $::git_name { $real_git_name = $::git_name }
  else { $real_git_name = false }
  if $real_git_name {
    exec { "git user.name config for ${name}" :
      command => "git config --global user.name \"${real_git_name}\"",
    }
  }

  if $git_email { $real_git_email = $git_email }
  elsif $::git_email { $real_git_email = $::git_email }
  else { $real_git_email = false }
  if $real_git_email {
    exec { "git user.email config for ${name}" :
      command => "git config --global user.email ${real_git_email}",
    }
  }

}
