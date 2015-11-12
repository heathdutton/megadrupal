<?php
/**
 * @file
 * Contains Drupal\image_replace\Tests\EntityTest.
 */

namespace Drupal\image_replace\Tests;

/**
 * Tests functionality of the replace image effect.
 */
class EntityTest extends ImageReplaceTestBase {

  protected $profile = 'standard';

  protected $styleName;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Entity',
      'description' => 'Tests core entity API integration for the replace image effect',
      'group' => 'Image Replace',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('image', 'image_replace'));

    // Create an image style containing the replace effect.
    $this->styleName = 'image_replace_test';
    $this->createImageStyle($this->styleName);

    // Add the replacement image field to the article bundle.
    $this->createImageField('image_replacement', 'article');

    // Add the original image field to the article bundle and specify
    // the replacement image as replacement.
    $instance = $this->createImageField('image_original', 'article', array(), array(
      'image_replace' => array(
        $this->styleName => array(
          'source_field' => 'image_replacement',
        ),
      ),
    ));

    $instance['display']['teaser']['type'] = 'image';
    $instance['display']['teaser']['settings']['image_style'] = $this->styleName;
    $instance['display']['full']['type'] = 'image';
    $instance['display']['full']['settings']['image_style'] = NULL;
    field_update_instance($instance);

    field_bundle_settings('node', 'article', array(
      'view_modes' => array(
        'full' => array(
          'custom_settings' => TRUE,
        ),
        'teaser' => array(
          'custom_settings' => TRUE,
        ),
      ),
    ));
  }

  /**
   * Tests image replacement on node entities.
   */
  public function testNodeView() {
    list($original_file, $replacement_file) = $this->createTestFiles();

    $node = (object) array(
      'type' => 'article',
    );
    node_object_prepare($node);

    $node->title = $this->randomName(16);
    $node->promote = 1;
    $node->language = LANGUAGE_NONE;
    $node->image_original[LANGUAGE_NONE][0] = (array) $original_file;
    $node->image_replacement[LANGUAGE_NONE][0] = (array) $replacement_file;
    node_save($node);

    // Check teaser.
    $this->drupalGet('node');
    $generated_url = image_style_url($this->styleName, $node->image_original[LANGUAGE_NONE][0]['uri']);
    $this->assertRaw(check_plain($generated_url), format_string('Image displayed using style @style.', array('@style' => $this->styleName)));

    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the replacement image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsReplacement($generated_uri), 'The generated file should be the same as the replacement file on teaser.');

    // Check full view.
    $this->drupalGet('node/' . $node->nid);
    $generated_url = file_create_url($node->image_original[LANGUAGE_NONE][0]['uri']);
    $this->assertRaw(check_plain($generated_url), 'Original image displayed');

    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the original image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsOriginal($generated_uri), 'The generated file should be the same as the original file on full node view.');
  }

}
