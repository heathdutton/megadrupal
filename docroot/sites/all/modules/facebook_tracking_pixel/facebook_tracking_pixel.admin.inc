<?php

/**
 * @file facebook_tracking_pixel.admin.inc
 * Admin pages.
 *
 * @author Brady Owens <info@fastglass.net>
 */

/**
 * Form builder for core modules settings.
 */

function facebook_tracking_pixel_settings_form($form_state) {
  $form = [];
  $form['intro_0'] = [
    '#markup' => '<p>' . t('Global Facebook Tracking Pixel Codes.') . '</p>',
  ];

  $form['aggregation'] = [
    '#type' => 'fieldset',
    '#description' => t('Aggregation settings for the javascripts. Default setting is to allow aggregation. Uncheck to disable aggregation of the tracking javascript codes.'),
  ];

  $form['aggregation']['facebook_tracking_pixel_aggregation'] = [
    '#type' => 'checkbox',
    '#title' => 'JS Aggregation',
    '#default_value' => variable_get('facebook_tracking_pixel_aggregation', TRUE),
  ];

  $form['facebook_tracking_pixel_path'] = [
    '#title' => t('File storage path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('facebook_tracking_pixel_path', 'public://facebook_tracking_pixel'),
    '#description' => t('The subdirectory of the system files directory where Facebook Tracking Pixel will store its files. Note: changing this path will cause the old path to move to the new path, overwriting the new path, if it exists.'),
    '#validate' => ['facebook_tracking_pixel_settings_form_validate'],
  ];

  // Render the role overview.
  $result = db_query('SELECT * FROM {role} ORDER BY name');

  $form['facebook_tracking_pixel_roles'] = [
    '#type' => 'fieldset',
    '#title' => t('Roles to track'),
    '#collapsible' => TRUE,
    '#description' => t('Define what user roles should be tracked with a Facebook pixel.'),
  ];

  foreach ($result as $role) {
    // Can't use empty spaces in varname.
    $role_name = $role->name;
    $role_varname = str_replace(' ', '_', $role_name);
    $form['facebook_tracking_pixel_roles']['facebook_tracking_pixel_roles_' . $role_varname] = [
      '#type' => 'checkbox',
      '#title' => t($role_name),
      '#default_value' => variable_get('facebook_tracking_pixel_roles_' . $role_varname, FALSE),
    ];
  }


  return system_settings_form($form);
}

/**
 * Validation for setting form.
 * @param $form
 * @param $form_state
 */

function facebook_tracking_pixel_settings_form_validate($form, &$form_state) {
  // Trim slashes from the path.
  $form_state['values']['facebook_tracking_pixel_path'] = trim($form_state['values']['facebook_tracking_pixel_path'], '/');

  // If the path changed, notify that the folder must be moved.
  $old_path = variable_get('facebook_tracking_pixel_path', 'public://facebook_tracking_pixel_path');
  if ($form_state['values']['facebook_tracking_pixel_path'] != $old_path and file_exists(drupal_realpath($old_path))) {
    $base_path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files') . '/';
    drupal_set_message(t('The file storage path has changed; thus, the contents of %old_path must manually be moved to %new_path.', [
      '%old_path' => $base_path . file_uri_target($old_path),
      '%new_path' => $base_path . file_uri_target($form_state['values']['facebook_tracking_pixel_path']),
    ]), 'warning', FALSE);
  }
}

/**
 * Form builder for base tracking codes.
 */
function facebook_tracking_pixel_base_codes_form() {
  // @todo Add and exclusion field for the base codes to disable for the whole site.
  $result = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c')
    ->orderBy('weight')
    ->execute()
    ->fetchAllAssoc('base_code_id');

  $resultcount = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c')
    ->countQuery()
    ->execute()
    ->fetchField();

  $form['base_codes']['#tree'] = TRUE;
  // Iterate through each database result.
  if ($resultcount > 0) {
    foreach ($result as $item) {
      $isglobal = ($item->base_code_global == 1) ? 'Yes' : 'No';
      $form['base_codes'][$item->base_code_id] = [
        'bcid' => [
          '#type' => 'hidden',
          '#value' => $item->base_code_id,
        ],
        'name' => [
          '#markup' => check_plain($item->base_code_name),
        ],
        'fbid' => [
          '#markup' => check_plain($item->base_code_fbid),
        ],
        'global' => [
          '#markup' => t($isglobal),
        ],
        'weight' => [
          '#type' => 'weight',
          '#default_value' => $item->weight,
          '#delta' => 10,
          '#title_display' => 'invisible',
          '#attributes' => [
            'class' => ['base-codes-weight'],
          ],
        ],
      ];
      // Determine the maximum weight value to use below for the add line.
      if (!isset($max_weight) || $item->weight > $max_weight) {
        $max_weight = $item->weight;
      }
    }
  }

  $form['add']['name'] = [
    '#type' => 'textfield',
    '#size' => 24,
    '#maxlength' => 255,
  ];
  $form['add']['fbid'] = [
    '#type' => 'textfield',
    '#default_value' => '',
    '#size' => 20,
    '#maxlength' => 255,
  ];
  $form['add']['global'] = [
    '#type' => 'checkbox',
    '#default_value' => 1,
  ];
  $form['add']['weight'] = [
    '#type' => 'weight',
    '#default_value' => 0,
    '#delta' => $resultcount > 10 ? $resultcount : 10,
    '#title_display' => 'invisible',
  ];
  // @todo this does not work yet.
  if (isset($_GET['bcid'])) {
    // Make the new component appear by default directly after the one that was
    // just added.
    $form['add']['weight']['#default_value'] = $result[$_GET['bcid']]->weight + 1;
    foreach ($result as $cid) {
      // Adjust all later components also, to make sure none of them have the
      // same weight as the new component.
      if ($form['base_codes'][$cid]['weight']['#default_value'] >= $form['add']['weight']['#default_value']) {
        $form['base_codes'][$cid]['weight']['#default_value']++;
      }
    }
  }
  else {
    // If no component was just added, the new component should appear by
    // default at the end of the list.
    $form['add']['weight']['#default_value'] = isset($max_weight) ? $max_weight + 1 : 0;
  }

  $form['add']['add'] = [
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 45,
    '#submit' => ['facebook_tracking_pixel_base_codes_add_form_submit'],
    '#validate' => ['facebook_tracking_pixel_base_codes_add_fbid_validate'],
  ];

  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save Arrangement'),
    '#weight' => 45,
  ];
  return $form;
}

