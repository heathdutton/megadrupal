<?php

/**
 * @file
 * Administrative functions for MaPS Import module
 */

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Request\Request;
use Drupal\maps_import\Fetcher\Fetcher;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Plugins\CTools\ExportUI\ProfileExportable;

/**
 * Form builder; MaPS Import profile add/edit form.
 *
 * @see maps_import_admin_profile_form_validate()
 * @see maps_import_admin_profile_form_submit()
 * @ingroup forms
 */
function maps_import_admin_profile_form($form, &$form_state, Profile $profile = NULL) {
  // @todo do not get the schema if the profile is set
  // and add an extra parameter $setDefault to the toArray() method
  // to add the default value or '' string if there is no value.
  $schema = drupal_get_schema('maps_import_profile');
  $default = array();

  foreach ($schema['fields'] as $column => $info) {
    $default[$column] = isset($info['default']) ? $info['default'] : '';
  }

  // Check if there is a defined path for private files.
  $is_defined_private_path = (bool) variable_get('file_private_path');

  if ($profile) {
    $default = array_merge($default, $profile->toArray());
    $form['pid'] = array('#type' => 'value', '#value' => $profile->getPid());
  }

  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $default['title'],
    '#description' => t("The profile's title."),
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $default['name'],
    '#maxlength' => 255,
    '#description' => t("The profile's unique name."),
    '#machine_name' => array(
      'exists' => 'maps_import_profile_name_exists',
      'source' => array('title'),
    ),
    '#access' => empty($default['name']),
  );

  $form['fetch_method'] = array(
    '#title' => t('Fetch method'),
    '#description' => t('Choose between Webservice and Files method'),
    '#type' => 'radios',
    '#options' => array(
      Profile::FETCH_WS => t('Webservice'),
      Profile::FETCH_FILES => t('Files'),
    ),
    '#default_value' => $default['fetch_method'],
  );

  $form['fetch_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fetch Files'),
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_FILES),
      ),
    ),
  );

  $form['fetch_files']['configuration_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Configuration file'),
    '#description' => t('The configuration data file path (inside the files/maps_suite directory).'),
    '#default_value' => $default['configuration_file'],
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_FILES),
      ),
      'required' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_FILES),
      ),
    ),
  );

  $form['fetch_files']['objects_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Objects file'),
    '#description' => t('The objects data file path (inside the files/maps_suite directory).'),
    '#default_value' => $default['objects_file'],
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_FILES),
      ),
      'required' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_FILES),
      ),
    ),
  );

  $form['fetch_ws'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fetch webservice'),
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
    ),
  );

  $form['fetch_ws']['token'] = array(
    '#type' => 'textfield',
    '#title' => t('Security token'),
    '#default_value' => $default['token'],
    '#description' => t('This token is a 40-character secure hash used for authentication against MaPS System® Web services.'),
    '#maxlength' => 40,
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
      'required' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
    ),
  );

  $form['fetch_ws']['publication_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication ID'),
    '#default_value' => $default['publication_id'],
    '#description' => t('Uses MaPS System® ID. Import process targets only object from the specified publication.'),
    '#element_validate' => array('maps_suite_integer_element_validate'),
    // A publication with ID 0 is valid and represents the root (master data).
    '#not_null' => FALSE,
    '#positive' => 1,
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
      'required' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
    ),
  );

  $form['fetch_ws']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('MaPS System® Web Services URL'),
    '#default_value' => $default['url'],
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
      'required' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
    ),
  );

  $form['fetch_ws']['web_template'] = array(
    '#type' => 'select',
    '#title' => t('Web Template'),
    '#options' => array(
      Fetcher::TEMPLATE_EXPORT_WEB => t('Template export Web'),
      Fetcher::TEMPLATE_EXPORT_WEB_DEMO => t('Template export Web demo'),
    ),
    '#default_value' => array($default['web_template']),
    '#description' => 'The name of the web template to use in the MaPS System® Webervices parameters.',
    '#states' => array(
      'visible' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
      'required' => array(
        ':input[name="fetch_method"]' => array('value' => Profile::FETCH_WS),
      ),
    ),
  );

  $form['fetch_ws']['force_empty_cache'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Empty cache'),
    '#options' => array(1 => t('Force empty cache')),
    '#default_value' => isset($default['options']['force_empty_cache']) ? array($default['options']['force_empty_cache']) : array(1),
    '#description' => t('Force MaPS Webservice to clean the cache.'),
  );

  $form['root_object_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Root object ID'),
    '#default_value' => $default['root_object_id'],
    '#description' => t('Uses MaPS System® ID. Related root object is not imported, but used as a starting point for import process.'),
    '#element_validate' => array('maps_suite_integer_element_validate'),
    '#not_null' => TRUE,
    '#positive' => 1,
    '#required' => TRUE,
  );

  $form['media'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media configuration'),
  );

  $form['media']['preset_group_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset group ID'),
    '#default_value' => $default['preset_group_id'],
    '#description' => t('Uses MaPS System® ID. This settings defines the group of image presets which is used for Web publication.'),
    '#element_validate' => array('maps_suite_integer_element_validate'),
    '#not_null' => TRUE,
    '#positive' => 1,
  );

  $form['media']['media_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Media directory'),
    '#default_value' => $default['media_directory'],
    '#description' => t('The relative path inside the Drupal public or private folder.'),
  );

  $media_accessibilities = array(
    Profile::MEDIA_ACCESSIBILITY_PUBLIC => t('Public'),
  );
  if ($is_defined_private_path) {
    $media_accessibilities[Profile::MEDIA_ACCESSIBILITY_PRIVATE] = t('Private');
  }

  $form['media']['media_accessibility'] = array(
    '#type' => 'select',
    '#title' => t('Media accessibility'),
    '#description' => t('The media accessibility (public or private)'),
    '#default_value' => $default['media_accessibility'],
    '#options' => $media_accessibilities,
  );

  $form['limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Objects number limits'),
    '#description' => t('The value %value should only be used for debug purposes and can only be selected if the site is in maintenance mode.', array('%value' => 1)),
  );

  $form['limits']['max_objects_per_request'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of objects per request'),
    '#default_value' => $default['max_objects_per_request'],
    '#options' => Profile::getMaxObjectOptions('request'),
    '#description' => t('This defines the maximum number of objects that may be returned by each HTTP request to MaPS System® Web Services.'),
    '#required' => TRUE,
  );

  $form['limits']['max_objects_per_op'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of objects to process per mapping operation'),
    '#default_value' => $default['max_objects_per_op'],
    '#options' => Profile::getMaxObjectOptions('mapping'),
    '#description' => t('This defines the maximum number of objects that may be processed by each mapping operation.'),
    '#required' => TRUE,
  );

  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Response format'),
    '#options' => array(
      Request::FORMAT_XML => t('XML'),
    ),
    '#default_value' => array($default['format']),
  );

  $form['differential'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Differential import'),
    '#options' => array(1 => t('Allow differential import')),
    '#default_value' => isset($default['options']['differential']) ? array($default['options']['differential']) : array(0),
    '#description' => t('Allow differential import or full import.'),
  );

  $form['enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Status'),
    '#options' => array(1 => t('Enabled')),
    '#default_value' => array($default['enabled']),
    '#description' => t('Inactive profiles are skipped from import process.'),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $profile ? t('Update') : t('Save'),
  );

  if ($profile) {
    $form['actions']['delete'] = array(
      '#markup' => l(t('delete'), 'admin/maps-suite/profiles/' . $profile->getName() . '/delete'),
      '#weight' => 10,
    );
  }

  return $form;
}

