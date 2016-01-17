<?php

/**
 * @file
 * Administrative page callbacks for the filemaker module.
 */

/*************************************************************
 * Admin landing page -- connections and scripts.
 *************************************************************/

/**
 * Page callback for admin settings page (admin/settings/filemaker).
 *
 * @return
 *    Themed HTML table of all the FileMaker connections stored in the Drupal database.
 */
function filemaker_admin_settings_page() {

  $link = l(t('Add a FileMaker connection'), FILEMAKER_ADMIN_PATH . '/connection');
  drupal_set_message($link);

  $link = l(t('Add a web layout'), 'node/add/filemaker');
  drupal_set_message($link);

  $output = '';

  // Connections table.
  $output .= '<h2>FileMaker Connections</h2>';
  $connection_model = new FmConnection();
  $connections = $connection_model->get_all();
  $output .= $connection_model->table($connections);

  return $output;
}

/*************************************************************
 * Connections.
 *************************************************************/

/**
 * Page callback for the filemaker connection page, (admin/settings/filemaker/connection).
 *
 * @return
 *    Form to insert or edit connection and themed HTML table of all the layouts the connection being edited has access to.
 */
function filemaker_admin_settings_connection_page($fmcid = NULL) {
  
  $output = '';

  // Do we have a record to edit? If so, load and pass the FmConnection object to the form.
  $fmcid = (int) $fmcid;
  if ($fmcid > 0) {
    $connection = new FmConnection($fmcid);
    $connection->test_if_valid();
  }

  // Nothing to edit, append form to page to create connection record.
  $export_form = drupal_get_form('filemaker_connection_form', $fmcid);
  $output .= drupal_render($export_form);

  return $output;
}

/**
 * Form to add or edit a single FileMaker connection.
 */
function filemaker_connection_form(array $form, array $form_state, $fmcid) {

  // Do we have a record to edit? If so, load and pass the FmConnection object to the form.
  $fmcid = (int) $fmcid;
  if ($fmcid > 0) {
    $connection = new FmConnection($fmcid);
    return $connection->admin_form($connection);
  }

  // No record to edit; we are creating one. We pass an empty FmConnection object so that default
  // values work on the form without us doing extra work.
  $connection_model = new FmConnection();
  return $connection_model->admin_form($connection_model);
}

/**
 * Submit handler for filemaker_connection_form.
 */
function filemaker_connection_submit(array &$form, array &$form_state) {
  $connection = new FmConnection();
  $connection->set_values_from_form_state($form_state);
  $connection->save();
  $connection->test_if_valid();
  drupal_goto(FILEMAKER_ADMIN_PATH);
}


/**
 * Validates the admin form to add or edit a connection.
 */
function filemaker_connection_validate(array &$form, array &$form_state) {
  $connection_model = new FmConnection();
  $connection_model->validate_form($form_state);
}

/**
 * Deletes a single FileMaker connection.
 */
function filemaker_delete_connection($fmcid) {
  $connection = new FmConnection($fmcid);
  $connection->delete();
  drupal_goto(FILEMAKER_ADMIN_PATH);
}  

/*************************************************************
 * Scripts.
 *************************************************************/

/**
 * Page callback for the FileMaker cron scripts page (admin/settings/filemaker/cron-jobs).
 *
 * @return
 *    Themed table listing all scripts that run at cron, or a message that there aren't any scripts.
 */
function filemaker_admin_settings_scripts_page($fmsid = NULL) {

  $link = l(t('Add a script'), FILEMAKER_ADMIN_PATH . '/cron/script');
  drupal_set_message($link);

  $output = '';

  // Scripts table.
  $script_model = new FmScript();
  $scripts = $script_model->get_all_cron();
  $output .= $script_model->table($scripts);

  return $output;
}

/**
 * Form to add or edit a single FileMaker script.
 *
 * @return
 *    Form to insert or edit a FileMaker script to be run with Drupal cron.
 */