/**
 * Submit callback for the Facebook Base Codes.
 *
 * Saves the weights of the items.
 * @param $form
 * @param $form_state
 */
function facebook_tracking_pixel_base_codes_form_submit($form, &$form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  if (!empty($form_state['values']['base_codes'])) {
    foreach ($form_state['values']['base_codes'] as $id => $item) {
      db_query(
        "UPDATE {facebook_tracking_pixel_base_codes} SET weight = :weight WHERE base_code_id = :id",
        [':weight' => $item['weight'], ':id' => $id]
      );
    }
  }
}

/**
 * Theme callback for the base code settings form.
 *
 * @param $variables
 * @return string
 */
function theme_facebook_tracking_pixel_base_codes_form($variables) {
  $form = $variables['form'];

  // Initialize the variable to hold the rows of the table.
  $rows = [];
  // Table header.
  $header = [
    t('Name'),
    t('Facebook ID Number'),
    t('Global'),
    t('Weight'),
    ['data' => t('Operations'), 'colspan' => 2],
  ];

  // Add a row containing form elements for a new item.
  unset($form['add']['name']['#title'], $form['add_type']['#description']);
  $form['add']['name']['#attributes']['rel'] = t('New Tracking ID name');
  $form['add']['name']['#attributes']['class'] = ['base-code-name-default-value'];
  $form['add']['fbid']['#attributes']['class'] = ['base-code-fbid'];
  $form['add']['weight']['#attributes']['class'] = ['base-code-weight'];
  $row_data = [
    drupal_render($form['add']['name']),
    drupal_render($form['add']['fbid']),
    drupal_render($form['add']['global']),
    drupal_render($form['add']['bcid']) . drupal_render($form['add']['weight']),
    ['colspan' => 2, 'data' => drupal_render($form['add']['add'])],
  ];
  $add_form = [
    'data' => $row_data,
    'class' => ['draggable', 'base-code-add-form'],
  ];
  $form_rendered = FALSE;
  // Iterate over each element in our $form['base_codes'] array.
  foreach (element_children($form['base_codes']) as $id) {
    $rows[] = [
      'data' => [
        // Add our 'name' column.
        drupal_render($form['base_codes'][$id]['name']),
        // Add our 'description' column.
        drupal_render($form['base_codes'][$id]['fbid']),
        // Add our 'global' column.
        drupal_render($form['base_codes'][$id]['global']),
        // Add our 'weight' column.
        drupal_render($form['base_codes'][$id]['weight']),
        l(t('Edit'), 'admin/config/system/facebook_tracking_pixel/base_codes/edit/' . $form['base_codes'][$id]['bcid']['#value'], ['query' => drupal_get_destination()]),
        l(t('Delete'), 'admin/config/system/facebook_tracking_pixel/base_codes/delete/' . $form['base_codes'][$id]['bcid']['#value'], ['query' => drupal_get_destination()]),
      ],
      // To support the tabledrag behaviour, we need to assign each row of the
      // table a class attribute of 'draggable'. This will add the 'draggable'
      // class to the <tr> element for that row when the final table is
      // rendered.
      'class' => ['draggable'],
    ];

  }
  // Append the add form if not already printed.
  if ($add_form) {
    $rows[] = $add_form;
  }

  $output = '';
  $output .= theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['id' => 'facebook-base-tracking-ids'],
  ]);
  $output .= drupal_render_children($form);
  drupal_add_tabledrag('facebook-base-tracking-ids', 'order', 'sibling', 'base-codes-weight');
  return $output;
}

