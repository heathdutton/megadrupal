<?php

namespace Drupal\role_provisioner\Tests;

use Drupal\role_provisioner\RoleProvisioner;

class RoleProvisionerTest extends RoleProvisioner {
  public function __construct() {
    parent::__construct();
    $this->loadConfig('editor');
  }
  /**
   * @inheritdoc
   */
  public function getPath() {
    return drupal_get_path('module', 'role_provisioner') . '/tests/config';
  }
}