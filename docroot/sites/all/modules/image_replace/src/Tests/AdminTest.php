<?php
/**
 * @file
 * Contains Drupal\image_replace\Tests\AdminTest.
 */

namespace Drupal\image_replace\Tests;

/**
 * Tests administrative interface for image replace.
 */
class AdminTest extends ImageReplaceTestBase {

  protected $profile = 'standard';

  protected $styleName;

  protected $adminUser;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Admin interface',
      'description' => 'Tests the administrative interface of the image replace module',
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
    $instance = $this->createImageField('image_original', 'article');
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

    $this->adminUser = $this->drupalCreateUser(array(
      'access content',
      'access administration pages',
      'administer site configuration',
      'administer content types',
      'administer nodes',
      'create article content',
      'edit any article content',
      'delete any article content',
      'administer image styles',
    ));
  }

  /**
   * Tests image replacement on node entities.
   */
  public function testFieldEditUi() {
    list($original_file, $replacement_file) = $this->createTestFiles();

    // Create an unrelated image style.
    $unrelated_style_name = 'other_style';
    $style = image_style_save(array(
      'name' => $unrelated_style_name,
      'label' => $this->randomString(),
    ));

    $this->drupalLogin($this->adminUser);

    $this->drupalGet('admin/structure/types/manage/article/fields/image_original');

    // Verify that a select field is present with a list of available source
    // fields for the generated image style.
    $field_name = 'instance[settings][image_replace][' . $this->styleName . '][source_field]';
    $this->assertFieldByName($field_name, "0", 'Image replace selector found for style containing the image replace effect');
    $result = $this->xpath($this->constructFieldXpath('name', $field_name));

    $options = $this->getAllOptions($result[0]);
    $contains_image_original = FALSE;
    $contains_image_replacement = FALSE;
    foreach ($options as $option) {
      $contains_image_original |= $option['value'] == 'image_original';
      $contains_image_replacement |= $option['value'] == 'image_replacement';
    }
    $this->assertFalse($contains_image_original, 'Original image field is not in the list of options');
    $this->assertTrue($contains_image_replacement, 'Replacement image field is in the list of options');

    // Verify that no select field is present for an image style which does not
    // contain the replacement effect.
    $field_name = 'instance[settings][image_replace][' . $unrelated_style_name . '][source_field]';
    $this->assertNoFieldByName($field_name, NULL, 'Image replace settings not present for untrelated style');

    // Choose the replacement image field as the replacement source.
    $field_name = 'instance[settings][image_replace][' . $this->styleName . '][source_field]';
    $edit = array(
      $field_name => 'image_replacement',
    );
    $this->drupalPost(NULL, $edit, t('Save settings'));

    // Verify that no message is displayed if the mapping changes when there is
    // no existing content.
    $this->assertNoText('The image replacement settings have been modified. As a result, it is necessary to rebuild the image replacement mapping for existing content. Note: The replacement mapping is updated automatically when saving an entity.');

    // Post new content.
    $edit = array(
      'title' => $this->randomName(),
      'promote' => 1,
    );
    $edit['files[image_original_' . LANGUAGE_NONE . '_0]'] = drupal_realpath($original_file->uri);
    $edit['files[image_replacement_' . LANGUAGE_NONE . '_0]'] = drupal_realpath($replacement_file->uri);
    $this->drupalPost('node/add/article', $edit, t('Save'));
    $this->assertResponse(200);

    preg_match('/node\/([0-9]+)/', $this->getUrl(), $matches);
    $node = node_load($matches[1]);

    // Verify that the original image is shown on the full node view.
    $generated_url = file_create_url($node->image_original[LANGUAGE_NONE][0]['uri']);
    $this->assertRaw(check_plain($generated_url), 'Original image displayed');

    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the original image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsOriginal($generated_uri), 'The generated file should be the same as the original file on full node view.');

    // Verify that the replacement image is shown on the teaser.
    $this->drupalGet('node');
    $generated_url = image_style_url($this->styleName, $node->image_original[LANGUAGE_NONE][0]['uri']);
    $this->assertRaw(check_plain($generated_url), format_string('Image displayed using style @style.', array('@style' => $this->styleName)));

    $generated_image_data = $this->drupalGet($generated_url);
    $this->assertResponse(200);

    // Assert that the result is the replacement image.
    $generated_uri = file_unmanaged_save_data($generated_image_data);
    $this->assertTrue($this->imageIsReplacement($generated_uri), 'The generated file should be the same as the replacement file on teaser.');

    // Go back to the field settings and reset the replacement mapping.
    $field_name = 'instance[settings][image_replace][' . $this->styleName . '][source_field]';
    $edit = array(
      $field_name => '0',
    );
    $this->drupalPost('admin/structure/types/manage/article/fields/image_original', $edit, t('Save settings'));

    // Verify that a message is displayed if the mapping changes when there is
    // existing content.
    $this->assertText('The image replacement settings have been modified. As a result, it is necessary to rebuild the image replacement mapping for existing content. Note: The replacement mapping is updated automatically when saving an entity.');
  }

}
