<?php
/**
 * @file
 * Controls the admin interface of this module.
 */

/* PATHS ADMIN PAGE
 */
function print_anything_paths_admin_form($form, &$form_state) {
  // Overview of all formats.
  $result = db_select('print_anything_paths', 'p')
    ->fields('p', array('pid', 'path', 'weight'))
    ->orderBy('p.weight')
    ->execute()->fetchAll();
  
  $form['#tree'] = TRUE;
  $form['#theme'] = 'print_anything_paths_admin_form';
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => 'The order of the paths should go from specific to general (top to bottom) with the exception of the default, which should always remain at the top. This is because once a match is found the system stops looking. So for example with the paths "path/subpath/one", "path/subpath/*" and "path/*", they should be in that order down the list. Use "&#60;front&#62;" for the home page. If no match is found, the default settings will be used.',
  );
  foreach ($result as $row) {
    if ($row->path == '/') {
      $form['paths'][$row->pid]['path'] = array(
        '#markup' => 'Default *(Should remain at the top)',
      );
      $form['paths'][$row->pid]['ops'] = array(
        '#markup' => l(t('Manage CSS IDs'), PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $row->pid)
      );
    }
    elseif ($row->path == '<front>') {
      $form['paths'][$row->pid]['path'] = array(
        '#markup' => 'Home Page',
      );
      $form['paths'][$row->pid]['ops'] = array(
        '#markup' => l(t('Edit'), PRINT_ANYTHING_ADMINPATH . '/path/' . $row->pid . '/edit')
          . ' | ' . l(t('Delete'), PRINT_ANYTHING_ADMINPATH . '/path/' . $row->pid . '/delete')
          . ' | ' . l(t('Manage CSS IDs'), PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $row->pid)
        );
    }
    else {
      $form['paths'][$row->pid]['path'] = array(
        '#markup' => $row->path,
      );
      $form['paths'][$row->pid]['ops'] = array(
        '#markup' => l(t('Edit'), PRINT_ANYTHING_ADMINPATH . '/path/' . $row->pid . '/edit')
          . ' | ' . l(t('Delete'), PRINT_ANYTHING_ADMINPATH . '/path/' . $row->pid . '/delete')
          . ' | ' . l(t('Manage CSS IDs'), PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $row->pid)
      );
    }

    $form['paths'][$row->pid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $row->pid)),
      '#title_display' => 'invisible',
      '#default_value' => $row->weight,
    );

    $form['paths'][$row->pid]['pid'] = array(
      '#type' => 'hidden',
      '#value' => $row->pid,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  if (count($result) > 1) {
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  }
  else {
    $form['no_paths_records'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no custom paths configured.')
    );
  }
  return $form;
}

function print_anything_paths_admin_form_submit($form, $form_state) {
  foreach ($form_state['values']['paths'] as $path) {
    $query = db_update('print_anything_paths')
      ->fields(array(
      'weight' => $path['weight']))
      ->condition('pid', $path['pid'])
      ->execute();
  }
  drupal_set_message('The path order has been saved', 'status');
}

/* ADD A PATH PAGE */
function print_anything_path_add() {
  $output = drupal_get_form('print_anything_path_add_form');
  return $output;
}
function print_anything_path_add_form($form, &$form_state) {
  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Back to list'), PRINT_ANYTHING_ADMINPATH . '/paths') . '</p>'
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path')
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Path')
  );
  return $form;
}
function print_anything_path_add_form_validate($form, &$form_state) {
  $result = db_select('print_anything_paths', 'p')
    ->fields('p', array('pid'))
    ->condition('path', $form_state['values']['path'])
    ->countQuery()->execute()->fetchField();

  if ($result) {
    form_set_error('Edit Path', t('This path has already been saved'));
  }
}
function print_anything_path_add_form_submit($form, &$form_state) {
  $rid = db_insert('print_anything_paths')
    ->fields(array(
        'path' => $form_state['values']['path'],
        'weight' => 0,
      ))
    ->execute();

  $form_state['redirect'] = PRINT_ANYTHING_ADMINPATH . '/paths';
  drupal_set_message(t('The path "@path" has been added.', array('@path' => $form_state['values']['path'])), 'status');
  watchdog('print_anything', 'The path "@path" was added.', array('@path' => $form_state['values']['path']), WATCHDOG_INFO);
}

