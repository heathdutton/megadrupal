<?php
/**
 * @file
 * Contains Drupal\image_replace\Tests\ReplaceEffectTest.
 */

namespace Drupal\image_replace\Tests;

/**
 * Tests functionality of the replace image effect.
 */
class ReplaceEffectTest extends ImageReplaceTestBase {

  protected $profile = 'testing';

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Replace effect',
      'description' => 'Tests functionality of the replace image effect',
      'group' => 'Image Replace',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('image', 'image_replace'));
  }

  /**
   * Tests functionality of the replace image effect.
   *
   * Functionality covered by this test include:
   * - image_replace_add()
   * - image_replace_get()
   * - image_replace_remove()
   * - image_replace_effect()
   */
  public function testReplaceEffect() {
    list($original_file, $replacement_file) = $this->createTestFiles();

    // Create an image style containing the replace effect.
    $style_name = 'image_replace_test';
    $style = $this->createImageStyle($style_name);

    // Apply the image style to a test image.
    $generated_url = image_style_url($style_name, $original_file->uri);
    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the original image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsOriginal($generated_uri), 'The generated file should be the same as the original file if there is no replacement mapping.');

    // Set up a replacement image.
    image_replace_add($style_name, $original_file->uri, $replacement_file->uri);
    image_style_flush($style);

    // Apply the image style to the test imge.
    $generated_url = image_style_url($style_name, $original_file->uri);
    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the replacement image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsReplacement($generated_uri), 'The generated file should be the same as the replacement file');

    // Set up a replacement image.
    image_replace_remove($style_name, $original_file->uri, $replacement_file->uri);
    image_style_flush($style);

    // Apply the image style to a test image.
    $generated_url = image_style_url($style_name, $original_file->uri);
    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the original image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsOriginal($generated_uri), 'The generated file should be the same as the original file if the replacement mapping was removed.');
  }

}
