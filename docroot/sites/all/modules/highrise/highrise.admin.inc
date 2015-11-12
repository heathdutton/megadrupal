<?php

/**
 * @file
 * The mapping system of highrise module.
 */
require_once("includes/Highrise.class.php");

/**
 * @file
 * Highrise admin file for module.
 */

/*
 * Creates Admin settings form.
 */
function highrise_admin_settings() {
  $form['highrise_account'] = array(
      '#type' => 'textfield',
      '#title' => t('Highrise account name'),
      '#default_value' =>  variable_get('highrise_account', ''),
      '#description' => t('The account name of highrise that will post the data.(https://&lt;<b>accountname</b>&gt;.highrisehq.com).'),
  );

  $form['highrise_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Highrise authentication token'),
      '#default_value' =>  variable_get('highrise_token', ''),
      '#description' => t('The authentication token to validate against the accountname.  Note:  you\'ll find on the "My Info" screen in Highrise (click the "Reveal authentication token for feeds/API" link).'),
  );

  $form['highrise_tag'] = array(
      '#type' => 'textfield',
      '#title' => t('Highrise tag'),
      '#default_value' =>  variable_get('highrise_tag', ''),
      '#description' => t('The tag to be associated with the contact in highrise.'),
  );
  $form['highrise_unique_mail'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unique email address'),
      '#description' => t('Allow only unique email data details on highrise'),
      '#default_value' => variable_get('highrise_unique_mail', ''),
      '#required' => FALSE,
  );
  $form['#validate'][] = 'highrise_admin_settings_validate';
  return system_settings_form($form);
}

/*
 * Adds a validate handler to the settings form
 */

function highrise_admin_settings_validate($form, &$form_state) {
  $hr = new Highrise();
  $hr->debug = FALSE;
  $account = $form_state['values']['highrise_account'];
  $token = $form_state['values']['highrise_token'];
  $hr->setAccount($account);
  $hr->setToken($token);
  $my_info = $hr->findMe();
  if (!isset($my_info->id)) {
    drupal_set_message(t('Account details are not valid. Please enter again.'), 'error');
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/*
 * Displays current mappings.
 */
function highrise_configure() {
  $output = '';
  $header = array(
    array('data' => t('Mapper Name'), 'field' => 'd.domain_id'),
    array('data' => t('Webform'), 'field' => 'd.subdomain'),
    array('data' => t('Operations')),
  );
  $rows = highrise_getwebforms();
  if (empty($rows)) {
    $output = "<p>" . t('No Mapping defined yet.') . "</p>";
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE, 'attributes' => array('id' => 'domain-list')));
  return $output;
}

/*
 * Allows to create mappings for a webform.
 */
function highrise_mapping() {
  drupal_add_js(drupal_get_path('module', 'highrise') . '/lib/highrise.admin.js', array('type' => 'file', 'scope' => 'footer'));
  $highrise_fields = highrise_getfields();
  $webform_option = highrise_mapping_form();
  if (empty($webform_option)) {
    $form['demandbase_error'] = array(
    '#type' => 'markup',
    '#markup' => t('Please check your Web forms. There may be no form created or left to be mapped.'),
    );
    return system_settings_form($form);
  }
  $form['highrise_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mapper Name'),
    '#required' => TRUE,
    '#weight' => -1
  );
  $form['webform'] = array(
    '#type' => 'select',
    '#title' => 'Select a Webform',
    '#options' => $webform_option
  );

  foreach ($highrise_fields as $fields) {
    if (in_array($fields['nid'], array(1, 2))) {
      $required = TRUE;
    }
    else {
      $required = FALSE;
    }
    $form[$fields['field_name']] = array(
      '#type' => 'textfield',
      '#required' => $required,
      '#title' => t($fields['field_description']),
      '#autocomplete_path' => 'admin/config/services/highrise/autocomplete'
      );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#default_value' =>  t('Save'),
  );
  $form['#submit'][] = '_highrise_save';
  return $form;

}

/*
 * Displays webform listing to map with,
 * except the one which has been previously mapped.
 */