/**
 * Form validation handler; MaPS Import profile add/edit form.
 *
 * @see maps_import_admin_profile_form()
 * @see maps_import_admin_profile_form_submit()
 */
function maps_import_admin_profile_form_validate($form, &$form_state) {
  if ($form_state['values']['fetch_method'] === Profile::FETCH_WS && !valid_url($form_state['values']['url'])) {
    form_set_error('url', t('Field %field: please specify a valid URL.', array('%field' => $form['url']['#title'])));
  }

  // Title should be unique.
  if ($form_state['values']['title'] !== $form['title']['#default_value']) {
    if (CacheProfile::getInstance()->loadSingle($form_state['values']['title'], 'title')) {
      form_set_error('title', t('The title @title is already in use.', array('@title' => $form_state['values']['title'])));
    }
  }

  // Validate secure token.
  if (isset($form_state['values']['token']) && strlen($form_state['values']['token'])) {
    // Force lowercase.
    $token = drupal_strtolower($form_state['values']['token']);

    // Secure token should be exactly 40-character
    // long and use hexadecimal characters only.
    if (!preg_match('/^[0-9a-f]{40}$/', $token)) {
      form_set_error('token', t('The authentication token should be exactly 40-character long and only contain hexadecimal characters.'));
    }
    // Corrects case if necessary.
    elseif (strcmp($token, $form_state['values']['token'])) {
      form_set_value($form['token'], $token, $form_state);
    }
  }

  // Validate media directory.
  if (!empty($form_state['values']['media_directory'])) {
    $media_directory = trim($form_state['values']['media_directory'], '/');

    if (!strlen($media_directory)) {
      form_set_error('media_directory', t('The media directory cannot be the same as Drupal file directory.'));
    }
    elseif (!preg_match('/^[a-z0-9\/_-]+$/i', $media_directory)) {
      form_set_error('media_directory', t('The media directory %value is not valid. It should only contains letters, digits and forward slashes.', array('%value' => $media_directory)));
    }
    elseif (empty($form_state['values']['media_accessibility'])) {
      form_set_error('media_accessibility', t('The media accessibility settings are not valid.', array('%value' => $media_directory)));
    }
    else {
      $dir = $form_state['values']['media_accessibility'] . '://' . $media_directory;

      // Use file_exists() to take care of syslinks.
      if (!file_exists($dir)) {
        form_set_error('media_directory', t('The media directory @dir does not exist.', array('@dir' => $dir)));
      }
    }
  }

  // Ensure there is a preset group ID if the media directory is provided.
  if (!empty($form_state['values']['media_directory']) && empty($form_state['values']['preset_group_id'])) {
    form_set_error('preset_group_id', t('You have to provide the preset group ID to work with media.'));
  }

  // Validates limits.
  $maintenance_mode = variable_get('maintenance_mode', 0);

  foreach (element_children($form['limits']) as $key) {
    if (!empty($form_state['values'][$key]) && $form_state['values'][$key] == 1 && !$maintenance_mode) {
      form_set_error($key, t('The value %value should only be used for debug purposes and can only be selected if the site is in maintenance mode.', array('%value' => 1)));
    }
  }
}