function filemaker_script_form(array $form, array $form_state, $fmsid, $node = 0, $node_tab = false) {
  
  // Do we have a record to edit? If so, load and pass the FmConnection object to the form.
  $fmsid = (int) $fmsid;
  
  if ($fmsid > 0) {
    $script = new FmScript($fmsid);
    return $script->admin_form($script);
  }

  // No record to edit; we are creating one. We pass an empty FmScript object so that default
  // values work on the form without us doing extra work.
  $script_model = new FmScript();
  return $script_model->admin_form($script_model);
}

/**
 * Submit handler for filemaker_script_form.
 */
function filemaker_script_submit(array &$form, array &$form_state) {
  
  $script_model = new FmScript();
  $script_model->set_values_from_form_state($form_state);
  $script_model->save();

  $node_tab = ( ! isset($form_state['build_info']['args'][2])) ? false : 'scripts';
  filemaker_redirect($node_tab);
}

/**
 * Returns an array of FileMaker layouts, for the $options of a select field.
 */
function filemaker_scripts_array($fm) {
  
  $script_options = $fm->listScripts();
  filemaker_is_error($options);

  if (!$script_options) return;

  $options = array();

  foreach ($script_options as $script) {
    $options[$script] = $script;
  }

  return $options;
}

/**
 * Deletes a single FileMaker script.
 */
function filemaker_delete_script($fmsid, $node_tab = false) {
  $script = new FmScript($fmsid);
  $script->delete();
  filemaker_redirect($node_tab);
}

/*************************************************************
 * Delete Form.
 *************************************************************/

/**
 * Creates a form to delete a single record from the mySQL database.
 *
 * @return array
 */
function filemaker_delete_form($form, $form_state, $what_to_delete, $id, $node_tab = false) {
  
  if ($node_tab) {
    $node_tab = $what_to_delete . 's';
  }

  if ($_POST) {
    if ($_POST['op'] == 'Delete') {
      filemaker_delete_record($what_to_delete, $id, $node_tab);
    }
    else {
      filemaker_redirect($node_tab);
    }
  }
  
  $form['confirm_text'] = array(
    '#markup' => '<p>' . t('Are you absolutely sure you want to delete this? There is no undo.') . '</p>',
  );
  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#weight' => 19,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  
  return $form;
}

/**
 * Page callback for node/%node/filemaker/delete_record/%.
 */
function filemaker_delete_fm_record_form(array $form, array $form_state, stdClass $node, $fmid) {
  
  filemaker_redirect_to_settings_if_needed($node);

  if ($_POST) {
    if ($_POST['op'] == 'Delete') {
      filemaker_delete_fm_record($node, $fmid);
    }
    else {
      filemaker_goto_tab('browse', $node->nid);
    }
  }
  
  $form['confirm_text'] = array(
    '#markup' => '<p>' . t('Are you absolutely sure you want to delete this? There is no undo.') . '</p>',
  );
  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#weight' => 19,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  
  return $form;
}

/**
 * Deletes a single record from the FileMaker database.
 */
function filemaker_delete_fm_record($node, $fmid) {
  
  $fx = filemaker_fx_object($node);
  $fx->addDBParam('-recid', $fmid);
  $fx->FMDelete();

  // Perform new find.
  $filemaker = filemaker_web_layout($node);
  $fields = filemaker_get_fields($node);
  $filemaker->perform_find($fields, 'browse');
  
  // Return to browse tab and display new found set.
  filemaker_goto_tab('browse', $node->nid);
}

/**
 * Deletes a single record from the mySQL database.
 */
function filemaker_delete_record($what_to_delete, $id, $node_tab = false) {
  $delete_function = 'filemaker_delete_' . $what_to_delete;
  $delete_function($id, $node_tab);
}

/*************************************************************
 * Node tabs.
 *************************************************************/

/**
 * Redirects user to the specified tab.
 *
 * Tab must be one of:
 *  'fields' => 'layout/fields',
 *  'portals' => 'layout/portals',
 *  'scripts' => 'layout/scripts',
 *  'browse' => 'browse',
 *  'find' => 'find',
 *  'create' => 'create',
 */
