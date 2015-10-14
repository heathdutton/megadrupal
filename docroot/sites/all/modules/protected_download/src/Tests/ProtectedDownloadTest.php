<?php

/**
 * @file
 * Contains \Drupal\protected_download\Tests\ProtectedDownloadTest.
 */

namespace Drupal\protected_download\Tests;

/**
 * Test protected download.
 */
class ProtectedDownloadTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  /**
   * The URI for a generated private file.
   *
   * @var string
   */
  protected $privateFileUri;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Protected download',
      'description' => 'Test protected download.',
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
    file_unmanaged_copy($file->uri, $this->privateFileUri);
  }

  /**
   * Tests file downloads and cache headers.
   */
  public function testProtectedDownload() {
    // Attempt to download the file directly.
    $url = file_create_url($this->privateFileUri);
    $this->drupalGet($url);
    $this->assertResponse(403, 'Access to private file is denied.');

    // Generate a HMAC protected URL and use that.
    $url = protected_download_create_url($this->privateFileUri);
    $this->assertTrue($url);
    $this->drupalGet($url);

    // Check response headers.
    $this->assertResponse(200, 'Access to private file is granted with HMAC protected URL.');
    $content_type = $this->drupalGetHeader('Content-Type');
    $this->assertIdentical(strpos($content_type, 'text/plain'), 0, 'Content-Type header is text/plain');
    $content_length = $this->drupalGetHeader('Content-Length');
    $this->assertEqual($content_length, filesize($this->privateFileUri), 'Content-Length header returns correct length');
    $last_modified = $this->drupalGetHeader('Last-Modified');
    $this->assertEqual($last_modified, gmdate(DATE_RFC7231, filemtime($this->privateFileUri)), 'Last-Modified header returns correct date');
    $cache_control = $this->drupalGetHeader('Cache-Control');
    $this->assertIdentical(strpos($cache_control, 'private, max-age='), 0, 'Cache-Control header contains private and max-age directives');
    $etag = $this->drupalGetHeader('ETag');
    $this->assertTrue($etag, 'ETag header is present');
    $this->assertTrue($this->drupalGetHeader('Expires'), 'Expires header is present');

    // Try a conditional request.
    $headers = array(
      'If-Modified-Since: ' . $last_modified,
      'If-None-Match: ' . $etag,
    );
    $this->drupalGet($url, array(), $headers);

    // Check response headers for 304 response.
    $this->assertResponse(304, 'Conditional request returned 304 Not Modified.');
    $this->assertFalse($this->drupalGetHeader('Content-Type'), 'Content-Type header absent on 304 response');
    $this->assertFalse($this->drupalGetHeader('Content-Length'), 'Content-Length header absent on 304 response');
    $this->assertFalse($this->drupalGetHeader('Last-Modified'), 'Last-Modified header absent on 304 response');
    $cache_control = $this->drupalGetHeader('Cache-Control');
    $this->assertIdentical(strpos($cache_control, 'private, max-age='), 0, 'Cache-Control header contains private and max-age directives on 304 response');
    $this->assertTrue($this->drupalGetHeader('ETag'), 'ETag header is present on 304 response');
    $this->assertTrue($this->drupalGetHeader('Expires'), 'Expires header is present on 304 response');
  }

  /**
   * Tests customized cache headers.
   */
  public function testCustomCacheControl() {
    // Set custom Cache-Control directives.
    variable_set('protected_download_cache_control_private', 'public, no-transform');

    $url = protected_download_create_url($this->privateFileUri);
    $this->assertTrue($url);
    $this->drupalGet($url);
    $this->assertResponse(200, 'Access to private file is granted with HMAC protected URL.');
    $cache_control = $this->drupalGetHeader('Cache-Control');
    $this->assertIdentical(strpos($cache_control, 'public, no-transform, max-age='), 0, 'Cache-Control header contains custom directives and max-age');

    // Suppress the addition of any Cache-Control directives except for max-age.
    variable_set('protected_download_cache_control_private', FALSE);

    $url = protected_download_create_url($this->privateFileUri);
    $this->assertTrue($url);
    $this->drupalGet($url);
    $this->assertResponse(200, 'Access to private file is granted with HMAC protected URL.');
    $cache_control = $this->drupalGetHeader('Cache-Control');
    $this->assertIdentical(strpos($cache_control, 'max-age='), 0, 'Cache-Control header only contains max-age directive');

    // Reset configuration.
    variable_del('protected_download_cache_control_private');
  }

  /**
   * Tests customized menu path.
   */
  public function testCustomMenuItemPath() {
    $path = $this->randomName(8) . '/' . $this->randomName(4);
    variable_set('protected_download_menu_item_path', $path);
    menu_rebuild();

    $url = protected_download_create_url($this->privateFileUri);
    $this->assertTrue($url);

    $base = url($path, array('absolute' => TRUE));

    $this->assertIdentical(substr($url, 0, strlen($base)), $base, 'URL starts with custom path');
    $this->drupalGet($url);
    $this->assertResponse(200, 'Access to private file is granted with HMAC protected URL.');
    $this->assertIdentical(substr($this->getUrl(), 0, strlen($base)), $base, 'URL starts with custom path');

    variable_del('protected_download_menu_item_path');
  }

  /**
   * Tests failure mode when attempting to generate invalid URLs.
   */
  public function testCreateInvalidUrls() {
    $url = protected_download_create_url('/uri/without/a/scheme');
    $this->assertIdentical($url, FALSE);
  }

  /**
   * Tests failure mode when attempting to retrieve files with manipulated URLs.
   */
  public function testProtectedDownloadInvalidUrls() {
    $url = protected_download_create_url($this->privateFileUri);
    $this->assertTrue($url);
    $this->drupalGet($this->replaceUrlComponents($url));
    $this->assertResponse(200, 'Access to private file is granted with HMAC protected URL.');

    // Test incomplete URL.
    $this->drupalGet($this->replaceUrlComponents($url, ''));
    $this->assertResponse(404, 'Not found if scheme is missing');
    $this->drupalGet($this->replaceUrlComponents($url, NULL, ''));
    $this->assertResponse(404, 'Not found if expire is missing');
    $this->drupalGet($this->replaceUrlComponents($url, NULL, NULL, ''));
    $this->assertResponse(404, 'Not found if hmac is missing');
    $this->drupalGet($this->replaceUrlComponents($url, NULL, NULL, NULL, ''));
    $this->assertResponse(404, 'Not found if target is missing');

    // Prepare another private file.
    $files = $this->drupalGetTestFiles('text');
    $file = end($files);
    $private_file_dir = 'private://' . $this->randomName(8) . '/';
    file_prepare_directory($private_file_dir, FILE_CREATE_DIRECTORY);
    $other_file_uri = $private_file_dir . drupal_basename($file->uri);
    file_unmanaged_copy($file->uri, $other_file_uri);

    $other_url = protected_download_create_url($other_file_uri);
    $this->assertTrue($other_url);
    $this->drupalGet($this->replaceUrlComponents($other_url));
    $this->assertResponse(200, 'Access to private file is granted with HMAC protected URL.');

    // Ensure that base url (including HMAC) of the first file cannot be used
    // for the second one.
    $other_basename = drupal_basename($other_file_uri);
    $this->drupalGet($this->replaceUrlComponents($url, NULL, NULL, NULL, $other_basename));
    $this->assertResponse(404, 'Not found if basename is replaced with another one');

    // Ensure that downloads fail for expired links.
    $url = protected_download_create_url($this->privateFileUri, REQUEST_TIME - 1);
    $this->assertTrue($url);
    $this->drupalGet($url);
    $this->assertResponse(404, 'Not found if expired');

    // Ensure that downloads fail for invalid schemes.
    $invalid_stream_uri = 'bogus://' . $this->randomName(8) . '/' . drupal_basename($file->uri);
    $url = protected_download_create_url($invalid_stream_uri);
    $this->assertTrue($url);
    $this->drupalGet($url);
    $this->assertResponse(404, 'Not found if expired');
  }

  /**
   * Helper function capable of replacing certain parts of the URL.
   */
  protected function replaceUrlComponents($url, $replacement_scheme = NULL, $replacement_expire = NULL, $replacement_hmac = NULL, $replacement_target = NULL) {
    $menu_item_path = variable_get('protected_download_menu_item_path', PROTECTED_DOWNLOAD_DEFAULT_MENU_ITEM_PATH);
    $base_url = url($menu_item_path, array('absolute' => TRUE));

    $this->assertIdentical(strpos($url, $base_url), 0, 'Correct base URL');

    $path_components = explode('/', substr($url, strlen($base_url) + 1));
    $scheme = array_shift($path_components);
    $expire = array_shift($path_components);
    $hmac = array_shift($path_components);
    $target = implode('/', $path_components);

    if (isset($replacement_scheme)) {
      $scheme = $replacement_scheme;
    }
    if (isset($replacement_expire)) {
      $expire = $replacement_expire;
    }
    if (isset($replacement_hmac)) {
      $hmac = $replacement_hmac;
    }
    if (isset($replacement_target)) {
      $target = $replacement_target;
    }

    return implode('/', array($base_url, $scheme, $expire, $hmac, $target));
  }

}
