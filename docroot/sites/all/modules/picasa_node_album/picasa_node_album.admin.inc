<?php

/**
 * @file
 * Admin callbacks for the picasa_node_album module.
 */

/**
 * Administration settings.
 */
function picasa_node_album_admin() {
  if (!$client = _picasa_node_album_get_client()) {
    drupal_set_message(t('There was a problem connecting to this gmail account. Please update the master account settings.'), 'error');
  }

  $form['master_account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Master Google Account for API Queries')
  );
  $form['master_account']['picasa_node_album_google_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Username'),
    '#description' => t('This is the master account which will be used to login to the Google API'),
    '#size' => 20,
    '#default_value' => variable_get('picasa_node_album_google_user', ''),
  );
  $form['master_account']['picasa_node_album_google_pass'] = array(
    '#type' => 'password',
    '#title' => t('Google Password'),
    '#description' => t('If left blank, it will not override the last password set.'),
    '#size' => 20,
    '#default_value' => '',
  );

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Picasa Account and Restrictions')
  );
  $form['account']['picasa_node_album_accounts'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed Google Accounts'),
    '#description' => t('A list of Picasa  accounts users are allowed to use. If any account is allowed, leave blank. One account per line.'),
    '#default_value' => variable_get('picasa_node_album_accounts', '')
  );
  $form['account']['picasa_node_album_exclude_accounts'] = array(
    '#type' => 'textarea',
    '#title' => t('Disallowed Google Accounts'),
    '#description' => t('Users will not be allowed to use these accounts. One account name per line.'),
    '#default_value' => variable_get('picasa_node_album_exclude_accounts', '')
  );
  $form['account']['picasa_node_album_default_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Picasa Account'),
    '#description' => t('This is the default account that will be active if a user has not selected a custom one.'),
    '#default_value' => variable_get('picasa_node_album_default_account', '')
  );

  $form['account']['picasa_node_album_per_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Albums Per Node'),
    '#description' => t('The number of albums per node users are allowed to upload. Set to 0 for unlimited.'),
    '#default_value' => variable_get('picasa_node_album_per_node', 1),
    '#size' => 8
  );

  $form['viewing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings')
  );

  $types = array();
  $selected_types = _picasa_node_album_get_node_types();
  $result = db_select('node_type', 'nt')
    ->fields('nt', array('type', 'name'))
    ->orderBy('name')
    ->execute();
  foreach ($result as $record) {
    $types[$record->type] = $record->name;
  }

  $form['viewing']['picasa_node_album_node_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Node Types'),
      '#description' => t('Check the node types you want to enable Picasa Node Album on.'),
      '#options' => $types,
      '#default_value' => $selected_types
  );

  $options = array(
    0 => t('Hide Image Captions'),
    1 => t('Show Image Captions')
  );
  $form['viewing']['picasa_node_album_captions'] = array(
    '#type' => 'radios',
    '#title' => t('Image Caption Settings'),
    '#description' => t('This setting exposes the image title to the templates.'),
    '#options' => $options,
    '#default_value' => variable_get('picasa_node_album_captions', 0)
  );

  // these options are defined in the API docs
  $thumb_size_options = drupal_map_assoc(array(32, 48, 64, 72, 104, 144, 150, 160));
  $image_size_options = drupal_map_assoc(array(
    94, 110, 128, 200, 220, 288, 320, 400, 512, 576, 640, 720, 800, 912, 1024, 1152, 1280, 1440, 1600)
  );

  $form['viewing']['picasa_node_album_image_size'] = array(
    '#type' => 'select',
    '#title' => t('Image Size returned by Google'),
    '#options' => $image_size_options,
    '#default_value' => variable_get('picasa_node_album_image_size', 576),
    '#description' => t('The size of the image google returns.'),
  );
  $form['viewing']['picasa_node_album_thumb_size'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail Size returned by Google'),
    '#options' => $thumb_size_options,
    '#default_value' => variable_get('picasa_node_album_thumb_size', 72),
    '#description' => t('The size of the thumbnail google returns.'),
  );

  $form['viewing']['picasa_node_album_preview_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Images per album for preview'),
    '#description' => t('This number of images will display on the node page or teaser with a link to a full album view. Set to 0 for unlimited. For example, if set to 4, only the first 4 images of the album will be displayed on the node, and when you click to view more, you will be taken to a page with all the images in that gallery.'),
    '#default_value' => variable_get('picasa_node_album_preview_num', 0),
    '#size' => 8
  );

  $form['viewing']['picasa_node_album_view_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expose Albums Tab'),
    '#description' => t('This will display an albums tab on the node page which will link to a list of all albums for the specified node.'),
    '#default_value' => variable_get('picasa_node_album_view_tab', 0)
  );

  $form['viewing']['picasa_node_album_view_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show in teasers'),
    '#default_value' => variable_get('picasa_node_album_view_teaser', 0)
  );

  $form['viewing']['picasa_node_album_view_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show in full pages'),
    '#default_value' => variable_get('picasa_node_album_view_page', 1)
  );

  $form['viewing']['picasa_node_album_view_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#size' => 5,
    '#default_value' => variable_get('picasa_node_album_view_weight', 10)
  );

  $form['performance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance Settings')
  );

  $options = array(
    0 => t('<none>'),
    300 => t('5 min'),
    900 => t('15 min'),
    1800 => t('30 min'),
    3600 => t('1 hour'),
    21600 => t('6 hours'),
    43200 => t('12 hours'),
    86400 => t('1 day'),
    604800 => t('1 week'),
    1209600 => t('2 weeks'),
    -1  => t('Cache until flushed')
  );
  $form['performance']['picasa_node_album_cache_min'] = array(
    '#type' => 'select',
    '#title' => t('Minimum cache lifetime'),
    '#description' => t('Your server needs to contact Google every time it loads fresh album data. If you set a cache lifetime of 1 day, then your server will store the results locally and only contact Google every day.'),
    '#options' => $options,
    '#default_value' => variable_get('picasa_node_album_cache_min', 86400)
  );

  $form['performance']['flush_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flush Picasa node album cache'),
    '#description' => t('Checking this will cause all cached image data for Picasa node album to be flushed when this form is submitted.')
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save Settings'));
  return $form;
}