/* EDIT A PATH PAGE */
function print_anything_path_edit($rid) {
    $output = drupal_get_form('print_anything_edit_path_form', $rid);
    return $output;
}
function print_anything_edit_path_form($node, &$form_state, $pid) {
  $result = db_select('print_anything_paths', 'p')
    ->fields('p', array('pid', 'path'))
    ->condition('pid', $pid)
    ->execute()->fetchAll();

  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Back to list'), PRINT_ANYTHING_ADMINPATH . '/paths') . '</p>'
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $result[0]->path
  );
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Submit'));
  return $form;
}
function print_anything_edit_path_form_validate($form, &$form_state) {
  $result = db_select('print_anything_paths', 'p')
    ->fields('p', array('pid'))
    ->condition('path', $form_state['values']['path'])
    ->countQuery()->execute()->fetchField();

  if ($result) {
    form_set_error('Edit Path', t('This path has already been saved'));
  }
}
function print_anything_edit_path_form_submit($form, &$form_state) {
  $query = db_update('print_anything_paths')
    ->fields(array('path' => $form_state['values']['path']))
    ->condition('pid', $form_state['values']['pid'])
    ->execute();

  $form_state['redirect'] = PRINT_ANYTHING_ADMINPATH . '/paths';
  drupal_set_message(t('The path "@path" has been updated.', array('@path' => $form_state['values']['path'])), 'status');
  watchdog('print_anything', 'The path "@path" was updated', array('@path' => $form_state['values']['path']), WATCHDOG_INFO);
}

/* DELETE A PATH PAGE */
function print_anything_path_delete($pid) {
  return drupal_get_form('print_anything_path_delete_form', $pid);
}
function print_anything_path_delete_form($node, &$form_state, $pid) {
  $result = db_select('print_anything_paths', 'p')
    ->fields('p', array('pid', 'path'))
    ->condition('pid', $pid)
    ->execute()->fetchAll();

  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Back to list'), PRINT_ANYTHING_ADMINPATH . '/paths') . '</p>'
  );
  $form['markup'] = array(
    '#markup' => '<p>' . t('Delete the path "!id"', array('!id' => $result[0]->path)) . '</p><p>This cannot be undone.</p>'
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $result[0]->pid
  );
  $form['path'] = array(
    '#type' => 'hidden',
    '#value' => $result[0]->path
  );
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Delete Path'));
  return $form;
}
function print_anything_path_delete_form_submit($form, &$form_state) {
  // Delete the path
  $query = db_delete('print_anything_paths')
    ->condition('pid', $form_state['values']['pid'])
    ->execute();
  // Delete the path's css ids
  $query = db_delete('print_anything')
    ->condition('pid', $form_state['values']['pid'])
    ->execute();
  $form_state['redirect'] = PRINT_ANYTHING_ADMINPATH . '/paths';
  drupal_set_message(t('The path "@path" and all it\'s related CSS IDs has been deleted.', array('@path' => $form_state['values']['path'])), 'status');
  watchdog('print_anything', 'The path "@path" and all it\'s related CSS IDs was deleted', array('@path' => $form_state['values']['path']), WATCHDOG_INFO);
}

