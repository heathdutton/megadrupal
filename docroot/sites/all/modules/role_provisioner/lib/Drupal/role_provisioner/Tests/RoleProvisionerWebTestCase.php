<?php

/**
 * @file
 * Tests for Role Provisioner.
 */

namespace Drupal\role_provisioner\Tests;

/**
 * Test role provisioner
 */
class RoleProvisionerWebTestCase extends \DrupalWebTestCase {
  /**
   * @var RoleProvisionerTest
   */
  protected $provisioner;

  /**
   * Implements getInfo().
   */
  public static function getInfo() {
    return array(
      'name' => 'Role provisioner test',
      'description' => 'Tests the provisioner class',
      'group' => 'Role provisioner',
    );
  }

  /**
   * Implements setUp().
   */
  function setUp() {
    $this->provisioner = new RoleProvisionerTest();
    parent::setUp(array(
      'libraries',
      'xautoload',
    ));
  }

  /**
   * Test role provisioning.
   */
  function testRoleProvision() {
    $this->provisioner->ensureRoles();

    // Test that the role was created.
    $roles = user_roles();
    $this->assertTrue(in_array('editor', $roles));

    // Load the role itself
    $editor = user_role_load_by_name('editor');
    if (!$editor) {
      $this->fail('Failed to load role by name');
    }
    $this->assertTrue(
      ($editor->name == 'editor'),
      'Editor role name was not set properly.'
    );
  }

  /**
   * Test permission provisioning.
   */
  function testPermissionsProvision() {
    $this->provisioner->ensurePermissions();
    $roles = user_roles(false, 'administer nodes');

    // Test the provisioned role has administer nodes permission.
    $this->assertTrue(in_array('editor', $roles));
  }
}