# Role Provisioner [![Build Status](https://travis-ci.org/mglaman/role_provisioner.svg)](https://travis-ci.org/mglaman/role_provisioner)

Provides Drupal 7 with an OOP based based of controlling roles and permissions via YAML files

## Usage

This module provides a base for handling roles and permissions. The *RoleProvisioner* is expected to be extended through a another module. Your module will provide the configuration YAMLs and a class to ensure they're brought into scope.

### Extending the provisioner

@todo! For now look at test example \Drupal\role_provisioner\Tests\RoleProvisionerTest.

### Defining roles and permissions as YAML files

@todo!

*RoleProvisioner* provides the static method *exportYML* which accepts a role ID. It'll provide you the proper output for creating YAML files.

### Running the provisioner

The best practice would be to run this provisioner within the following hooks

* *hook_install*
* *hook_enable*
* *hook_update_N*

Example:

````
function role_provisioner_install() {
  $handler = new \Drupal\role_provisioner\RoleProvisioner();
  $handler->ensurePermissions();
}
````
