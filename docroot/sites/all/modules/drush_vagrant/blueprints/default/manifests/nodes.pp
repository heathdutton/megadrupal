# A Puppet manifest to provision a default server

node default {
  include drush-vagrant::user
}
