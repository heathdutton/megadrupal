<?php

/**
 * @file
 * Contains \Drupal\protected_download\Tests\TokenTest.
 */

namespace Drupal\protected_download\Tests;

/**
 * Tests token integration.
 */
class TokenTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  /**
   * The URI for a generated private file.
   *
   * @var string
   */
  protected $privateFileUri;

  /**
   * A file in the private directory.
   *
   * @var object
   */
  protected $privateFile;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Token integration',
      'description' => 'Tests token integration.',
      'group' => 'Protected Download',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('protected_download'));

    // Prepare a private file.
    $file = current($this->drupalGetTestFiles('text'));
    $private_file_dir = 'private://' . $this->randomName(8) . '/';
    file_prepare_directory($private_file_dir, FILE_CREATE_DIRECTORY);
    $this->privateFileUri = $private_file_dir . drupal_basename($file->uri);
    $this->privateFile = file_copy($file, $this->privateFileUri);
  }

  /**
   * Tests token integration.
   */
  public function testTokenIntegration() {
    $text = 'Your file is accessible until [file:protected-download-expire] using the following link [file:protected-download-url]';
    $pattern = '#^Your file is accessible until (.*) using the following link (.*)$#';
    $expected_date = REQUEST_TIME + PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL;
    $expected_base = url(implode('/', array(PROTECTED_DOWNLOAD_DEFAULT_MENU_ITEM_PATH, 'private')), array('absolute' => TRUE));

    $result = token_replace($text, array('file' => $this->privateFile));
    $status = preg_match($pattern, $result, $matches);
    $this->assertTrue($status);
    $this->assertIdentical(format_date($expected_date, 'medium'), $matches[1]);
    $this->assertIdentical(strpos($matches[2], $expected_base), 0);

    $text = 'Your file is accessible until [file:protected-download-expire:long] using the following link [file:protected-download-url]';
    $result = token_replace($text, array('file' => $this->privateFile));
    $status = preg_match($pattern, $result, $matches);
    $this->assertTrue($status);
    $this->assertIdentical(format_date($expected_date, 'long'), $matches[1]);
    $this->assertIdentical(strpos($matches[2], $expected_base), 0);
  }

}
