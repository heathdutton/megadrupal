<?php

/**
 * @file facebook_tracking_pixel.admin.path.inc
 * CRUD forms for tracking path entries.
 *
 * @author Brady Owens <info@fastglass.net>
 */

/**
 * Form builder for track by path.
 *
 * @return array
 */
function facebook_tracking_pixel_track_by_path_form() {
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
      $basecodes[$item->base_code_id] = $item->base_code_name;
    }
  }
  $form = [];
  $form['intro_0'] = [
    '#markup' => '<p><h2>' . t('Facebook Event tracking via path') . '</h2></p>',
    '#weight' => -50,
  ];
  $form['intro_1'] = [
    '#markup' => '<p>' . t('For more information regarding event tracking, read Facebook\'s') . ' ' . l('documentation.', 'https://www.facebook.com/business/help/402791146561655', [
        '#external' => TRUE,
        'attributes' => ['target' => '_blank']
      ]) . '</p>',
    '#weight' => -49,
  ];
  $result = db_select('facebook_tracking_pixel_events_path', 'c')
    ->fields('c')
    ->orderBy('weight')
    ->execute()
    ->fetchAllAssoc('event_id');

  $resultcount = db_select('facebook_tracking_pixel_events_path', 'c')
    ->fields('c')
    ->countQuery()
    ->execute()
    ->fetchField();
  // Identify that the elements in 'example_items' are a collection, to
  // prevent Form API from flattening the array when submitted.
  $form['facebook_tracking_pixel_tracking_codes_by_path']['#tree'] = TRUE;

  if ($resultcount > 0) {
    // Iterate through each database result.
    foreach ($result as $item) {
      $form['facebook_tracking_pixel_tracking_codes_by_path'][$item->event_id] = [
        'event_type' => [
          '#markup' => $item->event_type,
        ],
        'event_path' => [
          '#markup' => l($item->event_path, $item->event_path),
        ],
        // This shows the Base Code human readable name but we track based off the serial ID number.
        'event_base_code_id' => [
          '#markup' => $basecodes[$item->event_base_code_id],
        ],
        'event_uid' => [
          '#markup' => $item->event_uid,
        ],
        'weight' => [
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#default_value' => $item->weight,
          '#delta' => 10,
          '#title_display' => 'invisible',
          '#attributes' => [
            'class' => ['path-codes-weight'],
          ],
        ],
        'event_id' => [
          '#type' => 'hidden',
          '#value' => $item->event_id,
        ],
      ];
    }
  }

  $form['add']['event_type'] = array(
    '#type' => 'select',
    '#options' => facebook_tracking_pixel_events_options(),
  );
  $form['add']['event_path'] = [
    '#type' => 'textfield',
    '#size' => 24,
    '#maxlength' => 255,
  ];
  $form['add']['event_base_code_id'] = [
    '#type' => 'select',
    '#options' => $basecodes,
  ];
  $form['add']['weight'] = [
    '#type' => 'weight',
    '#default_value' => 0,
    '#delta' => $resultcount > 10 ? $resultcount : 10,
    '#title_display' => 'invisible',
  ];
  $form['add']['add'] = [
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 45,
    '#validate' => [
      'facebook_tracking_pixel_path_add_validate',
    ],
    '#submit' => ['facebook_tracking_pixel_path_add_form_submit'],
  ];

  // Now we add our submit button, for submitting the form results.
  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save Arrangement'),
    '#submit' => ['facebook_tracking_pixel_track_by_path_form_submit'],
  ];
  return $form;
}

/**
 * Theme callback for the facebook_tracking_pixel_track_by_path_form form.
 *
 * @param $variables
 * @return string
 * @throws \Exception
 */