/**
 * Validation function to make sure the FB Id is an integer.
 * @param $form
 * @param $form_state
 */
function facebook_tracking_pixel_base_codes_add_fbid_validate($form, &$form_state) {
  // Throw an error if no name has been provided.
  if (empty($form_state['values']['name'])) {
    form_error($form['add']['name'], t('Please supply a human readable name.'));
  }
  // Check that the entered component name is valid.
  if (!ctype_digit($form_state['values']['fbid'])) {
    form_error($form['add']['fbid'], t('Facebook ID numbers are numbers only.'));
  }
}

/**
 * Submit handler for the new base code tracking id form.
 * @param $form
 * @param $form_state
 * @return array
 */
function facebook_tracking_pixel_base_codes_add_form_submit($form, &$form_state) {
  $fb_script_src = facebook_tracking_pixel_base_code_js();

  if (!empty($form_state['values']['name']) && !empty($form_state['values']['fbid'])) {
    $name = check_plain($form_state['values']['name']);
    $id = check_plain($form_state['values']['fbid']);
    $isglobal = check_plain($form_state['values']['global']);
    $weight = check_plain($form_state['values']['weight']);
    $filename = 'fb_tkpx.' . $id . '.js';
    // Construct our tracking codes.
    $fb_base_code = $fb_script_src[0] . $id . $fb_script_src[1];

    // Save the global js files for tracking pixel.
    if (!empty($fb_base_code)) {
      // FBID to create the file name because the ID numbers will be unique.
      $success = facebook_tracking_pixel_save_file($fb_base_code, $filename);
      if ($success == FALSE) {
        watchdog('facebook_tracking_pixel', 'Failed writing global JS files for Facebook Tracking Pixel.', [], WATCHDOG_ERROR, NULL);
        return form_set_error(NULL, 'Could not save the JS file. Check the Drupal and PHP logs.', NULL);
      }
      // Write new base code to DB.
      db_insert('facebook_tracking_pixel_base_codes')
        ->fields([
          'base_code_name' => $name,
          'base_code_fbid' => $id,
          'base_code_global' => $isglobal,
          'weight' => $weight,
        ])
        ->execute();
      return TRUE;
    }
    return FALSE;

  }
  return FALSE;
}