function highrise_mapping_form() {
  $mappings = array();
  $result = db_query("SELECT n.nid , n.title
            FROM node n
            INNER JOIN {node_revision} r ON n.vid = r.vid
            WHERE n.type = :type
            AND n.nid NOT IN (SELECT nid FROM {highrise_webforms})", array(':type' => 'webform'));
  while ($rows = $result->fetchAssoc()) {
    $mappings[$rows['nid']] = $rows['title'];
  }
  return $mappings;
}

function highrise_getfields() {
  $query = db_select('highrise_fields', 'f')
    ->fields('f');
  $result = $query->execute();
  while ($obj = $result->fetchAssoc()) {
    $items[] = array('nid' => $obj['hid'], 'field_name' => $obj['name'], 'field_description' => $obj['description']);
  }
  return $items;
}

/*
 * Callback function to fetch component id.
 */
function highrise_autocomplete($string = '') {
  if ($string) {
    $query = db_select('webform_component', 'c')
    ->fields('c', array('cid', 'name'))
    ->condition('name', '%' . db_like(arg(6)) . '%', 'LIKE')
    ->condition('nid', $string)
    ->range(0, 10);
    $result = $query->execute();
    while ($obj = $result->fetchAssoc()) {
      $key = $obj['name'] . ' [cid:' . $obj['cid'] . ']';
      $items[$key] = $obj['name'];
    }
    drupal_json_output($items);
  }
}

function highrise_lookup($webformid) {
  $result = db_select('webform_component', 'c')
    ->fields('c', array('cid', 'name'))
    ->condition('nid', $webformid)
    ->execute();
  while ($obj = $result->fetchAssoc()) {
    $items[$obj->name] = check_plain($obj->cid);
  }
  return $items;
}

/*
 * Returns list of current mappings.
 */
function highrise_getwebforms() {
  $rows = array();
  $query = db_select('highrise_webforms', 'w');
  $query->join('node', 'n', 'w.nid = n.nid');
  $query->fields('w', array('nid', 'name', 'mid'));
  $query->fields('n', array('title'));
  $result = $query->execute();
  while ($object = $result->fetchAssoc()) {
    $row = array();
    $row['name'] = $object['name'];
    $row['title'] = $object['title'];
    $edit = l(t('edit'), 'admin/config/services/highrise/edit/' . $object['mid']);
    $row['delete'] = l(t('delete'), 'highrise/delete/' . $object['nid']) . ', ' . $edit;
    $rows[] = $row;
  }
  return $rows;
}

function _highrise_save($form, &$form_state) {
  $webform_id = $form_state['values']['webform']; // Webform ID (Contact Us)
  $mapping_name = $form_state['values']['highrise_name']; // Mapping Name (Machine-name)
  $mid = db_insert('highrise_webforms')
          ->fields(array(
           'nid' => $webform_id,
           'name' => $mapping_name,
          ))
          ->execute();
  $fields = array();
  $fields[] = $form_state['values']['first-name'];
  $fields[] = $form_state['values']['last-name'];
  $fields[] = $form_state['values']['title'];
  $fields[] = $form_state['values']['background'];
  $fields[] = $form_state['values']['email-address'];
  $fields[] = $form_state['values']['company-name'];
  $fields[] = $form_state['values']['phone-number'];

  $cids = highrise_filter_cid($fields);
  $hids_array = array('first-name', 'last-name', 'title', 'background', 'email-address', 'company-name',  'phone-number');
  $hids_selected = array();
  foreach ($fields as $key => $value) {
    if (!empty($value)) {
      $hids_selected[] = $hids_array[$key];
    }
  }

  $hids = highrise_fetch_hid($hids_selected);
  $query = '';
  $query = 'INSERT INTO {highrise_mappings} VALUES ';
  for ($i = 0; $i < sizeof($cids); $i++ ) {
    $query .= '(' . $mid . ',' . $cids[$i] . ',' . $hids[$i] . ')';
    if ($i < sizeof($cids) -1) {
      $query .= ',';
    }
  }
  db_query($query);
  $form_state['redirect']  = 'admin/config/services/highrise/mapping';
  drupal_set_message(t('Mapping has been saved.'));
}

/*
 * Filters the cid value.
 */
function highrise_filter_cid($fields) {
  foreach ($fields as $key)
  if (!empty($key)) {
    $key = drupal_explode_tags($key);
    foreach ($key as $v) {
      $cid = NULL;
      preg_match('/^(?:\s*|(.*) )?\[\s*cid\s*:\s*(\d+)\s*\]$/', $v, $matches);
      if (!empty($matches)) {
        // Explicit [cid:n].
        list(, $title, $cid) = $matches;
      }
      if (!empty($cid)) {
        $cids[] = $cid;
      }
    }
  }
  return $cids;
}

/*
 * Returns list of highrise field ids.
 */
function highrise_fetch_hid($vals) {
  $hids = array();
  foreach ($vals as $key) {
    $result = db_select('highrise_fields', 'f')
    ->fields('f', array('hid'))
    ->condition('name', $key, '=')
    ->execute()
    ->fetchAssoc();
    $hids[] = $result['hid'];
  }
  return $hids;
}

/*
 * Delete webform mapping.
 */
function highrise_mapping_delete($form, &$form_state, $mapp_id) {
  $form['#mapp_id'] = $mapp_id;
  return confirm_form($form, t('Are you sure you want to delete this mapping?'), 'admin/config/services/highrise/mapping', t('This action cannot be undone.'), t('Delete'));
}

function highrise_mapping_delete_submit($form, &$form_state) {
  $form_id = $form['#mapp_id'];
  $result = db_select('highrise_webforms', 'w')
    ->fields('w', array('mid'))
    ->condition('w.nid', $form_id, '=')
    ->execute()
    ->fetchAssoc();
  $mid = $result['mid'];
  if ($mid) {
    $map_deleted = db_delete('highrise_mappings')
                    ->condition('mid', $mid, '=')
                    ->execute();
    $web_deleted = db_delete('highrise_webforms')
                    ->condition('mid', $mid, '=')
                    ->execute();
  }
  $form_state['redirect']  = 'admin/config/services/highrise/mapping';
  drupal_set_message(t('Mapping has been deleted.'));
}

/**
 * List of highrise fields
 * @return type
 */
function highrise_fields() {
  $rows = array();
  $query = db_select('highrise_fields', 'f')
    ->fields('f');
  $result = $query->execute();
  while ($obj = $result->fetchAssoc()) {
    $row = array();
    $row['name'] = $obj['name'];
    $row['description'] = $obj['description'];
    $rows[] = $row;
  }
  $header = array(
    array('data' => t('Highrise field'), 'field' => 'd.domain_id'),
    array('data' => t('Description'), 'field' => 'd.subdomain'),
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE, 'attributes' => array('id' => 'domain-list')));
  return $output;
}