/* LIST OF CSS IDS PAGE */
function print_anything_cssids_admin($pid) {
  return drupal_get_form('print_anything_cssids_admin_form', $pid);
}
function print_anything_cssids_admin_form($node, &$form_state, $pid) {
  // Overview of all formats.
  $result = db_select('print_anything', 'p')
    ->fields('p', array('rid', 'pid', 'css_id', 'weight'))
    ->condition('pid', $pid)
    ->orderBy('p.weight')
    ->execute()->fetchAll();

  $form['#tree'] = TRUE;
  $form['#theme'] = 'print_anything_cssids_admin_form';
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => 'The order of the ids is the order they will appear in the print page.',
  );
  foreach ($result as $row) {
    $form['css_ids'][$row->css_id]['css_id'] = array(
        '#markup' => $row->css_id,
    );
    $form['css_ids'][$row->css_id]['ops'] = array(
        '#markup' => l(t('Edit'), PRINT_ANYTHING_ADMINPATH . '/css-id/' . $row->rid . '/edit') . ' | ' . l(t('Delete'), PRINT_ANYTHING_ADMINPATH . '/css-id/' . $row->rid . '/delete')
    );
    $form['css_ids'][$row->css_id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $row->css_id)),
      '#title_display' => 'invisible',
      '#default_value' => $row->weight,
    );
    $form['css_ids'][$row->css_id]['rid'] = array(
      '#type' => 'hidden',
      '#value' => $row->rid,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  if (count($result)) {
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  }
  else {
    $form['no_cssid_records'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no CSS ids configured.')
    );
  }
  return $form;
}

function print_anything_cssids_admin_form_submit($form, $form_state) {
  foreach ($form_state['values']['css_ids'] as $css_id) {
    $query = db_update('print_anything')
      ->fields(array(
      'weight' => $css_id['weight']))
      ->condition('rid', $css_id['rid'])
      ->execute();
  }
  drupal_set_message('The CSS IDs order has been saved', 'status');
}

/* ADD A CSS ID PAGE */
function print_anything_cssid_add($pid) {
  return drupal_get_form('print_anything_cssid_add_form', $pid);
}
function print_anything_cssid_add_form($form, &$form_state, $pid) {
  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Back to CSS list'), PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $pid) . '</p>'
  );
  $form['css_id'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS ID')
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#default_value' => $pid
  );
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add CSS ID'));
  return $form;
}
function print_anything_cssid_add_form_submit($form, &$form_state) {
  $rid = db_insert('print_anything')
    ->fields(array(
      'css_id' => $form_state['values']['css_id'],
      'pid' => $form_state['values']['pid'],
      'weight' => 0
    ))
    ->execute();

  $form_state['redirect'] = PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $form_state['values']['pid'];
  drupal_set_message(t('The CSS ID "@css_id" has been added.', array('@css_id' => $form_state['values']['css_id'])), 'status');
  watchdog('print_anything', 'The CSS ID "@css_id" was added', array('@css_id' => $form_state['values']['css_id']), WATCHDOG_INFO);
}

/* EDIT A CSS ID PAGE */
function print_anything_cssid_edit($rid) {
  $output = drupal_get_form('print_anything_edit_cssid_form', $rid);
  return $output;
}
function print_anything_edit_cssid_form($node, &$form_state, $rid) {
  $result = db_select('print_anything', 'p')
    ->fields('p', array('rid', 'pid', 'css_id', 'weight'))
    ->condition('rid', $rid)
    ->orderBy('p.weight')
    ->execute()->fetchAll();

  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Back to CSS list'), PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $result[0]->pid) . '</p>'
  );
  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid
  );
  $form['css_id'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS ID'),
    '#default_value' => $result[0]->css_id
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#default_value' => $result[0]->pid
  );
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Submit'));
  return $form;
}
function print_anything_edit_cssid_form_submit($form, &$form_state) {
  $query = db_update('print_anything')
    ->fields(array(
      'css_id' => $form_state['values']['css_id']))
    ->condition('rid', $form_state['values']['rid'])
    ->execute();
  $form_state['redirect'] = PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $form_state['values']['pid'];
  drupal_set_message(t('The CSS ID "@css_id" has been updated.', array('@css_id' => $form_state['values']['css_id'])), 'status');
  watchdog('print_anything', 'The CSS ID "@css_id" was updated', array('@css_id' => $form_state['values']['css_id']), WATCHDOG_INFO);
}

