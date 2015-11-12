# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"

  require_relative "mount.rb"

  config.vm.network 'private_network', ip: "10.234.234.234"

  config.vm.synced_folder './nfs_test', '/tmp/nfs_test',
    type: 'nfs',
    create: true,
    nfs: 3

end