function filemaker_goto_tab($tab, $nid = 0) {
  
  try {
    $path = filemaker_get_tab_path($tab);
    $nid = ($nid) ? $nid : filemaker_get_nid();
    drupal_goto('node/' . $nid . '/' . $path);
  }

  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    watchdog('filemaker', $e->getMessage(), array(), WATCHDOG_ERROR);
  }
}

/**
 * Uses the nid from the url to test if the node is valid.
 *
 * @return int
 */
function filemaker_get_nid() {
  
  $nid = arg(1);
  $node = node_load($nid);

  if (is_object($node)) {
    return $nid;
  }
  else {
    throw new Exception('Node does not exist (inside of filemaker.admin.inc/filemaker_goto_tab). nid: ' . $nid, 1);
  }
}

/**
 * Returns the appropriate path for the desired tab.
 *
 * @param string $tab
 * @return string
 */
function filemaker_get_tab_path($tab) {
  
  $tabs = array(
    'fields' => 'layout/fields',
    'portals' => 'layout/portals',
    'scripts' => 'layout/scripts',
    'settings' => 'layout/settings',
    'browse' => 'filemaker/browse',
    'find' => 'filemaker/find',
    'create' => 'filemaker/create',
  );

  if ( ! isset($tabs[$tab])) {
    throw new Exception('Tab does not exist (inside of filemaker.admin.inc/filemaker_goto_tab). Tab: ' . $tab, 1);    
  }
  else {
    return $tabs[$tab];
  }
}

/**
 * Page callback for layout settings tab: node/[nid]/layout/settings.
 *
 * Not calling drupal_get_form() directly from the menu because we need it, since
 * we call it other places.
 */
function filemaker_settings_tab($node) {
  return drupal_get_form('filemaker_settings_form', $node);
}

/**
 * Builds the form for the layout settings tab.
 */
function filemaker_settings_form(array $form, array &$form_state, stdClass $node) {
  
  $web_layout = filemaker_web_layout($node);
  $fx = $web_layout->fx();
  $layout_options = $web_layout->connection->layout_options($fx);
  
  if (filemaker_is_error($layout_options)) {
    drupal_set_message(t('The connection is not valid.'), 'error');
    $message = 'Invalid connection, fmcid: %fmcid';
    watchdog('filemaker', $message, array('%fmcid' => $node->fmcid), WATCHDOG_ERROR);
    $layout_options = array();
  }
  
  return $web_layout->layout_settings_form($layout_options);
}

/**
 * Submit function for the filemaker_settings_form.
 */
function filemaker_settings_submit(array &$form, array &$form_state) {
  $node = $form_state['build_info']['args'][0];
  $node->layout = $form_state['values']['name'];
  $node->drupal_view = $form_state['values']['drupal_view'];
  $node->records_per_page = $form_state['values']['records_per_page'];
  $node->show_record_number = $form_state['values']['show_record_number'];
  node_save($node);
  filemaker_goto_tab('fields');
}

/**
 * Page callback for node/%nid/fields.
 */
function filemaker_fields_tab(stdClass $node) {

  filemaker_redirect_to_settings_if_needed($node);

  $page = '';

  $link = l(t('Add a FileMaker field to this web layout'), 'node/' . $node->nid . '/layout/field/0');
  $page .= theme_item_list(array('type' => 'ul', 'items' => array($link), 'title' => '', 'attributes' => array()));

  // Fields table.
  $field_model = new FmField();
  $fields = $field_model->get_by_nid($node->nid);
  $page .= $field_model->table($fields);
  
  return $page;
}

/**
 * Page callback for node/%nid/field [/%fmfid].
 */