/**
 * Edit the highrise mapping
 * @param type $form
 * @param type $form_state
 * @param type $mid
 * @return string
 */
function highrise_mapping_edit($form, $form_state, $mid) {
  drupal_add_js(drupal_get_path('module', 'highrise') . '/lib/highrise.admin.js', array('type' => 'file', 'scope' => 'footer'));
  $highrise_fields = highrise_getfields();
  $webform_option = highrise_get_webform($mid);
  $current_mappings = highrise_get_cureent_mappings($mid);
  $form['highrise_mid'] = array(
    '#type' => 'hidden',
    '#title' => t('Form id'),
    '#required' => TRUE,
    '#default_value' => $mid,
  );
  $form['highrise_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mapper Name'),
    '#default_value' => highrise_mapping_title($mid),
    '#required' => TRUE,
    '#weight' => -1
  );
  $form['webform'] = array(
    '#type' => 'select',
    '#title' => 'Select a Webform',
    '#options' => $webform_option
  );
  foreach ($highrise_fields as $fields) {
    $default = '';
    if (key_exists($fields['nid'], $current_mappings)) {
      $default = $current_mappings[$fields['nid']]['name'] . ' [cid:' . $current_mappings[$fields['nid']]['cid'] . ']';
    }
    if (in_array($fields['nid'], array(1, 2))) {
      $required = TRUE;
    }
    else {
      $required = FALSE;
    }
    $form[$fields['field_name']] = array(
      '#type' => 'textfield',
      '#required' => $required,
      '#title' => t($fields['field_description']),
      '#autocomplete_path' => 'admin/config/services/highrise/autocomplete',
      '#default_value' => $default,
      );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#default_value' =>  t('Save'),
  );
  $form['#submit'][] = '_highrise_edit_save';
  return $form;

}

