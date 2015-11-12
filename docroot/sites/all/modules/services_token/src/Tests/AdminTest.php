<?php
/**
 * @file
 * Contains Drupal\services_token\Tests\AdminTest.
 */

namespace Drupal\services_token\Tests;

/**
 * Tests administrative interface.
 */
class AdminTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  protected $adminUser;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Admin',
      'description' => 'Tests administrative interface',
      'group' => 'Services Token',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('services_token', 'rest_server'));
    $this->adminUser = $this->drupalCreateUser(array(
      'administer services',
      'administer site configuration',
    ));
    $this->drupalLogin($this->adminUser);
  }

  /**
   * Tests administrative interface.
   */
  public function testAdminInterface() {
    $name = strtolower($this->randomName());
    $path = strtolower($this->randomName());
    $realm = $this->randomString();

    // Create new service with token authentication enabled.
    $edit = array(
      'name' => $name,
      'server' => 'rest_server',
      'path' => $path,
      'authentication[services_token]' => TRUE,
    );
    $this->drupalPost('admin/structure/services/add', $edit, 'Save');
    $this->assertResponse(200);

    // Set the basic auth realm.
    $edit = array(
      'services_token[realm]' => $realm,
    );
    $this->drupalPost('admin/structure/services/list/' . $name . '/authentication', $edit, 'Save');
    $this->assertResponse(200);

    // Enable generate action in token resource with basic auth fallback.
    $edit = array(
      'resources[services_token][actions][generate][enabled]' => TRUE,
      'resources[services_token][actions][generate][settings][services_token][password_fallback]' => TRUE,
    );
    $this->drupalPost('admin/structure/services/list/' . $name . '/resources', $edit, 'Save');
    $this->assertResponse(200);

    // Verify the endpoint settings
    cache_clear_all('services:' . $name . ':', 'cache', TRUE);
    ctools_export_load_object_reset();
    $endpoint = services_endpoint_load($name);
    $this->assertIdentical($endpoint->authentication['services_token']['realm'], $realm, 'Realm is present in endpoint authentication settings');
    $this->assertTrue($endpoint->resources['services_token']['actions']['generate']['enabled'], 'Generate action of token resource is enabled');
    $this->assertTrue($endpoint->resources['services_token']['actions']['generate']['settings']['services_token']['password_fallback'], 'Generate action of token resource has password fallback');

    // Disable password fallback on token resource.
    $edit = array(
      'resources[services_token][actions][generate][settings][services_token][password_fallback]' => FALSE,
    );
    $this->drupalPost('admin/structure/services/list/' . $name . '/resources', $edit, 'Save');

    // Verify the endpoint settings.
    cache_clear_all('services:' . $name . ':', 'cache', TRUE);
    ctools_export_load_object_reset();
    $endpoint = services_endpoint_load($name);
    $this->assertFalse($endpoint->resources['services_token']['actions']['generate']['settings']['services_token']['password_fallback'], 'Generate action of token resource has password fallback');
  }

}