/* DELETE A CSS ID PAGE */
function print_anything_cssid_delete($rid) {
  return drupal_get_form('print_anything_cssid_delete_form', $rid);
}
function print_anything_cssid_delete_form($form, &$form_state, $rid) {
  $result = db_select('print_anything', 'p')
    ->fields('p', array('rid', 'pid', 'css_id', 'weight'))
    ->condition('rid', $rid)
    ->orderBy('p.weight')
    ->execute()->fetchAll();

  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Back to CSS List'), PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $result[0]->pid) . '</p>'
  );
  $form['markup'] = array(
    '#markup' => '<p>' . t('Delete the CSS Id "!id"', array('!id' => $result[0]->css_id)) . '</p><p>' . t('This cannot be undone.') . '</p>'
  );
  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $result[0]->pid
  );
  $form['css_id'] = array(
    '#type' => 'hidden',
    '#value' => $result[0]->css_id
  );
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Delete CSS ID'));
  return $form;
}
function print_anything_cssid_delete_form_submit($form, &$form_state) {
  $query = db_delete('print_anything')
    ->condition('rid', $form_state['values']['rid'])
    ->execute();
  $form_state['redirect'] = PRINT_ANYTHING_ADMINPATH . '/css-ids/' . $form_state['values']['pid'];
  drupal_set_message(t('The CSS ID "@css_id" has been deleted.', array('@css_id' => $form_state['values']['css_id'])), 'status');
  watchdog('print_anything', 'The CSS ID "@css_id" was deleted', array('@css_id' => $form_state['values']['css_id']), WATCHDOG_INFO);
}

