<?php
/**
 * @file
 * Internationalization (i18n) hooks
 */


/**
 * Implements hook_i18n_object_info().
 *
 * Defines the Translation Set for handling Media files.
 */
function media_translation_i18n_object_info() {
  $info = array(
    'file' => array(
      'title' => t('Media Translation'),
      'entity' => 'file',
      'key' => 'fid',
      'class' => 'media_translation_translation_wrapper',
    )
  );
  $info['media_type'] = array(
    'title' => t('File Type'),
    'entity' => 'media_type',
    'key' => 'name',
    'translation container' => array(
      'name' => t('file type'),
      'item type' => 'file_managed',
      'item name' => t('files'),
      'options' => array(I18N_MODE_NONE, I18N_MODE_TRANSLATE),
    ),
  );


  return $info;
}


/**
 * Implements hook_i18n_translation_set_info().
 *
 * @see hook_i18n_translation_set_info().
 */
function media_translation_i18n_translation_set_info() {
  $info['file'] = array(
    'title' => t('Media Translation'),
    'class' => 'media_translation_translation_set',
    'table' => 'file_managed',
    'field' => 'i18n_tsid',
    'placeholder' => '%media_translation_translation_set',
    'list path' => 'media/%file/translate',
    'edit path' => 'media/%file/translate',
    'delete path' => 'media-translation/translation-set/%media_translation_translation_set/delete',
  );

  return $info;
}