/**
 * Form for deleting a base code.
 *
 * @param $form
 * @param $form_state
 * @param $tid
 * @return array
 */
function facebook_tracking_pixel_base_codes_delete_form($form, $form_state, $tid) {
  $id = check_plain($tid);
  $result = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c')
    ->condition('base_code_id', $id, '=')
    ->execute()
    ->fetchAssoc();
  $form = [];
  $form['tid'] = [
    '#type' => 'hidden',
    '#value' => $id,
  ];
  $form['fbid'] = [
    '#type' => 'hidden',
    '#value' => $result['base_code_fbid'],
  ];
  $form['confirmation'] = [
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('You are about to delete the base code name:' . ' ' . $result['base_code_name']),
  ];
  $form['notice'] = [
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('This will also remove any path tracking you have for this base code!'),
  ];
  $form['submit'] = [
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#type' => 'submit',
    '#value' => t('Confirm Delete'),
    '#weight' => 50,
    '#submit' => ['facebook_tracking_pixel_base_codes_delete_form_submit'],
  ];

  return $form;
}

/**
 * Submit handler to delete a base code.
 *
 * @param $form_state
 * @return bool
 */
function facebook_tracking_pixel_base_codes_delete_form_submit($form, &$form_state) {
  if (facebook_tracking_pixel_base_code_delete($form_state['values']['tid'], $form_state['values']['fbid'])) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Form builder for the base code edit function.
 *
 * @param $form
 * @param $form_state
 * @param $tid
 * @return array
 */
function facebook_tracking_pixel_base_codes_edit_form($form, $form_state, $tid) {
  $id = check_plain($tid);
  $result = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c')
    ->orderBy('weight')
    ->condition('base_code_id', $id, '=')
    ->execute()
    ->fetchAssoc();
  $form = [];
  $form['name'] = [
    '#type' => 'textfield',
    '#title' => 'Base Code Given Name',
    '#default_value' => $result['base_code_name'],
  ];
  $form['fbid'] = [
    '#type' => 'textfield',
    '#title' => 'ID Number',
    '#description' => t('The Facebook provided ID number.'),
    '#default_value' => $result['base_code_fbid'],
  ];
  $form['global'] = [
    '#type' => 'checkbox',
    '#title' => 'Global',
    '#description' => t('If this base code is used to track PageViews across the entire site.'),
    '#default_value' => $result['base_code_global'],
  ];
  $form['fbidold'] = [
    '#type' => 'hidden',
    '#value' => $result['base_code_fbid'],
  ];
  $form['tid'] = [
    '#type' => 'hidden',
    '#value' => $result['base_code_id'],
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => ['facebook_tracking_pixel_base_codes_edit_form_submit'],
    '#validate' => ['facebook_tracking_pixel_base_codes_edit_form_validate'],
  ];

  return $form;
}

/**
 * Submit handler for the base code edit form.
 *
 * @param $form
 * @param $form_state
 * @return bool
 */
function facebook_tracking_pixel_base_codes_edit_form_submit($form, &$form_state) {
  $fbid = check_plain($form_state['values']['fbid']);
  $name = check_plain($form_state['values']['name']);
  $isglobal = check_plain($form_state['values']['global']);
  $tid = check_plain($form_state['values']['tid']);
  $fbidold = check_plain($form_state['values']['fbidold']);
  $filename = 'fb_tkpx.' . $fbid . '.js';
  $filenameold = 'fb_tkpx.' . $fbidold . '.js';
  // Delete old file
  if (!facebook_tracking_pixel_delete_file($filenameold)) {
    watchdog('facebook_tracking_pixel', 'Failed to delete previous JS file before adding new file on edit of base code.', [], WATCHDOG_ERROR, NULL);
    return FALSE;
  }
  // Get the JS code.
  $fb_script_js = facebook_tracking_pixel_base_code_js();
  // Attempt to save the new code.
  if (!facebook_tracking_pixel_save_file($fb_script_js[0] . $fbid . $fb_script_js[1], $filename)) {
    watchdog('facebook_tracking_pixel', 'Failed to save new JS file in the base code edit function.', [], WATCHDOG_ERROR, NULL);
  }
  // Now to update the database with the new tracking information.
  if (db_update('facebook_tracking_pixel_base_codes')
    ->fields([
      'base_code_name' => $name,
      'base_code_fbid' => $fbid,
      'base_code_global' => $isglobal,
    ])
    ->condition('base_code_id', $tid, '=')
    ->execute()
  ) {
    facebook_tracking_pixel_clear_cache();
    drupal_set_message(t('Base Code Updated'), 'status', FALSE);
  }
  else {
    drupal_set_message(t('Base Code Not Updated'), 'warning', FALSE);
  }
}

/**
 * Validation function to make sure the FB Id is an integer for the base
 * code edit form.
 *
 * @param $form
 * @param $form_state
 */
function facebook_tracking_pixel_base_codes_edit_form_validate($form, &$form_state) {
  // Throw an error if no name has been provided.
  if (empty($form_state['values']['name'])) {
    form_error($form['name'], t('Please supply a human readable name.'));
  }
  // Check that the entered component name is valid.
  if (!ctype_digit($form_state['values']['fbid'])) {
    form_error($form['fbid'], t('Facebook ID numbers are numbers only.'));
  }
}

/**
 * Form builder for purge feature.
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function facebook_tracking_pixel_form_purge($form, &$form_state) {
  $form['intro_0'] = [
    '#markup' => '<p>' . t('Destroy all codes, records, and files.') . '</p>',
  ];
  $form['intro_1'] = [
    '#markup' => '<p>' . t('This will purge the file system deleting all files and directories and purge the database of all configured paths. Use this function with caution.') . '</p>',
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Purge All'),
    '#submit' => ['facebook_tracking_pixel_form_purge_submit'],
  ];
  return $form;
}

/**
 * Submit handler for purge function of module.
 *
 * @return bool
 */

function facebook_tracking_pixel_form_purge_submit() {
  $status = TRUE;
  $sql = 'TRUNCATE {facebook_tracking_pixel_events_path}';
  if (!db_query($sql)) {
    $status = FALSE;
  }
  $sql = 'TRUNCATE {facebook_tracking_pixel_base_codes}';
  if (!db_query($sql)) {
    $status = FALSE;
  }
  $path = variable_get('facebook_tracking_pixel_path', 'public://facebook_tracking_pixel');
  if (!file_unmanaged_delete_recursive($path)) {
    $status = FALSE;
  }
  if (!$status) {
    drupal_set_message(t('Purging failed check your PHP logs on your server.'), 'error', FALSE);
  }
  else {
    drupal_flush_all_caches();
    drupal_set_message(t('Purging complete'), 'status', FALSE);
  }
  return $status;
}

/**
 * Form builder for core modules settings.
 */

function facebook_tracking_pixel_user_registration($form, &$form_state) {
  $form['intro'] = [
    '#markup' => t('Enabliing this will track user registration for the entire site. To not track users added by admins, do not enable tracking for their roles.'),
  ];
  //Check to see if there are base codes.
  $resultcountbasecode = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c')
    ->countQuery()
    ->execute()
    ->fetchField();
  if ($resultcountbasecode == 0) {
    $form['emptyset'] = [
      '#type' => 'hidden',
      '#value' => TRUE,
    ];
    $form['notice'] = [
      '#markup' => t('You must first set a base code for tracking.'),
    ];
    return $form;
  }
  else {
    // If we have results then build an array to use as an option drop down for
    // the path items.
    $resultbasecode = db_select('facebook_tracking_pixel_base_codes', 'c')
      ->fields('c')
      ->orderBy('weight')
      ->execute()
      ->fetchAllAssoc('base_code_id');
    $basecodes = [];
    foreach ($resultbasecode as $item) {
      $basecodes[$item->base_code_id] = $item->base_code_fbid;
    }
  }
  $enable = variable_get('facebook_tracking_pixel_user_registration_enable', 0);
  $form['facebook_tracking_pixel_user_registration_enable'] = [
    '#title' => t('Enable the tracking of user registrations'),
    '#type' => 'checkbox',
    '#default_value' => $enable,
    '#ajax' => [
      'callback' => 'facebook_tracking_pixel_user_registration_ajax',
      'wrapper' => 'replace-facebook-tracking-pixel-user-registration-basecodes',
      'effect' => 'fade',
    ],
  ];
  $form['facebook_tracking_pixel_user_registration_basecode'] = [
    '#markup' => t('Enable user tracking to set basecode'),
    // '#default_value' => variable_get('freshdesk_widget_popup_type_attribute'),
    '#prefix' => '<div id="replace-facebook-tracking-pixel-user-registration-basecodes">',
    '#suffix' => '</div>',
  ];
  if ((!empty($form_state['values']['facebook_tracking_pixel_user_registration_enable']) && $form_state['values']['facebook_tracking_pixel_user_registration_enable']) || $enable == 1) {
    unset($form['facebook_tracking_pixel_user_registration_basecode']['#options']);
    unset($form['facebook_tracking_pixel_user_registration_basecode']['#type']);
    unset($form['facebook_tracking_pixel_user_registration_basecode']['#markup']);
    $form['facebook_tracking_pixel_user_registration_basecode']['#type'] = 'select';
    $form['facebook_tracking_pixel_user_registration_basecode']['#title'] = t('Facebook Base Tracking Code to Use');
    $form['facebook_tracking_pixel_user_registration_basecode']['#options'] = $basecodes;
    $form['facebook_tracking_pixel_user_registration_basecode']['#default_value'] = variable_get('facebook_tracking_pixel_user_registration_basecode', NULL);
  }
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'Save configuration',
  ];

  return $form;
}

