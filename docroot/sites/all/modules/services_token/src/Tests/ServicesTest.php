<?php
/**
 * @file
 * Contains Drupal\services_token\Tests\ServicesTest.
 */

namespace Drupal\services_token\Tests;

/**
 * Tests integration with services.
 */
class ServicesTest extends \ServicesWebTestCase {

  protected $profile = 'testing';

  protected $realm;

  protected $endpoint;

  protected $plainUser;

  protected $privilegedUser;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Services',
      'description' => 'Tests integration with services',
      'group' => 'Services Token',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('services_token'));

    $this->endpoint = $this->saveNewEndpoint();

    $this->realm = $this->randomString();

    // Add the token authentication and resource.
    $this->endpoint->resources['services_token'] = array(
      'actions' => array(
        'generate' => array(
          'enabled' => '1',
          'settings' => array(
            'services_token' => array(
              'password_fallback' => '1',
            ),
          ),
        ),
      ),
    );
    $this->endpoint->authentication['services_token'] = array(
      'realm' => $this->realm,
    );
    services_endpoint_save($this->endpoint);

    $this->plainUser = $this->drupalCreateUser();
    $this->privilegedUser = $this->drupalCreateUser(array('generate services token'));
  }

  /**
   * Tests generating tokens.
   */
  public function testTokenResource() {
    $this->drupalLogin($this->plainUser);
    $this->servicesPost($this->endpoint->path . '/services_token/generate', array());
    $this->assertResponse(403);

    $this->drupalLogin($this->privilegedUser);
    $response = $this->servicesPost($this->endpoint->path . '/services_token/generate', array());
    $this->assertResponse(200);

    $this->assertTrue(strtotime($response['body']->expires) > REQUEST_TIME, 'Generated token has an expiry time in the future');
    $valid = services_token_validate($response['body']->token, $this->realm);
    $this->assertIdentical($valid, TRUE, 'Generated token is valid');
  }

  /**
   * Test token generation password fallback.
   */
  public function testPasswordFallback() {
    $headers = array(
      'Authorization: Basic ' . base64_encode($this->privilegedUser->name . ':' . $this->privilegedUser->pass_raw),
    );

    // Use username/password to authenticate and verify that accessing the token
    // generator succeeds.
    $response = $this->servicesPost($this->endpoint->path . '/services_token/generate', array(), $headers);
    $this->assertResponse(200);

    $this->assertTrue(strtotime($response['body']->expires) > REQUEST_TIME, 'Generated token has an expiry time in the future');
    $valid = services_token_validate($response['body']->token, $this->realm);
    $this->assertIdentical($valid, TRUE, 'Generated token is valid');

    // Use username/password to authenticate and verify that access to other
    // resources is denied.
    $response = $this->servicesGet($this->endpoint->path . '/user/' . $this->privilegedUser->uid, NULL, $headers);
    $this->assertResponse(401);
    $this->assertEqual($response['body'], 'Invalid credentials.');

    // Use username/password to authenticate and verify that accessing the token
    // generator fails if password fallback is disabled..
    $this->endpoint->resources['services_token']['actions']['generate']['settings']['services_token']['password_fallback'] = '0';
    services_endpoint_save($this->endpoint);

    $response = $this->servicesPost($this->endpoint->path . '/services_token/generate', array(), $headers);
    $this->assertResponse(401);
    $this->assertEqual($response['body'], 'Invalid credentials.');
  }

  /**
   * Test token authentication.
   */
  public function testTokenAuthentication() {
    $uid = $this->plainUser->uid;

    // Try to get a protected resource without any authentication.
    $this->servicesGet($this->endpoint->path . '/user/' . $uid);
    $this->assertResponse(401);
    $challenge = $this->drupalGetHeader('WWW-Authenticate');
    $this->assertIdentical(strtolower(substr($challenge, 0, 12)), 'basic realm=', 'Basic auth challenge header present');
    $this->assertIdentical(substr($challenge, 12), '"' . check_plain($this->realm) . '"', 'Basic auth realm present');

    // Try to get a protected resource with token auth.
    $token = services_token($this->realm, $uid, REQUEST_TIME + 3600);
    $headers = array(
      'Authorization: Basic ' . base64_encode($token . ':'),
    );
    $response = $this->servicesGet($this->endpoint->path . '/user/' . $uid, NULL, $headers);
    $this->assertResponse(200);
    $this->assertEqual($response['body']->uid, $uid, 'Fetched correct user object');
  }

}