function filemaker_field_form(array $form, array $form_state, stdClass $node, $fmfid = 0) {

  $filemaker = filemaker_web_layout($node);

  // Do we have a record to edit? If so, load and pass the FmField object to the form.
  if ($fmfid > 0) {
    $field = new FmField($fmfid);
    $field->field_options = $filemaker->field_options();
    $form = $field->admin_form();
    $form['field']['fmfid'] = array(
      '#type' => 'hidden',
      '#value' => $fmfid,
    );
    return $form;
  }

  // No record to edit; we are creating one. We pass an empty Fmfield object so that default
  // values work on the form without us doing extra work.
  $field_model = new FmField();
  $field_model->field_options = $filemaker->field_options();
  return $field_model->admin_form();
}

/**
 * Submit handler for filemaker_field_form.
 */
function filemaker_field_submit(array &$form, array &$form_state) {  
  
  if (isset($form['field']['fmfid']['#value'])) {
    $fmfid = $form['field']['fmfid']['#value'];
  }
  else {
    $fmfid = '';
  }

  $field = new FmField($fmfid);
  $field->set_values_from_form_state($form_state);
  $field->save();
  filemaker_goto_tab('fields');
}

/**
 * Deletes a single FileMaker field.
 */
function filemaker_delete_field($fmfid) {
  $field = new FmField($fmfid);
  $field->delete();
  filemaker_goto_tab('fields');
}

function filemaker_web_layout($node) {
  $filemaker = new FmWebLayout($node->nid);
  return $filemaker;
}

/**
 * Page callback for the 'Browse Mode' tab on a filemaker node.
 *
 * Displays a found set of FileMaker records and, optionally a form to edit a single FileMaker
 * record, as well as all portals displaying data related to the record being edited.
 */
function filemaker_browse_tab(stdClass $node, $fmid = NULL) {  
  
  filemaker_redirect_to_settings_if_needed($node);
  $filemaker = filemaker_web_layout($node);
  
  // Normal Drupal node view. Perform default find if there is no found set.
  if ( ! isset($_SESSION['filemaker'][$node->nid]['foundset']['table']) || $fmid == 'default') {
    $filemaker->default_find();
  }
  
  // If this is a subsequent page result, $fmid will be 'page'
  if ($fmid == 'page') {
    $filemaker->perform_find('browse');
  }

  // If there is a record to edit, build form to edit FileMaker record.
  if ($fmid && $fmid != 'page' && $fmid != 'default') {
    $output = drupal_get_form('filemaker_record_form', $node, 'edit', $fmid);
  }
  
  // Otherwise return a found set of records.
  else {
    $output = $filemaker->found_set();
  }

  return $output;
}

/**
 * Form to create or find a record, in FileMaker mode.
 *
 * @param string $what_to_do
 *  Either 'edit' or 'find'
 * @return array
 *  Drupal fapi form array
 */
function filemaker_record_form(array $form, array $form_state, $node, $what_to_do, $fmid = NULL) {
  
  $filemaker = filemaker_web_layout($node);
  $form_type = $what_to_do . '_form';
  $record = array();
  
  if ($fmid) {
    $fx = $filemaker->fx();
    $fx->addDBParam('-recid', $fmid);
    $result = $fx->FMFind();
    $record = $result['data'];
  }
  
  // Either edit_form() or find_form().
  return $filemaker->$form_type($record);
}

/**
 * Submit function for filemaker_record_form::edit_form.
 *
 * Creates a single FileMaker record.
 */
function filemaker_create_submit(&$form, &$form_state, $mode = 'filemaker') {
  
  $node = node_load($form_state['build_info']['args'][0]->nid);
  $filemaker = filemaker_web_layout($node);
  
  // Add the DBParams based on data input.
  $fx = $filemaker->fx();
  $fields = $filemaker->get_params_from_form_state_input($form_state['input']);
  $fx = filemaker_add_db_params($fx, $fields);
  
  // Save the new record to the FileMaker database.
  $fx->FMNew();
  
  // Error or success message.
  if ( ! filemaker_is_error($fx)) {
    drupal_set_message(t('Record saved to FileMaker.'));
    watchdog('filemaker', 'FileMaker record created', array(), WATCHDOG_INFO);
    
    if ($mode == 'filemaker') {
      // If record saved, update found set so new record is included.
      $filemaker->perform_find('create');

      // Return to browse tab and display new found set.
      filemaker_goto_tab('browse', $nid);
    }
  }
}

