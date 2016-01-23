<?php

/**
 * @file
 * Class that represents a converter of type media.
 */

namespace Drupal\maps_import\Converter;

use Drupal\maps_import\Mapping\Media as MappingMedia;

class Media extends Converter {

  /**
   * @inheritdoc
   *
   * Provide the code property as default value for the converter add form.
   */
  private $uid = 'property:code';

  /**
   * @inheritdoc
   */
  public function entityInfo() {
    return array_intersect_key(parent::entityInfo(), array_flip(array('file')));
  }

  /**
   * @inheritdoc
   */
  public function getMapping() {
    if (!isset($this->mapping)) {
      $this->mapping = new MappingMedia($this);
    }

    return $this->mapping;
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'media';
  }

  /**
   * @inheritdoc
   */
  public function isMappingAllowed() {
    $media_types = maps_import_get_maps_media_types($this->getProfile());
    $options = $this->getProfile()->getOptions();

    foreach ($media_types as $key => $media_type) {
      if (empty($options['media_settings'][$key]['path'])) {
        return FALSE;
      }

      $dir = "{$this->getProfile()->getMediaAccessibility()}://{$this->getProfile()->getMediaDirectory()}/{$options['media_settings'][$key]['path']}";

      if (!file_prepare_directory($dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function hasAdditionalOptions() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function optionsForm($form, &$form_state) {
    $options = $this->getOptions();

    $form_options = array(
      'file_management' => array(
        '#title' => t('File management'),
        '#type' => 'radios',
        '#options' => array(
          'local' => t('Use local files'),
          'download' => t('Download the related remote files'),
        ),
        '#default_value' => isset($options['file_management']) ? $options['file_management'] : 'local',
        '#required' => TRUE,
      )
    );

    $presets = maps_import_image_presets($this->getProfile());

    // @todo add some options for UID, UID scope or
    // entity_type/bundle fields if their values
    // are changed on an existing converter.
    // What to do with the entities created by this
    // converter? Deleted?
    // Kept but deleting maps_import_entities related entries ?
    if (!empty($presets)) {
      $form['preset'] = array(
        '#type' => 'select',
        '#title' => t('Preset'),
        '#default_value' => !empty($options['presets']) ? $options['presets'] : '',
        '#description' => t('Select the entity type.'),
        '#options' => array('' => '- Select -') + $presets,
        '#states' => array(
          'visible' => array(
            ':input[name="bundles[file]"]' => array('value' => 'image'),
          ),
          'required' => array(
            ':input[name="bundles[file]"]' => array('value' => 'image'),
          ),
        ),
      );
    }

    return $form_options;
  }

}
