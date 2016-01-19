<?php

/**
 * @file
 * Field API integration for the media_youtube_upload module.
 */

/**
 * Implements hook_field_widget_form().
 */
function media_youtube_upload_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  switch ($instance['widget']['type']) {
    case 'media_youtube_upload':

      // Get the element info first.
      module_load_include('admin.inc', 'media_youtube_upload');
      $element_info = element_info('media_youtube_upload_upload');
      $element += $element_info;
      $element['#tree'] = TRUE;

      // Setup for the Field UI Configuration form.
      if ($form_state['build_info']['form_id'] === 'field_ui_field_edit_form' && $form_state['build_info']['args'][0]['widget']['type'] === 'media_youtube_upload') {

        $input_fid = isset($form_state['input'][$element['#field_name']][$element['#language']][$element['#delta']]['fid']) ? $form_state['input'][$element['#field_name']][$element['#language']][$element['#delta']]['fid'] : 0;
        $fid = isset($items[$delta]['fid']) ? $items[$delta]['fid'] : $input_fid;
        // If the file was accidentally deleted remove the value so we can make
        // a new one. TODO: Check on file delete if a file is a configuration
        // file for a youtube field. And if it is, deny deleting it.
        $value = file_load($fid) ? $fid : '';

        $element += _media_youtube_upload_get_field_default_settings_form($element, $form_state, $value);
      }
      // Set the defaults and field state for our field.
      else {
        $defaults = array(
          'fid' => 0,
          'display' => !empty($field['settings']['display_default']),
          'description' => '',
        );

        // Load the items for form rebuilds from the field state as they might
        // not be in $form_state['values'] because of validation limitations.
        // Also, they are only passed in as $items when editing existing
        // entities.
        $field_state = field_form_get_state($element['#field_parents'], $field['field_name'], $langcode, $form_state);
        if (isset($field_state['items']) && array_key_exists($delta, $field_state['items']) && empty($items[$delta])) {
          $element['#default_value'] = !empty($field_state['items']) ? $field_state['items'][$delta] : $defaults;
        }
        elseif (array_key_exists($delta, $items)) {
          $element['#default_value'] = !empty($items[$delta]) ? $items[$delta] : $defaults;
        }
      }
      break;
  }
  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function media_youtube_upload_field_widget_info() {
  $youtube_fields = field_info_fields('file', 'youtube');
  $fields = array_keys($youtube_fields);
  $element_info = element_info('media_youtube_upload_upload');
  return array(
    'media_youtube_upload' => array(
      'label' => t('YouTube Upload'),
      'field types' => array('file'),
      'settings' => array(
        'upload_validators' => array(
          'allowed_extensions' => MEDIA_YOUTUBE_UPLOADER_ALLOWED_EXTENSIONS,
          'max_uploadsize' => MEDIA_YOUTUBE_UPLOADER_MAX_UPLOADSIZE,
        ),
        'youtube_fields' => array(
          'enabled_fields' => $fields,
          'default_values' => array(),
        ),
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_error().
 */
function media_youtube_upload_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}

/**
 * Implements hook_field_settings_form().
 */
function media_youtube_upload_field_widget_settings_form($field, $instance) {

  $widget = $instance['widget'];
  $settings = $widget['settings'];

  if ($widget['type'] == 'media_youtube_upload') {
    // Build the form up from our widget settings.
    $form = _media_youtube_upload_get_field_widget_settings_form($settings);

    return $form;
  }
}

/**
 * Implements hook_field_instance_settings_form().
 */
function media_youtube_upload_field_instance_settings_form($field, $instance) {

  $settings = $instance['settings'];
  // Make the extension list a little more human-friendly by comma-separation.
  $settings['upload_validators']['allowed_extensions'] = str_replace(' ', ', ', $settings['upload_validators']['allowed_extensions']);

  $form = _media_youtube_upload_get_field_instance_settings_form($instance, $settings);

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function media_youtube_upload_field_is_empty($item, $field) {
  if (empty($item['fid'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_presave().
 */
function media_youtube_upload_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $item) {
    if (isset($item['fid'])) {
      $items[$delta]['fid'] = $item['fid'];
    }
  }
}

/**
 * Implements hook_field_delete_instance().
 */
function media_youtube_upload_field_delete_instance($instance) {
  // Delete the configuration file for this field instance.
  if (isset($instance['default_value'][0]['fid'])) {
    $fid = $instance['default_value'][0]['fid'];
    file_delete_multiple(array($fid));
  }
}

/**
 * Helper function to skip validation if the field ui youtube field is empty.
 *
 * @param array $element
 *   The attached fields on file type youtube for field settings configuration.
 *
 * @return array
 *   The element that needs to skip validation if it is empty.
 *
 * @see _media_youtube_upload_get_field_default_settings_form()
 */
function _media_youtube_upload_field_skip_validation_if_empty($element) {
  $element['#required'] = TRUE;
  return $element;
}


/**
 * Element validate callback.
 *
 * Validates the maximum upload size field and ensure a size that can be parsed
 * by parse_size() has been entered.
 *
 * @param array $element
 *   The element for which to validate the maximum upload size.
 * @param array $form_state
 *   The form state.
 *
 * @see _media_youtube_upload_get_field_instance_settings_form()
 */
function _media_youtube_upload_generic_settings_max_filesize($element, &$form_state) {
  if (!empty($element['#value']) && !is_numeric(parse_size($element['#value']))) {
    form_error($element, t('The "@name" option must contain a valid value. You may either leave the text field empty or enter a string like "900 MB" (megabytes) or "20 GB (gigabytes).', array('@name' => t($element['title']))));
  }
}

/**
 * Element validate callback for the allowed file extensions field.
 *
 * This doubles as a convenience clean-up function and a validation routine.
 * Commas are allowed by the end-user, but ultimately the value will be stored
 * as a space-separated list for compatibility with file_validate_extensions().
 *
 * @param array $element
 *   The element for which to validate the allowed extensions.
 * @param array $form_state
 *   The form state.
 *
 * @see _media_youtube_upload_get_field_instance_settings_form()
 */
function _media_youtube_upload_generic_settings_extensions($element, &$form_state) {

  if (!empty($element['#value'])) {
    $unsupported_file_types = array();
    $allowed_extensions = preg_replace('/([, ]+\.?)/', ' ', MEDIA_YOUTUBE_UPLOADER_ALLOWED_EXTENSIONS);
    $allowed_extensions = array_filter(explode(' ', $allowed_extensions));
    $extensions = preg_replace('/([, ]+\.?)/', ' ', trim(drupal_strtolower($element['#value'])));
    $extensions = array_filter(explode(' ', $extensions));
    $diff = array_diff($extensions, $allowed_extensions);
    foreach ($diff as $file_type) {
      if (!in_array($file_type, $allowed_extensions)) {
        $unsupported_file_types[] = $file_type;
      }
    }
    if (!empty($unsupported_file_types)) {
      form_error($element, t('The <b>@name</b> are not valid, be sure to exclude unsupported file types: <b>@unsupported</b>', array('@name' => t($element['#title']), '@unsupported' => implode(', ', $unsupported_file_types))));
    }
    $extensions = implode(' ', array_unique($extensions));
    if (!preg_match('/^([a-z0-9]+([.][a-z0-9])* ?)+$/', $extensions)) {
      form_error($element, t('The <b>@name</b> are not valid, be sure to exclude leading dots and to separate extensions with a comma or space.', array('@name' => t($element['#title']))));
    }
    else {
      form_set_value($element, $extensions, $form_state);
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function media_youtube_upload_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {

  if ($form['#instance']['widget']['type'] === 'media_youtube_upload' && $form['#instance']['widget']['module'] === 'media_youtube_upload') {

    // Unset some settings from the file field we don't really need.
    unset($form['instance']['settings']['file_directory']);
    unset($form['instance']['settings']['file_extensions']);
    unset($form['instance']['settings']['max_filesize']);
    unset($form['instance']['settings']['description_field']);

    $form['instance']['settings'] += _media_youtube_upload_get_field_instance_settings_form($form['#instance'], $form['#instance']['widget']['settings']);
    // Change the text for the allowed values function display to reflect a better
    // explanation on why these allowed values should not be changed.
    if ($form['#field']['field_name'] === 'field_file_youtube_category') {
      $form['field']['settings']['allowed_values_function_display']['#markup'] = t('The allowed values for this field are being determined by the !country_setting. Changing that value can result in loss of data.', array(
        '!country_setting' => l(t('country setting'), 'admin/config/media/media-youtube-upload', array('fragment' => 'media-youtube-upload-app-country'))
      ));
    }
  }
}

/**
 * Helper function to return the youtube categories for the category field.
 *
 * @return array $categories
 *   Returns the youtube categories depending on the YouTube app settings.
 *
 * @see media_youtube_upload_field_bases()
 */
function _media_youtube_upload_allowed_values_for_category_field_function() {

  // Get the categories.
  $ytapi = new MediaYoutubeUploadYtapi();
  $categories = $ytapi->setCategories();
  if (empty($categories['error'])) {
    return $categories;
  }
  $token = variable_get('media_youtube_upload_token', array());
  // If application isn't authenticated yet display error.
  if (empty($token['access_token'])) {
    // Point administrators to the configuration page.
    if (drupal_valid_path('admin/config/media/media_youtube_upload')) {
      drupal_set_message(t('You need to authenticate the application on <a href="@configuration_page">this page</a>.', array('@configuration_page' => url('admin/config/media/media_youtube_upload'))), 'warning');
    }
    // Ask users with insufficient permissions to contact the administrator.
    else {
      drupal_set_message(t('An error has occured while trying to connect to YouTube. Please contact your administrator to resolve this problem.'), 'warning');
    }
  }
  else {
    return $categories;
  }
}

/**
 * Helper function for the field widget settings form.
 *
 * This part of the form saves the enabled fields for our native youtube fields.
 *
 * @param array $settings
 *   The widget settings.
 *
 * @return array $form
 *   The form for field widget settings form.
 *
 * @see media_youtube_upload_field_widget_settings_form()
 * @see media_youtube_upload_upload_settings_form()
 */
function _media_youtube_upload_get_field_widget_settings_form($settings) {

  $form = array();

  // Fieldset for our checkboxes.
  $form['youtube_fields'] = array(
    '#title' => t("YouTube enabled fields."),
    '#prefix' => '<div id="youtube_fields">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#description' => t('Select which fields you want the user to be able to customize.'),
  );
  // Get the fields on our youtube type file.
  $youtube_fields = field_info_instances('file', 'youtube');
  $native_youtube_fields = variable_get('media_youtube_upload_youtube_fields', unserialize(MEDIA_YOUTUBE_UPLOAD_NATIVE_YOUTUBE_FIELDS));
  // Prepare the options for the select box.
  $options = array();
  foreach ($youtube_fields as $youtube_field) {
    if (in_array($youtube_field['field_name'], $native_youtube_fields)) {
      $options[$youtube_field['field_name']] = $youtube_field['label'];
    }
  }
  // The checkboxes form.
  $form['youtube_fields']['enabled_fields'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => isset($settings['youtube_fields']['enabled_fields']) ? $settings['youtube_fields']['enabled_fields'] : array(),
  );

  return $form;
}

/**
 * The upload validators for our instance settings form.
 *
 * @param array $instance
 *   The instance for which the upload validators have to be fetched.
 * @param array $settings
 *   The settings for the upload validators default values.
 *
 * @return array $form
 *   The form for our field instance settings form.
 *
 * @see media_youtube_upload_field_instance_settings_form()
 * @see media_youtube_upload_upload_settings_form()
 */
function _media_youtube_upload_get_field_instance_settings_form($instance = NULL, $settings) {

  $form = array();

  $form['upload_validators'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload validators'),
  );

  $form['upload_validators']['allowed_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed file extensions'),
    '#default_value' => $settings['upload_validators']['allowed_extensions'],
    '#description' => t('Separate extensions with a space or comma and do not include the leading dot. The default allowed file extensions for YouTube are: !supported_types. It is only possible to remove unwanted types. It is not possible to add unsupported types.', array('!supported_types' => MEDIA_YOUTUBE_UPLOADER_ALLOWED_EXTENSIONS)),
    '#element_validate' => array('_media_youtube_upload_generic_settings_extensions'),
    '#weight' => 1,
    '#maxlength' => 256,
    '#required' => TRUE,
  );

  $form['upload_validators']['max_uploadsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size'),
    '#default_value' => $settings['upload_validators']['max_uploadsize'],
    '#description' => t('Enter a value like "900 MB" (megabytes) or 20GB (gigabytes) in order to restrict the allowed file size. If left empty the default will be set at YouTube\'s 60 Gigabyte. Values larger will reset to YouTube\'s maximum upload size as well.'),
    '#size' => 10,
    '#element_validate' => array('_media_youtube_upload_generic_settings_max_filesize'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Helper function to construct the default settings form.
 *
 * @param array $element
 *   The element for which to fetch fields.
 * @param array $form_state
 *   The form state.
 * @param int $value
 *   The file id.
 *
 * @return array $element
 *   The updated element.
 *
 * @see media_youtube_upload_field_widget_form()
 * @see media_youtube_upload_upload_settings_form()
 */
function _media_youtube_upload_get_field_default_settings_form($element = NULL, &$form_state, $value) {
  if (empty($value) || !file_load($value)) {

    $parts = explode("/", request_uri());
    $settings_type = end($parts);
    $settings_type = drupal_ucfirst(str_replace('-', ' ', $settings_type));

    // Create new file object.
    $file = new stdClass();
    $file->uid = $GLOBALS['user']->uid;
    $file->type = 'youtube';
    $file->filename = 'Youtube config: ' . (isset($element['#field_name']) ? $element['#field_name'] : $settings_type);
    // Clean up name for uri.
    $uri_string = str_replace('_', '-', $file->filename);
    $uri_string = str_replace(': ', '/', $uri_string);
    $uri_string = str_replace(' ', '-', $uri_string);
    $file->uri = 'youtube://v/' . strtolower($uri_string);
    $file->filemime = 'youtube/config';
    $file->timestamp = REQUEST_TIME;
    $file->status = FILE_STATUS_PERMANENT;
  }
  else {
    $file = file_load($value);
  }
  $element['#title_display'] = 'invisible';
  // Grab the file for validation.
  $element['#default_file'] = $file;
  // Load the youtube fields.
  $parents = isset($element['#field_name']) && isset($element['#language']) && isset($element['#delta']) ? array(
    $element['#field_name'],
    $element['#language'],
    $element['#delta']
    ) : array();
  $element['youtube_fields'] = array(
    '#type' => empty($parents) ? 'fieldset' : 'container',
    '#parents' => $parents,
  );
  
  $language = LANGUAGE_NONE;
  if (module_exists('entity_translation')) {
    // Get the entity translation default language if there is one set.
    module_load_include('inc', 'entity_translation', 'translation.handler');
    $handler = entity_translation_get_handler('file', $file);
    $language = $handler->getDefaultLanguage();
  }
  
  field_attach_form('file', $file, $element['youtube_fields'], $form_state, $language);

  // Alter the fields.
  $youtube_fields = field_info_instances('file', 'youtube');
  $native_youtube_fields = variable_get('media_youtube_upload_youtube_fields', unserialize(MEDIA_YOUTUBE_UPLOAD_NATIVE_YOUTUBE_FIELDS));
  foreach ($youtube_fields as $youtube_field) {
    if (in_array($youtube_field['field_name'], $native_youtube_fields)) {
      // TODO: Add javascript indicating which fields are shown and which
      // are not. Saying defaults will be loaded regardless.

      // Fake the required fields to skip validation when empty.
      $field_language = $element['youtube_fields'][$youtube_field['field_name']]['#language'];
      if (!empty($element['youtube_fields'][$youtube_field['field_name']][$field_language][0]['value']['#required'])) {
        $element['youtube_fields'][$youtube_field['field_name']][$field_language][0]['value']['#required'] = FALSE;
        $element['youtube_fields'][$youtube_field['field_name']][$field_language][0]['value']['#pre_render'][] = '_media_youtube_upload_field_skip_validation_if_empty';
      }
      elseif (!empty($element['youtube_fields'][$youtube_field['field_name']][$field_language]['#required'])) {
        $element['youtube_fields'][$youtube_field['field_name']][$field_language]['#required'] = FALSE;
        $element['youtube_fields'][$youtube_field['field_name']][$field_language]['#pre_render'][] = '_media_youtube_upload_field_skip_validation_if_empty';
      }
    }
    else {
      // Only display the native values for our default value widget.
      unset($element['youtube_fields'][$youtube_field['field_name']]);
    }
  }
  $element['fid'] = array(
    '#type' => 'hidden',
    '#default_value' => $value,
    '#weight' => -10,
  );

  return $element;
}