/**
 * Adds paramaters to an FX object.
 * Attach valid query parameters to the fx object and return it.
 */
function filemaker_add_db_params(FX $fx, array $fields) {
  foreach ($fields as $key => $val) {
    if ($val) { 
      $fx->addDBParam($key, $val);
    }
  }
  return $fx;
}

/**
 * Submit function for filemaker_browse_form.
 *
 * Edits a single FileMaker record.
 */
function filemaker_browse_submit(&$form, &$form_state) {
  
  $node = node_load($form_state['build_info']['args'][0]->nid);
  $filemaker = filemaker_web_layout($node);
  $fx = $filemaker->fx();

  // Get the record ID and add DB Param for '-recid'.
  $fmid = $form_state['build_info']['args'][2];
  $fx->addDBParam('-recid', $fmid); 
  
  // Add the DBParams based on data input.
  $fields = $filemaker->get_params_from_form_state_input($form_state['input']);
  $fx = filemaker_add_db_params($fx, $fields);
  
  // Update record.
  $fx->FMEdit();

  // Success or error message.
  if ( ! filemaker_is_error($fx)) {
    drupal_set_message(t('Record edited in FileMaker.'));
    watchdog('filemaker', 'FileMaker record updated', array(), WATCHDOG_INFO);
    
    // If record saved, update found set so new record is included.
    $filemaker->perform_find('browse');
    
    // Return to browse tab and display new found set.
    filemaker_goto_tab('browse', $node->nid);
  }
}

/**
 * Submit function for filemaker_record_form::find_form.
 *
 * Submits a find request.
 */
function filemaker_find_submit(&$form, &$form_state, $mode = 'filemaker') {

  $nid = $form_state['build_info']['args'][0]->nid;
  
  $node = node_load($nid);
  $filemaker = filemaker_web_layout($node);
  $fields = $filemaker->get_params_from_form_state_input($form_state['input']);
  $fields = $filemaker->prepare_fields_for_find($fields);
  
  // Add the DBParams based on data input.
  $fx = $filemaker->fx();
  $fx = filemaker_add_db_params($fx, $fields);
  
  // Store find request in session for use in perform_find.
  $_SESSION['filemaker'][$node->nid]['foundset']['request'] = serialize($fx);
  
  // Perform find.
  $filemaker->perform_find('find');

  // Return to browse tab and display new found set.
  if ($mode == 'filemaker') {
    filemaker_goto_tab('browse', $nid);
  }
}

/**
 * Sends user to either:
 *  - The specified tab of a filemaker node.
 *  - The administrative page for the FileMaker module.
 */
function filemaker_redirect($tab = false) {
  if ($tab) {
    filemaker_goto_tab($tab);
  }
  else {
    drupal_goto(FILEMAKER_ADMIN_PATH);
  }
}

/**
 * Redirects to layout settings for a node if there isn't a connection selected.
 *
 * If node's layout is not specified, send user to Settings tab and display message.
 */
function filemaker_redirect_to_settings_if_needed($node) {
  if (isset($node->layout) && $node->layout) {
    return;
  }
  drupal_set_message('Please select which FileMaker layout to use.');
  filemaker_goto_tab('settings', $node->nid);
}

/**
 * Tests what we expect to be an FX object to see if it is actualy an FX_Error object.
 *
 * Logs the error and displays it to the user, if there is an error.
 */
function filemaker_is_error($fx_or_error) {
  
  if ($fx_or_error instanceof FX_Error) {
    $message = $fx_or_error->message;
    watchdog('filemaker', $message, array(), WATCHDOG_ERROR);
    drupal_set_message($message, 'error');
    return true;
  }

  return false;
}