/* SETTINGS */
function print_anything_settings_admin_form($form, &$form_state) {
  $form['back_link'] = array(
    '#markup' => '<p style="text-align:right">' . l(t('Go to List of Paths'), PRINT_ANYTHING_ADMINPATH . '/paths') . '</p>'
  );

  // If there is already an uploaded image display the image here.
  if ($image_fid = variable_get('print_anything_image_fid', FALSE)) {
    $image = file_load($image_fid);
    $style = variable_get('print_anything_image_style_name', 'thumbnail');
    $form['logo'] = array(
      '#markup' => theme('image', array('image' => $image, 'style' => $style)),
    );
  }

  // Use the #managed_file FAPI element to upload an image file.
  $form['print_anything_image_fid'] = array(
    '#title' => t('Header Image'),
    '#type' => 'managed_file',
    '#description' => t('The uploaded image will be displayed on the printer page in the upper left corner using the image style chosen below.'),
    '#default_value' => variable_get('print_anything_image_fid', ''),
    '#upload_location' => 'public://',
  );

  // Provide a select field for choosing an image style to use when displaying
  // the image.
  if (module_exists('image')) {
   $form['print_anything_image_style_name'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#description' => t('Choose an image style to use when displaying the above image.'),
      '#options' => image_style_options(TRUE),
      '#default_value' => isset($style) ? $style : '',
    );
  }
  $form['header'] = array(
    '#type' => 'textarea',
    '#title' => t('Header'),
    '#description' => t('Header is displayed to the right of Header Logo.'),
    '#default_value' => variable_get('print_anything_header', '') ? variable_get('print_anything_header', '') : '',
  );

  $form['footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer'),
    '#default_value' => variable_get('print_anything_footer', '') ? variable_get('print_anything_footer', '') : '',
  );
  $form['print_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for print link'),
    '#description' => t('If left blank, the link will default to "Print".'),
    '#default_value' => variable_get('print_anything_link_text', '') ? variable_get('print_anything_link_text', '') : '',
  );
  $form['include_wrappers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Entire DOM Object'),
    '#description' => t('The default behavior in the print-friendly page is to display the inner content (innerHTML) of the targeted DOM elements. In some cases, this will deliver some unexpected results. When using the Panels module for example, the wrapper often contains all the css identifiers that apply the styles. Checking this box will force the inclusion of the entire DOM element. The markup will be more complex, but the styling will be easier.'),
    '#default_value' => variable_get('print_anything_include_wrappers', '') ? variable_get('print_anything_include_wrappers', 0) : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function print_anything_settings_admin_form_submit($form, &$form_state) {
  // When using the #managed_file form element the file is automatically
  // uploaded an saved to the {file} table. The value of the corresponding
  // form element is set to the {file}.fid of the new file.

  // If fid is not 0 we have a valid file
  // and fid does not equal the already stored ID, process the image upload
  if ($form_state['values']['print_anything_image_fid'] != 0 &&
      $form_state['values']['print_anything_image_fid'] != variable_get('print_anything_image_fid', null)) {
    // The new file's status is set to 0 or temporary and in order to ensure
    // that the file is not removed after 6 hours we need to change it's status
    // to 1. Save the ID of the uploaded image for later use.
    $file = file_load($form_state['values']['print_anything_image_fid']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    // When a module is managing a file, it must manage the usage count.
    // Here we increment the usage count with file_usage_add().
    file_usage_add($file, 'print_anything', 'logo_image', 1);

    // Save the fid of the file so that the module can reference it later.
    variable_set('print_anything_image_fid', $file->fid);
    if ($form_state['values']['print_anything_image_style_name']) {
      $message = 'The image "@image_name" was saved with a file ID of @fid and the image style "@style".';
      $watchdogMsg = 'The image "@image_name" was saved with a file ID of @fid and the image style "@style".';
    } else {
      $message = 'The image "@image_name" was saved with a file ID of @fid and no image style.';
      $watchdogMsg = 'The image "@image_name" was saved with a file ID of @fid and no image style.';
    }
    drupal_set_message(
      t($message,
        array(
          '@image_name' => $file->filename,
          '@fid' => $file->fid,
          '@style' => $form_state['values']['print_anything_image_style_name'])
      ), 'status');
    watchdog(
      'print_anything',
      $watchdogMsg,
      array(
        '@image_name' => $file->filename,
        '@fid' => $file->fid,
        '@style' => $form_state['values']['print_anything_image_style_name']),
      WATCHDOG_INFO);
  }
  // If the file was removed we need to remove the module's reference to the
  // removed file's fid, and remove the file.
  elseif ($form_state['values']['print_anything_image_fid'] == 0) {
    // Retrieve the old file's id.
    $fid = variable_get('print_anything_image_fid', FALSE);
    $file = $fid ? file_load($fid) : FALSE;
    if ($file) {
      // When a module is managing a file, it must manage the usage count.
      // Here we decrement the usage count with file_usage_delete().
      file_usage_delete($file, 'print_anything', 'logo_image', 1);

      // The file_delete() function takes a file object and checks to see if
      // the file is being used by any other modules. If it is the delete
      // operation is cancelled, otherwise the file is deleted.
      file_delete($file);
    }

    // Either way the module needs to update it's reference since even if the
    // file is in use by another module and not deleted we no longer want to
    // use it.
    variable_set('print_anything_image_fid', FALSE);
    if (isset($file->filename)) {
      drupal_set_message(t('The image "@image_name" was removed.', array('@image_name' => $file->filename)), 'status');
      watchdog('print_anything', 'The image "@image_name" was removed.', array('@image_name' => $file->filename), WATCHDOG_INFO);
    }
  }
  // Save the name of the image style chosen by the user.
  variable_set('print_anything_image_style_name', $form_state['values']['print_anything_image_style_name']);

  // Save all the other config settings
  variable_set('print_anything_header', $form_state['values']['header']);
  variable_set('print_anything_footer', $form_state['values']['footer']);
  variable_set('print_anything_link_text', $form_state['values']['print_link_text']);
  variable_set('print_anything_include_wrappers', $form_state['values']['include_wrappers']);

  drupal_set_message('The configuration settings have been saved.', 'status');
  watchdog('print_anything', 'The configuration settings were saved', null, WATCHDOG_INFO);
}