/**
 * Validate handler
 */
function picasa_node_album_admin_validate(&$form, $form_state) {

  // setting these in the validation hook because we need to validate the default user against them
  variable_set('picasa_node_album_accounts', $form_state['values']['picasa_node_album_accounts']);
  variable_set('picasa_node_album_exclude_accounts', $form_state['values']['picasa_node_album_exclude_accounts']);

  // need to run this down here because we need to set the new allowed/disallowed vars first
  if (!_picasa_node_album_validate_account($form_state['values']['picasa_node_album_default_account'])) {
    form_set_error('picasa_node_album_default_account', t('Your default account is not valid in relation to the allowed/disallowed user settings.'));
    return;
  }
}

/**
 * Submit handler. Had to make it custom to handle the password logic
 */
function picasa_node_album_admin_submit(&$form, $form_state) {

  variable_set('picasa_node_album_google_user', $form_state['values']['picasa_node_album_google_user']);
  // only set the pass if they submitted a value
  if ($form_state['values']['picasa_node_album_google_pass']) {
    variable_set('picasa_node_album_google_pass', $form_state['values']['picasa_node_album_google_pass']);
  }

  variable_set('picasa_node_album_default_account', $form_state['values']['picasa_node_album_default_account']);
  variable_set('picasa_node_album_captions', $form_state['values']['picasa_node_album_captions']);
  variable_set('picasa_node_album_per_node', $form_state['values']['picasa_node_album_per_node']);
  variable_set('picasa_node_album_node_types', serialize($form_state['values']['picasa_node_album_node_types']));
  variable_set('picasa_node_album_cache_min', $form_state['values']['picasa_node_album_cache_min']);
  variable_set('picasa_node_album_preview_num', $form_state['values']['picasa_node_album_preview_num']);
  variable_set('picasa_node_album_view_teaser', $form_state['values']['picasa_node_album_view_teaser']);
  variable_set('picasa_node_album_view_page', $form_state['values']['picasa_node_album_view_page']);
  variable_set('picasa_node_album_view_weight', $form_state['values']['picasa_node_album_view_weight']);
  variable_set('picasa_node_album_view_tab', $form_state['values']['picasa_node_album_view_tab']);

  // flush images if a new style thumb or image size was defined.
  $flush_cache = $form_state['values']['flush_cache'] == 1 ? TRUE : FALSE;
  $thumbsize = variable_get('picasa_node_album_thumb_size', 0);
  $imagesize = variable_get('picasa_node_album_image_size', 0);
  $new_thumbsize = $form_state['values']['picasa_node_album_thumb_size'];
  $new_imagesize = $form_state['values']['picasa_node_album_image_size'];

  if ($flush_cache || $thumbsize != $new_thumbsize || $imagesize != $new_imagesize) {
    db_query("UPDATE {picasa_node_album} SET images = ''");
    if ($flush_cache) {
      drupal_set_message(t('Picasa node album cache has been flushed.'));
    }
    else {
      drupal_set_message(t('Thumb or image size change detected: Picasa node album cache has been flushed.'));
    }
  }

  // set new image sizes
  variable_set('picasa_node_album_thumb_size', $new_thumbsize);
  variable_set('picasa_node_album_image_size', $new_imagesize);

  // now clear menu cache since there is logic
  module_invoke('menu', 'rebuild');

  drupal_set_message(t('Picasa node album settings saved successfully.'));
}
