<?php
/**
 * @file
 * Contains Drupal\services_token\Tests\TokenTest.
 */

namespace Drupal\services_token\Tests;

/**
 * Tests generation and validation of tokens.
 */
class TokenTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Token',
      'description' => 'Tests generation and validation of tokens',
      'group' => 'Services Token',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('services_token'));
  }

  /**
   * Tests generating tokens.
   */
  public function testTokenGeneration() {
    $realm = $this->randomString(8);
    $expires = mt_rand(0, 0xFFFFFFFE);
    $account = $this->drupalCreateUser();

    $token1 = services_token($realm, $account->uid, $expires);
    list($hexuid, $hexexpires, $hmac) = explode('.', $token1);
    $this->assertEqual(hexdec($hexuid), $account->uid, 'User id is part of the token');
    $this->assertEqual(hexdec($hexexpires), $expires, 'Expiry timestamp is part of the token');
    $this->assertIdentical(strlen($hmac), 43, 'Generated hmac is exactly 43 characters long');

    $token2 = services_token($realm, $account->uid, $expires);
    $this->assertIdentical($token1, $token2, 'Token generation is idempotent');

    $realm = $this->randomString(8);
    $token3 = services_token($realm, $account->uid, $expires);
    $this->assertNotEqual($token1, $token3, 'Generated token varies for different realms');

    $account = $this->drupalCreateUser();
    $token4 = services_token($realm, $account->uid, $expires);
    $this->assertNotEqual($token3, $token4, 'Generated token varies for different users');

    $expires++;
    $token5 = services_token($realm, $account->uid, $expires);
    $this->assertNotEqual($token4, $token5, 'Generated token varies for different expiry times');
  }

  /**
   * Tests token validation.
   */
  public function testTokenValidation() {
    $account = $this->drupalCreateUser();
    $realm = $this->randomString(8);
    $expires = mt_rand(REQUEST_TIME + 1, 0xFFFFFFFE);
    $token = services_token($realm, $account->uid, $expires);

    $result = services_token_validate($token, $realm);
    $this->assertIdentical($result, TRUE, 'Validation succeeds if parameters are the same');

    $parts = explode('.', $token);
    $parts[2] = drupal_random_key(16);
    $result = services_token_validate(implode('.', $parts), $realm);
    $this->assertIdentical($result, FALSE, 'Validation fails if token is garbage');

    $parts = explode('.', $token);
    $parts[0] = dechex($this->drupalCreateUser()->uid);
    $result = services_token_validate(implode('.', $parts), $realm);
    $this->assertIdentical($result, FALSE, 'Validation fails if account is different');

    $parts = explode('.', $token);
    $parts[1] = dechex($expires + 1);
    $result = services_token_validate(implode('.', $parts), $realm);
    $this->assertIdentical($result, FALSE, 'Validation fails if expiry date is different');

    $other_realm = $this->randomString(7);
    $result = services_token_validate($token, $other_realm);
    $this->assertIdentical($result, FALSE, 'Validation fails if realm is different');
  }

  /**
   * Tests token expiry.
   */
  public function testTokenExpiry() {
    $account = $this->drupalCreateUser();
    $realm = $this->randomString(8);

    $expires = REQUEST_TIME + 1;
    $token = services_token($realm, $account->uid, $expires);
    $result = services_token_validate($token, $realm);
    $this->assertIdentical($result, TRUE, 'Token is valid if expiry time is in the future');

    $expires = REQUEST_TIME + 1;
    $token = services_token($realm, $account->uid, (string) $expires);
    $result = services_token_validate($token, $realm);
    $this->assertIdentical($result, TRUE, 'Token validation succeeds if the expiry time is a string');

    $expires = REQUEST_TIME + 1;
    $token = services_token($realm, $account->uid, (int) $expires);
    $result = services_token_validate($token, $realm);
    $this->assertIdentical($result, TRUE, 'Token validation succeeds if the expiry time is an int');

    $expires = REQUEST_TIME;
    $token = services_token($realm, $account->uid, $expires);
    $result = services_token_validate($token, $realm);
    $this->assertIdentical($result, FALSE, 'Token is invalid if expiry time is reached');

    $expires = REQUEST_TIME - 1;
    $token = services_token($realm, $account->uid, $expires);
    $result = services_token_validate($token, $realm);
    $this->assertIdentical($result, FALSE, 'Token is invalid if expiry time passed');
  }

}