/**
 * Form submission handler; MaPS Import profile add/edit form.
 *
 * @see maps_import_admin_profile_form()
 * @see maps_import_admin_profile_form_validate()
 */
function maps_import_admin_profile_form_submit($form, &$form_state) {
//  if ($form_state['values']['fetch_method'] === Profile::FETCH_FILES) {
//    unset($form['fetch_ws']);
//  }
//  else {
//    unset($form['fetch_files']);
//  }


  $pks = $options = array();
  if (!empty($form_state['values']['pid'])) {
    $pks[] = 'pid';
    $profile = maps_import_profile_load($form_state['values']['pid']);
    $options = $profile->getOptions();
  }

  // Flatten status value.
  $form_state['values']['enabled'] = reset($form_state['values']['enabled']);

  // Add values to options.
  $form_state['values']['options'] = array_merge($options, array(
    'differential' => (int) reset($form_state['values']['differential']),
    'force_empty_cache' => (int) reset($form_state['values']['force_empty_cache']),
  ));

  drupal_write_record('maps_import_profile', $form_state['values'], $pks);

  // Clear the cache.
  CacheProfile::getInstance()->clearBinCache();

  $form_state['redirect'] = array('admin/maps-suite/profiles');

  $t_args = array('@title' => $form_state['values']['title']);
  $message = $pks ? t('The profile @title has been updated.', $t_args) : t('The profile @title has been created.', $t_args);
  drupal_set_message(check_plain($message));
}

/**
 * Move the file from the old path to the new path.
 *
 * @param string $old_path
 * @param string $new_path
 */
function maps_import_admin_profile_move_files($old_path, $new_path) {
  $new_dir = explode('/', $new_path);
  array_pop($new_dir);
  $new_dir = implode('/', $new_dir);
  file_prepare_directory($new_dir, FILE_CREATE_DIRECTORY || FILE_MODIFY_PERMISSIONS);

  file_move(file_uri_to_object($old_path), $new_path);
}

/**
 * Form builder; comfirmation form for profile deletion.
 *
 * @see maps_import_admin_profile_delete_confirm_form_submit()
 * @ingroup forms
 */
function maps_import_admin_profile_delete_confirm_form($form, &$form_state, ProfileExportable $profile) {
  $form['converter_delete_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Converter deletion mode'),
    '#options' => array(
      'unlink' => t('Keep the converted entities in Drupal'),
      'delete' => t('Delete all the converted entities'),
    ),
    '#default_value' => 'delete',
    '#description' => t('Caution: if the entities are kept, they will no more be synchronised through MaPS Suite.'),
    '#required' => TRUE,
  );

  $form['library_delete_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Library deletion mode'),
    '#options' => array(
      'unlink' => t('Keep the converted taxonomy terms in Drupal'),
      'delete terms' => t('All the taxonomy terms that have a correspondence with a library are deleted.'),
      'delete vocabularies' => t('The all vocabularies that are mapped with a library are totally removed, including terms.'),
    ),
    '#default_value' => 'delete terms',
    '#description' => t('Caution: if the entities are kept, they will no more be synchronised through MaPS Suite.'),
    '#required' => TRUE,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the profile @title?', array('@title' => $profile->getTitle())),
    'admin/maps-suite/profiles',
    t('This action cannot be undone.'),
    t('Delete'));
}

/**
 * Form submission handler; comfirmation form for profile deletion.
 *
 * @see maps_import_admin_profile_delete_confirm_form()
 */
function maps_import_admin_profile_delete_confirm_form_submit($form, &$form_state) {
  $profile = reset($form_state['build_info']['args'][0]);
  $profile->delete(array(
    'converter delete mode' => $form_state['values']['converter_delete_mode'],
    'library delete mode' => $form_state['values']['library_delete_mode'],
  ));

  CacheProfile::getInstance()->clearBinCache();
  $form_state['redirect'] = array('admin/maps-suite/profiles');
  drupal_set_message(t('The profile @title has been deleted.', array('@title' => $profile->getTitle())));
}
