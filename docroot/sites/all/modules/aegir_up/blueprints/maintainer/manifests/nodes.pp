node "maintainer" {

  $local_dev = true
  include aegir_up
  include drush-vagrant::users
  drush-vagrant::user {'aegir':
    home_dir => '/var/aegir',
  }

  aegir_up::ssh_key { "${username}'s keypair":
    ssh_username   => $username,
    ssh_host_alias => 'drupalorg',
    ssh_hostname   => 'git.drupal.org',
    known_hosts    => '|1|49g3Z62+xFx8i3+hLbjEs1mvEMU=|NxyiNc+UNCoqymJg78Tbdv/Ee7g= ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCgVyq0OHD5+hST8/yaZIM6FkD0akypeGVx2T4eabIah3PvCIPS9UBtxnNxlmUD3MK0juK9rz8PWnv6x7TIi51/ks1TRCr2Ca1mXvl5mQNRwl7Z+eGv5uJuceXxhT+dyei0pdVUCU0IOymYJm3GcTJJEUeYjt4ob5WvVvnhpvZNpHVx/qTPD438yJaJIVfvH+4/Sw6V/odpJ/GjwNrwJbdYEWpkruVDAYpA9sPIhXXus4QfbUA1omDaX6NkRSZEt11JX8CNXPlkY3VFflR3ZoMcUCBJ0gOV9reB7VmROdRhFOWaaug13GyuasqrVYXp0skT4XvTPTEROtN4qeUkyESH
    |1|MnEDfDGGsJjs0ckGq5hZ7bv6xVM=|TMug/s6uXzeeayp81ruJQ4GNFGo= ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCgVyq0OHD5+hST8/yaZIM6FkD0akypeGVx2T4eabIah3PvCIPS9UBtxnNxlmUD3MK0juK9rz8PWnv6x7TIi51/ks1TRCr2Ca1mXvl5mQNRwl7Z+eGv5uJuceXxhT+dyei0pdVUCU0IOymYJm3GcTJJEUeYjt4ob5WvVvnhpvZNpHVx/qTPD438yJaJIVfvH+4/Sw6V/odpJ/GjwNrwJbdYEWpkruVDAYpA9sPIhXXus4QfbUA1omDaX6NkRSZEt11JX8CNXPlkY3VFflR3ZoMcUCBJ0gOV9reB7VmROdRhFOWaaug13GyuasqrVYXp0skT4XvTPTEROtN4qeUkyESH',
  }

  aegir::platform { 'MyModules':
    makefile       => "/vagrant/makefiles/${username}.make",
    working_copy   => true,
    build_timeout  => 0,
    #force_complete => true,
  }

}