function theme_facebook_tracking_pixel_track_by_path_form($variables) {
  $form = $variables['form'];

  if (isset($form['emptyset'])) {
    $output = '';
    $output .= drupal_render($form['notice']);
    return $output;
  }

  // Initialize the variable which will store our table rows.
  $rows = [];
  // We now define the table header values.  Ensure that the 'header' count
  // matches the final column count for your table.
  $header = [
    t('Event Type'),
    t('Event Path'),
    t('Base Code ID'),
    t('Unique ID'),
    t('Weight'),
    [
      'data' => t('Operations'),
      'colspan' => 2,
    ],
  ];

  // Add a row containing form elements for a new item.
  unset($form['add']['event_path']['#title']);
  $form['add']['event_path']['#attributes']['rel'] = t('New Tracking Path');
  $form['add']['event_path']['#attributes']['class'] = ['path-code-name-default-value'];
  $form['add']['event_type']['#attributes']['class'] = ['path-code-type'];
  $form['add']['weight']['#attributes']['class'] = ['path-code-weight'];
  $row_data = [
    drupal_render($form['add']['event_type']),
    drupal_render($form['add']['event_path']),
    drupal_render($form['add']['event_base_code_id']),
    drupal_render($form['add']['event_uid']),
    drupal_render($form['add']['event_id']) . drupal_render($form['add']['weight']),
    ['colspan' => 2, 'data' => drupal_render($form['add']['add'])],
  ];
  $add_form = [
    'data' => $row_data,
    'class' => ['draggable', 'path-code-add-form']
  ];

  // Iterate over each element in our $form['example_items'] array.
  foreach (element_children($form['facebook_tracking_pixel_tracking_codes_by_path']) as $id) {
    $rows[] = [
      'data' => [
        // Add our 'event_type' column.
        drupal_render($form['facebook_tracking_pixel_tracking_codes_by_path'][$id]['event_type']),
        // Add our 'event_path' column.
        drupal_render($form['facebook_tracking_pixel_tracking_codes_by_path'][$id]['event_path']),
        // Add the base code ID
        drupal_render($form['facebook_tracking_pixel_tracking_codes_by_path'][$id]['event_base_code_id']),
        // Add our 'event_uid' column.
        drupal_render($form['facebook_tracking_pixel_tracking_codes_by_path'][$id]['event_uid']),
        // Add our 'weight' column.
        drupal_render($form['facebook_tracking_pixel_tracking_codes_by_path'][$id]['weight']),
        // Operations column for editing and deleting.
        l(t('Edit'), 'admin/config/system/facebook_tracking_pixel/path/edit/' . $id, ['query' => drupal_get_destination()]),
        l(t('Delete'), 'admin/config/system/facebook_tracking_pixel/path/delete/' . $id, ['query' => drupal_get_destination()]),

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
  $output .= drupal_render($form['intro_0']);
  $output .= drupal_render($form['intro_1']);
  // We can render our tabledrag table for output.
  $output .= theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['id' => 'facebook-tracking-pixel-path-items-table'],
  ]);

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('facebook-tracking-pixel-path-items-table', 'order', 'sibling', 'path-code-weight');
  return $output;
}

/**
 * Submit handler to save the weights of the path items.
 *
 * @param $form
 * @param $form_state
 */
function facebook_tracking_pixel_track_by_path_form_submit($form, $form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  if (!empty($form_state['values']['facebook_tracking_pixel_tracking_codes_by_path'])) {
    foreach ($form_state['values']['facebook_tracking_pixel_tracking_codes_by_path'] as $id => $item) {
      db_query(
        "UPDATE {facebook_tracking_pixel_events_path} SET weight = :weight WHERE event_id = :id",
        [':weight' => $item['weight'], ':id' => $id]
      );
    }
  }
}

/**
 * Validate handler for path add form.
 *
 * @param $form
 * @param $form_state
 */
function facebook_tracking_pixel_path_add_validate($form, &$form_state) {
  // Check that the entered component name is valid.
  $path = check_plain($form_state['values']['event_path']);
  if (drupal_strlen(trim($path)) <= 0) {
    form_set_error('event_path', t('When adding a new event, the path field is required.'));
  }
  // @todo current paths and basecodes should be compared to submitted data.
  $base_code_id = check_plain($form_state['values']['event_base_code_id']);

  $result = db_select('facebook_tracking_pixel_events_path', 'c')
    ->fields('c')
    ->condition(db_and()->condition('event_base_code_id', $base_code_id, '=')->condition('event_path', $path, '='))
    ->execute()
    ->fetchField();
  // If the same path has been added for an existing base code, reject the add.
  if ($result){
    form_set_error('event_path', t('You cannot add a duplicate event to the same path and base code.'));
  }
  if(!(drupal_lookup_path('source', $path) || drupal_valid_path($path))){
    form_set_error('event_path', t('The path you have entered (:path) does not appear to be a valid path.', [':path' => $path]));
  }
}


/**
 * Submit handler to add a new path based event.
 *
 * @param $form
 * @param $form_state
 * @return bool
 * @throws \Exception
 */
function facebook_tracking_pixel_path_add_form_submit($form, &$form_state) {
  $path_js_real = variable_get('facebook_tracking_pixel_path', 'public://facebook_tracking_pixel');
  $subdir = variable_get('facebook_tracking_pixel_path_subdir', 'pathtracking');
  $uid = uniqid('', TRUE);
  // Get the base code.
  $base_code_script = facebook_tracking_pixel_base_code_js();
  $base_code_id = check_plain($form_state['values']['event_base_code_id']);
  $base_code_fbid = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c', ['base_code_fbid'])
    ->condition('base_code_id', $base_code_id, '=')
    ->execute()
    ->fetchField();

  $path = check_plain($form_state['values']['event_path']);
  $type = check_plain($form_state['values']['event_type']);
  $fb_events = facebook_tracking_pixel_events();
  $event_type = $fb_events[$type];
  $contents = $base_code_script[0] . $base_code_fbid . $base_code_script[1];
  $contents .= $event_type['code'];
  // Write out the new file for this tracking item.
  if (facebook_tracking_pixel_save_file($contents, 'fb_trk.' . $uid . '.js', $subdir)) {
    // Full path and filename.
    $event_js = $path_js_real . '/' . $subdir . '/' . 'fb_trk.' . $uid . '.js';
    try {
      db_insert('facebook_tracking_pixel_events_path')
        ->fields(
          [
            'event_path' => $path,
            'event_type' => $type,
            'event_base_code_id' => $base_code_id,
            'event_js_file' => $event_js,
            'event_uid' => $uid,
            'weight' => 0
          ]
        )
        ->execute();
      drupal_set_message(t('Path tracking entry added.'), 'status', FALSE);
      // Clear caches.
      facebook_tracking_pixel_clear_cache();
    }
    catch (Exception $e) {
      drupal_set_message(t('Path tracking entry failed to write to database.'), 'error', FALSE);
      watchdog('facebook_tracking_pixel', 'Failed writing path tracking Information to database :e.', [':e' => $e], WATCHDOG_ERROR, NULL);
    }
  }
  else {
    drupal_set_message(t('Path tracking entry failed to write JS to file system.'), 'error', FALSE);
    watchdog('facebook_tracking_pixel', 'Failed writing path tracking JS files for Facebook Tracking Pixel. Check PHP logs.', [], WATCHDOG_ERROR, NULL);
  }
  return TRUE;
}

/**
 * Form builder for tracking path delete.
 *
 * @param $form
 * @param $form_state
 * @param $tid
 * @return array
 */
function facebook_tracking_pixel_path_delete_form($form, $form_state, $tid) {
  $id = check_plain($tid);
  $path = db_select('facebook_tracking_pixel_events_path', 'c')
    ->fields('c', ['event_path'])
    ->condition('event_id', $id, '=')
    ->execute()
    ->fetchField();

  $form = [];
  $form['tid'] = [
    '#type' => 'hidden',
    '#value' => $id,
  ];
  $form['confirmation'] = [
    '#markup' => t('You are about to delete the path tracking for path ' . $path),
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 50,
    '#submit' => ['facebook_tracking_pixel_path_delete_form_submit'],
  ];
  return $form;
}

/**
 * Delete submit handler for the delete form.
 *
 * @param $form
 * @param $form_state
 * @return bool
 */
function facebook_tracking_pixel_path_delete_form_submit($form, &$form_state) {

  $id = check_plain($form_state['values']['tid']);
  // Load the UID from the database.
  $uid = db_select('facebook_tracking_pixel_events_path', 'c')
    ->fields('c', ['event_uid'])
    ->condition('event_id', $id, '=')
    ->execute()
    ->fetchField();

  $subdir = variable_get('facebook_tracking_pixel_path_subdir', 'pathtracking');
  $deletefile = 'fb_trk.' . $uid . '.js';
  // Attempt to delete the file.
  $filedelete = facebook_tracking_pixel_delete_file($deletefile, $subdir);
  if ($filedelete) {
    // Delete the entry from the database.
    try {
      db_delete('facebook_tracking_pixel_events_path')
        ->condition('event_id', $id, '=')
        ->execute();
    }
    catch (Exception $e) {
      drupal_set_message(t('Path tracking entry failed to delete from the database.'), 'error', FALSE);
      watchdog('facebook_tracking_pixel', 'Failed deleteing path tracking Information from the database :e.', [':e' => $e], WATCHDOG_ERROR, NULL);
      return FALSE;
    }
  }
  else {
    drupal_set_message(t('Path tracking entry failed to delete file from the filesytem.'), 'error', FALSE);
    watchdog('facebook_tracking_pixel', 'Failed deleteing path tracking JS file form the file system.', [], WATCHDOG_ERROR, NULL);
    return FALSE;
  }

  if ($filedelete) {
    // Clear caches.
    facebook_tracking_pixel_clear_cache();
  }

}

/**
 * Form builder for tracking path edit.
 *
 * @param $form
 * @param $form_state
 * @param $uid
 * @return array
 */
function facebook_tracking_pixel_path_edit_form($form, $form_state, $tid) {
  $result = db_select('facebook_tracking_pixel_events_path', 'e')
    ->condition('event_id', $tid, '=')
    ->fields('e')
    ->execute()
    ->fetchAssoc();
  $resultbasecode = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c')
    ->orderBy('weight')
    ->execute()
    ->fetchAllAssoc('base_code_id');
  $basecodes = [];
  foreach ($resultbasecode as $item) {
    $basecodes[$item->base_code_id] = $item->base_code_name;
  }
  $form = [];
  $pathtypeoptions = facebook_tracking_pixel_events_options();
  $form['event_type'] = [
    '#title' => 'Event Type',
    '#type' => 'select',
    '#options' => $pathtypeoptions,
    '#default_value' => $result['event_type'],
  ];
  $form['event_path'] = [
    '#title' => 'Path for Action',
    '#type' => 'textfield',
    '#size' => 24,
    '#maxlength' => 255,
    '#default_value' => $result['event_path'],
  ];
  $form['event_base_code_id'] = [
    '#title' => 'Facebook Base Code ID',
    '#description' => t('Corresponds to your Facebook advertising account.'),
    '#type' => 'select',
    '#options' => $basecodes,
    '#default_value' => $result['event_base_code_id'],
  ];
  $form['event_uid_markup'] = [
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('Unique ID: ') . $result['event_uid'] . '<br />' . t('This is the current Unique ID. A new value for the Unique ID will be generated when you save this form.'),
  ];
  $form['event_uid'] = [
    '#type' => 'hidden',
    '#value' => $result['event_uid'],
  ];
  $form['event_id'] = [
    '#type' => 'hidden',
    '#value' => $tid,
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 50,
    '#validate' => ['facebook_tracking_pixel_path_add_validate'],
    '#submit' => ['facebook_tracking_pixel_path_edit_form_submit'],
  ];
  return $form;
}

/**
 * Submit handler for path edit form.
 *
 * @param $form
 * @param $form_state
 * @return bool
 */
function facebook_tracking_pixel_path_edit_form_submit($form, $form_state) {
  // Sanitize all input.
  $tid = check_plain($form_state['values']['event_id']);
  $type = check_plain($form_state['values']['event_type']);
  $path = check_plain($form_state['values']['event_path']);
  $base_code_id = check_plain($form_state['values']['event_base_code_id']);
  $uid = check_plain($form_state['values']['event_uid']);
  // Get the base code FBID.
  $base_code_fbid = db_select('facebook_tracking_pixel_base_codes', 'c')
    ->fields('c', ['base_code_fbid'])
    ->condition('base_code_id', $base_code_id, '=')
    ->execute()
    ->fetchField();
  // Our path to files.
  $path_js_real = variable_get('facebook_tracking_pixel_path', 'public://facebook_tracking_pixel');
  // Generate a new UID.
  $uidnew = uniqid('', TRUE);
  // Sub-directory for path items.
  $subdir = variable_get('facebook_tracking_pixel_path_subdir', 'pathtracking');
  // Attempt to delete the existing file.
  $deletefile = 'fb_trk.' . $uid . '.js';
  try {
    facebook_tracking_pixel_delete_file($deletefile, $subdir);
  }
  catch (Exception $e) {
    return FALSE;
  }

  $newfile = 'fb_trk.' . $uidnew . '.js';
  $base_code_script = facebook_tracking_pixel_base_code_js();
  $fb_events = facebook_tracking_pixel_events();
  $event_type = $fb_events[$type];
  $contents = $base_code_script[0] . $base_code_fbid . $base_code_script[1];
  $contents .= $event_type['code'];
  // Write out the new file for this tracking item.
  if (facebook_tracking_pixel_save_file($contents, $newfile, $subdir)) {
    // Full path and filename.
    $event_js = $path_js_real . '/' . $subdir . '/' . 'fb_trk.' . $uidnew . '.js';
    try {
      db_update('facebook_tracking_pixel_events_path')
        ->fields(
          [
            'event_path' => $path,
            'event_type' => $type,
            'event_base_code_id' => $base_code_id,
            'event_js_file' => $event_js,
            'event_uid' => $uidnew,
          ]
        )
        ->condition('event_id', $tid, '=')
        ->execute();
      drupal_set_message(t('Path tracking entry edited.'), 'status', FALSE);
      // Clear caches.
      facebook_tracking_pixel_clear_cache();
    }
    catch (Exception $e) {
      drupal_set_message(t('Path tracking entry failed to update database.'), 'error', FALSE);
      watchdog('facebook_tracking_pixel', 'Failed writing path tracking information to database. Called from edit path item. Check PHP logs. :e', [':e' => $e], WATCHDOG_ERROR, NULL);
    }
  }
  else {
    drupal_set_message(t('Path tracking entry failed to write new JS to file system. Called from edit path item.'), 'error', FALSE);
    watchdog('facebook_tracking_pixel', 'Failed writing path tracking JS files for Facebook Tracking Pixel. Called from edit path item. Check PHP logs.', [], WATCHDOG_ERROR, NULL);
  }

}
