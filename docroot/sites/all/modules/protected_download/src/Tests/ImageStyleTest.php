<?php

/**
 * @file
 * Contains \Drupal\protected_download\Tests\ProtectedDownloadImageStyleTest.
 */

namespace Drupal\protected_download\Tests;

/**
 * Tests functions for generating paths and URLs to image styles.
 */
class ImageStyleTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  protected $styleName;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Image styles path and URL functions',
      'description' => 'Tests functions for generating paths and URLs to image styles.',
      'group' => 'Protected Download',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('protected_download', 'image'));

    $this->styleName = 'style_foo';
    image_style_save(array('name' => $this->styleName, 'label' => $this->randomString()));

    $protected_dir = $this->private_files_directory . '/' . $this->randomName(8);
    $result = file_prepare_directory($protected_dir, FILE_CREATE_DIRECTORY);
    $this->assertTrue($result, 'Created protected directory');
    variable_set('protected_download_file_path_protected', $protected_dir);
    drupal_static_reset('file_get_stream_wrappers');
  }

  /**
   * Tests failure mode when source image is missing.
   */
  public function testImageStyleUrlForMissingSourceImage() {
    $non_existent_uri = 'protected://foo.png';
    $generated_url = image_style_url($this->styleName, $non_existent_uri);
    $this->drupalGet($generated_url);
    $this->assertResponse(404, 'Accessing an image style URL with a source image that does not exist provides a 404 error response.');

    // Check that requesting a image style of a nonexistent image does not
    // create any new directories in the file system.
    $directory = 'protected://styles/' . $this->styleName . '/protected/' . $this->randomName();
    $this->drupalGet(file_create_url($directory . '/' . $this->randomName()));
    $this->assertFalse(file_exists($directory), 'New directory was not created in the filesystem when requesting an unauthorized image.');
  }

  /**
   * Tests functions for generating paths and URLs to image styles.
   */
  public function testImageStyleUrlAndPath() {
    $scheme = 'protected';

    // Create the directories for the styles.
    $directory = $scheme . '://styles/' . $this->styleName;
    $status = file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
    $this->assertNotIdentical(FALSE, $status, 'Created the directory for the generated images for the test style.');

    // Create a working copy of the file.
    $files = $this->drupalGetTestFiles('image');
    $file = array_shift($files);
    $image_info = image_get_info($file->uri);
    $original_uri = file_unmanaged_copy($file->uri, $scheme . '://', FILE_EXISTS_RENAME);
    $this->assertNotIdentical(FALSE, $original_uri, 'Created the generated image file.');

    // Get the URL of a file that has not been generated and try to create it.
    $generated_uri = image_style_path($this->styleName, $original_uri);
    $this->assertFalse(file_exists($generated_uri), 'Generated file does not exist.');
    $generate_url = image_style_url($this->styleName, $original_uri);

    // Fetch the URL that generates the file.
    $this->drupalGet($generate_url);
    $this->assertResponse(200, 'Image was generated at the URL.');
    $this->assertTrue(file_exists($generated_uri), 'Generated file does exist after we accessed it.');
    $this->assertRaw(file_get_contents($generated_uri), 'URL returns expected file.');
    $generated_image_info = image_get_info($generated_uri);
    $this->assertEqual($this->drupalGetHeader('Content-Type'), $generated_image_info['mime_type'], 'Expected Content-Type was reported.');
    $this->assertEqual($this->drupalGetHeader('Content-Length'), $generated_image_info['file_size'], 'Expected Content-Length was reported.');

    // Fetch the URL a second time and ensure that proper cache control headers
    // are present on subsequent request.
    $this->drupalGet($generate_url);
    $this->assertResponse(200, 'Access to generated file granted with HMAC protected URL.');
    $content_type = $this->drupalGetHeader('Content-Type');
    $this->assertIdentical(strpos($content_type, 'image/png'), 0, 'Content-Type header is text/plain');
    $content_length = $this->drupalGetHeader('Content-Length');
    $this->assertEqual($content_length, filesize($generated_uri), 'Content-Length header returns correct length');
    $last_modified = $this->drupalGetHeader('Last-Modified');
    $this->assertEqual($last_modified, gmdate(DATE_RFC7231, filemtime($generated_uri)), 'Last-Modified header returns correct date');
    $cache_control = $this->drupalGetHeader('Cache-Control');
    $this->assertIdentical(strpos($cache_control, 'public, max-age='), 0, 'Cache-Control header contains private and max-age directives');
    $etag = $this->drupalGetHeader('ETag');
    $this->assertTrue($etag, 'ETag header is present');
    $this->assertTrue($this->drupalGetHeader('Expires'), 'Expires header is present');
  }

}