/**
 * Get the current Webform details
 * @param type $mid
 * @return type
 */
function highrise_get_webform($mid) {
  $mappings = array();
  $query = db_select('highrise_webforms' , 'hw');
  $query->join('node', 'n', 'hw.nid = n.nid');
  $query->condition('hw.mid', $mid);
  $query->fields('n', array('title', 'nid'));
  $result = $query->execute();
  while ($rows = $result->fetchAssoc()) {
    $mappings[$rows['nid']] = $rows['title'];
  }
  return $mappings;
}

/**
 * Get current mapping details
 * @param type $mid
 */
function highrise_get_cureent_mappings($mid) {
  $mappings = array();
  $query = db_select('highrise_mappings' , 'hm');
  $query->join('highrise_webforms', 'hw', 'hm.mid = hw.mid');
  $query->join('webform_component', 'wc', 'hw.nid = wc.nid and hm.cid=wc.cid');
  $query->condition('hm.mid', $mid);
  $query->fields('wc', array('name'));
  $query->fields('hm', array('hid', 'cid'));
  $result = $query->execute();
  while ($rows = $result->fetchAssoc()) {
    $mappings[$rows['hid']] = $rows;
  }
  return $mappings;
}

/**
 * Get highrise mapping name
 * @param type $mid
 * @return type
 */
function highrise_mapping_title($mid) {
  $name = '';
  $query = db_select('highrise_webforms' , 'hw');
  $query->condition('hw.mid', $mid);
  $query->fields('hw', array('name'));
  $result = $query->execute();
  while ($rows = $result->fetchAssoc()) {
    $name = $rows['name'];
  }
  return $name;
}

/**
 * Save the highrise mapping form changes
 * @param type $form
 * @param type $form_state
 */
function _highrise_edit_save($form, &$form_state) {
  $highrise_fields = highrise_getfields();
  $webform_id = $form_state['values']['webform']; // Webform ID (Contact Us)
  $mapping_name = $form_state['values']['highrise_name']; // Mapping Name (Machine-name)
  $mid = $form_state['values']['highrise_mid'];
  $hids_array = array();
  $fields = array();
  foreach ($highrise_fields as $pos => $field_info) {
    $fields[] = $form_state['values'][$field_info['field_name']];
    $hids_array[] = $field_info['field_name'];
  }
  $cids = highrise_filter_cid($fields);
  $hids_selected = array();
  foreach ($fields as $key => $value) {
    if (!empty($value)) {
      $hids_selected[] = $hids_array[$key];
    }
  }
  $hids = highrise_fetch_hid($hids_selected);
  $query = '';
  $query = 'INSERT INTO {highrise_mappings} VALUES ';
  for ($i = 0; $i < sizeof($cids); $i++ ) {
    $query .= '(' . $mid . ',' . $cids[$i] . ',' . $hids[$i] . ')';
    if ($i < sizeof($cids) -1) {
      $query .= ',';
    }
  }
  db_query("UPDATE {highrise_webforms} set name = :name where nid = :nid",
    array(':name' => $mapping_name, ':nid' => $webform_id));
  db_query("DELETE FROM {highrise_mappings} where mid = :mid", array(':mid' => $mid));
  db_query($query);
  $form_state['redirect']  = 'admin/config/services/highrise/mapping';
  drupal_set_message(t('Mapping changes for "@mapping_name" has been saved.', array('@mapping_name' => $mapping_name)));
}

