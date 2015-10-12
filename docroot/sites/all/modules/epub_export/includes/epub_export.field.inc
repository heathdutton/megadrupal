<?php
/**
 * @file
 * Field integration functions for epub_export.
 */

/**
 * Implements hook_field_info().
 */
function epub_export_field_info() {
  return array(
    'epub_export_epub' => array(
      'label' => t('EPUB'),
      'description' => t('This field enables and stores links to EPUB download files.'),
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
      ),
      'instance_settings' => array(
        'file_directory' => '',
      ),
      'default_widget' => 'epub_export_epub_widget',
      'default_formatter' => 'epub_export_epub_formatter',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function epub_export_field_settings_form($field, $instance) {
  $defaults = field_info_field_settings($field['type']);
  $settings = array_merge($defaults, $field['settings']);

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }

  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload desitination'),
    '#options' => $scheme_options,
    '#default_value' => $settings['uri_scheme'],
    '#description' => t('Select where the build EPUB documents should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function epub_export_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['file_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('File directory'),
    '#default_value' => $settings['file_directory'],
    '#description' => t('Optional subdirectory within the upload destination where files will be stored. Do not include preceding or trailing slashes.'),
    '#element_validate' => array('_file_generic_settings_file_directory_validate'),
    '#weight' => 3,
  );

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function epub_export_field_is_empty($item, $field) {
  // Return FALSE to force hook_field_presave as our values are calculated.
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function epub_export_field_widget_info() {
  return array(
    'epub_export_epub_widget' => array(
      'label' => t('EPUB download'),
      'field types' => array('epub_export_epub'),
      'settings' => array(
        'display_stale_epub' => FALSE,

      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function epub_export_field_formatter_info() {
  return array(
    'epub_export_epub_formatter' => array(
      'label' => t('EPUB Default formatter'),
      'field types' => array('epub_export_epub'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function epub_export_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Widget is entirely calculated.
  switch ($instance['widget']['type']) {
    case 'epub_export_epub_widget':
      if (empty($items[$delta])) {
        $element[$delta] = array(
          'status' => array(
            '#type' => 'value',
            '#value' => EPUB_EXPORT_STATUS_NOT_BUILT,
          ),
          'version' => array(
            '#type' => 'value',
            '#value' => 1,
          ),
          'fid' => array(
            '#type' => 'value',
            '#value' => 0,
          ),
        );
      }
      else {
        $element[$delta] = array(
          'status' => array(
            '#type' => 'value',
            '#value' => EPUB_EXPORT_STATUS_NEEDS_REBUILD,
          ),
          'version' => array(
            '#type' => 'value',
            '#value' => $items[$delta]['version'] + 1,
          ),
          'fid' => array(
            '#type' => 'value',
            '#value' => $items[$delta]['fid'],
          ),
        );
      }
      break;
  }
  return $element;
}
/**
 * Implements hook_field_formatter_view().
 */
function epub_export_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  if ($display['type'] == 'epub_export_epub_formatter') {
    foreach ($items as $delta => $item) {
      switch ($item['status']) {
        case EPUB_EXPORT_STATUS_NOT_BUILT:
          $element[$delta] = array('#markup' => 'EPUB not built yet.');
          break;

        case EPUB_EXPORT_STATUS_NEEDS_REBUILD:
          $element[$delta] = array('#markup' => 'EPUB waiting for build.');
          break;

        case EPUB_EXPORT_STATUS_ERRORS:
          $element[$delta] = array('#markup' => 'Errors during EPUB build.');
          break;

        case EPUB_EXPORT_STATUS_READY:
          if ($file = file_load($item['fid'])) {
            $element[$delta] = array('#markup' => theme('file_link', array('file' => $file)));
          }
          break;
      }
    }
  }
  return $element;
}

/**
 * Implements hook_field_storage_delete().
 */
function epub_export_field_storage_delete($field) {
  // @todo do any necessary cleanup when flagging field for deletion
}

/**
 * Implements hook_field_storage_delete_instance().
 */
function epub_export_field_storate_delete_instance($instance) {
  // @todo do we cleanup/delete related info here or defer until actuall
  // deletion takes place?
  // @todo decrement file usage counter if necessary
}

/**
 * Implements hook_field_presave().
 *
 * @see epub_export_attach_epub()
 */
function epub_export_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // We just set the field to "Needs rebuild" hook_entity_update() will actually
  // build and attach the EPUB since we need to ensure the node is fully updated
  // before building our EPUB document. We only do this in the event that we are
  // not being called from a running epub documenet build process.
  if (empty($entity->epub_export_in_progress)) {
    $items[0]['status'] = EPUB_EXPORT_STATUS_NEEDS_REBUILD;
  }

  if (!isset($items[0]['version'])) {
    $items[0]['version'] = 1;
  }

  if (!isset($items[0]['fid'])) {
    $items[0]['fid'] = 0;
  }
}
