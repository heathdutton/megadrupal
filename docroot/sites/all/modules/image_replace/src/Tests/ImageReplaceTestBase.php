<?php
/**
 * @file
 * Contains Drupal\image_replace\Tests\ImageReplaceTestBase.
 */

namespace Drupal\image_replace\Tests;

/**
 * Tests functionality of the replace image effect.
 */
class ImageReplaceTestBase extends \DrupalWebTestCase {

  /**
   * Create a new image field.
   *
   * @param string $name
   *   The name of the new field (all lowercase), exclude the "field_" prefix.
   * @param string $type_name
   *   The node type that this field will be added to.
   * @param array $field_settings
   *   A list of field settings that will be added to the defaults.
   * @param array $instance_settings
   *   A list of instance settings that will be added to the instance defaults.
   * @param array $widget_settings
   *   A list of widget settings that will be added to the widget defaults.
   */
  protected function createImageField($name, $type_name, $field_settings = array(), $instance_settings = array(), $widget_settings = array()) {
    $field = array(
      'field_name' => $name,
      'type' => 'image',
      'settings' => array(),
      'cardinality' => !empty($field_settings['cardinality']) ? $field_settings['cardinality'] : 1,
    );
    $field['settings'] = array_merge($field['settings'], $field_settings);
    field_create_field($field);

    $instance = array(
      'field_name' => $field['field_name'],
      'entity_type' => 'node',
      'label' => $name,
      'bundle' => $type_name,
      'required' => !empty($instance_settings['required']),
      'settings' => array(
        'default_image' => NULL,
      ),
      'widget' => array(
        'type' => 'image_image',
        'settings' => array(),
      ),
    );
    $instance['settings'] = array_merge($instance['settings'], $instance_settings);
    $instance['widget']['settings'] = array_merge($instance['widget']['settings'], $widget_settings);
    return field_create_instance($instance);
  }

  /**
   * Create an image style containing the image repace effect.
   *
   * @param string $name
   *   The name of the new image style (all lowercase).
   *
   * @return array
   *   The newly created image style array.
   */
  protected function createImageStyle($name) {
    // Create an image style containing the replace effect.
    $style = image_style_save(array('name' => $name, 'label' => $this->randomString()));
    $effect = array(
      'name' => 'image_replace',
      'data' => array(),
      'isid' => $style['isid'],
    );
    image_effect_save($effect);
    return $style;
  }

  /**
   * Create a pair of test files.
   *
   * @return array
   *   An array with two file objects (original_file, replacement_file).
   */
  protected function createTestFiles() {
    // Generate test images.
    $original_uri = file_unmanaged_copy(__DIR__ . '/fixtures/original.png', 'public://', FILE_EXISTS_RENAME);
    $this->assertTrue($this->imageIsOriginal($original_uri));
    $this->assertFalse($this->imageIsReplacement($original_uri));
    $original_file = file_save((object) array(
      'filename' => drupal_basename($original_uri),
      'uri' => $original_uri,
      'status' => FILE_STATUS_PERMANENT,
      'filemime' => file_get_mimetype($original_uri),
    ));

    $replacement_uri = file_unmanaged_copy(__DIR__ . '/fixtures/replacement.png', 'public://', FILE_EXISTS_RENAME);
    $this->assertTrue($this->imageIsReplacement($replacement_uri));
    $this->assertFalse($this->imageIsOriginal($replacement_uri));
    $replacement_file = file_save((object) array(
      'filename' => drupal_basename($replacement_uri),
      'uri' => $replacement_uri,
      'status' => FILE_STATUS_PERMANENT,
      'filemime' => file_get_mimetype($replacement_uri),
    ));

    return array($original_file, $replacement_file);
  }

  /**
   * Returns TRUE if the image pointed at by the URI is the original image.
   */
  protected function imageIsOriginal($image_uri) {
    $expected_info = array(
      'extension' => 'png',
      'height' => 90,
      'mime_type' => 'image/png',
      'width' => 120,
    );

    $image_info = image_get_info($image_uri);
    unset($image_info['file_size']);
    ksort($image_info);

    // FIXME: Assert that original image has a red pixel on x=40, y=30.
    return $expected_info === $image_info;
  }

  /**
   * Returns TRUE if the image pointed at by the URI is the replacement image.
   */
  protected function imageIsReplacement($image_uri) {
    $expected_info = array(
      'extension' => 'png',
      'height' => 60,
      'mime_type' => 'image/png',
      'width' => 80,
    );

    $image_info = image_get_info($image_uri);
    unset($image_info['file_size']);
    ksort($image_info);

    // FIXME: Assert that replacement image has a green pixel on x=40, y=30.
    return $expected_info === $image_info;
  }

}