/**
 * AJAX callback to replace button attribute.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function facebook_tracking_pixel_user_registration_ajax($form, $form_state) {
  return $form['facebook_tracking_pixel_user_registration_basecode'];
}

/**
 * Submit handler for user registration admin form.
 *
 * @param $form
 * @param $form_state
 */
function facebook_tracking_pixel_user_registration_submit($form, $form_state) {
  // Sanitize variables for good measure.
  $enabled = check_plain($form_state['values']['facebook_tracking_pixel_user_registration_enable']);
  !empty($enabled) ? variable_set('facebook_tracking_pixel_user_registration_enable', $enabled) : variable_set('facebook_tracking_pixel_user_registration_enable', 0);

  if (!empty($form_state['values']['facebook_tracking_pixel_user_registration_basecode'])) {
    $basecodeid = check_plain($form_state['values']['facebook_tracking_pixel_user_registration_basecode']);
    // This is saving the basecode ID number - not the basecode itself.
    variable_set('facebook_tracking_pixel_user_registration_basecode', $basecodeid);
    $fbscriptjs = facebook_tracking_pixel_base_code_js();
    $events = facebook_tracking_pixel_events_reserved();
    // This is the full JS code to save to the file system.
    $base_code_fbid = db_select('facebook_tracking_pixel_base_codes', 'c')
      ->fields('c', ['base_code_fbid'])
      ->condition('base_code_id', $basecodeid, '=')
      ->execute()
      ->fetchField();
    $data = $fbscriptjs[0] . $base_code_fbid . $fbscriptjs[1] . $events['ftpregistration']['code'];
    // Delete any existing file.
    facebook_tracking_pixel_delete_file('fb_trk_user_registration.js', 'registrationtracking');
    // Save new file.
    facebook_tracking_pixel_save_file($data, 'fb_trk_user_registration.js', 'registrationtracking');
  }
}
