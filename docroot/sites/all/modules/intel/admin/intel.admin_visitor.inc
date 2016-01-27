<?php
/**
 * @file
 * Administration of visitors
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

function intel_visitor_admin_settings_form($form, $form_state) {
  $form['intel_visitor_default_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default visitor picture'),
    '#default_value' => variable_get('intel_visitor_default_image_path', ''),
    '#description' => t('URL of picture to display for visitors with no custom picture. Leave blank for none.'),
  );

  $form['intel_sync_visitordata_fullcontact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync Full Contact data'),
    '#default_value' => variable_get('intel_sync_visitordata_fullcontact', 0),
    '#description' => t('Check to enable syncing data via FullContact API. Only available for pro accounts.'),
  );
  return system_settings_form($form);
}

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function intel_visitor_add() {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $entity = entity_get_controller('intel_visitor')->create();
  return drupal_get_form('intel_visitor_edit_form', $entity);
}

/**
 * Form function to create an intel_contact entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function intel_visitor_edit_form($form, &$form_state, $entity) {
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  $account = FALSE;
  if (isset($entity->uid)) {
    $account = user_load($entity->uid);
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => $entity->name,
  );
  
  $form['user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#required' => FALSE,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => isset($account->name) ? $account->name: '',
  );
    
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    //'#required' => TRUE,
    '#default_value' => $entity->email,
  );

  field_attach_form('intel_visitor', $entity, $form, $form_state);
  $form['intel_sync_visitordata_fullcontact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync Full Contact data'),
    '#default_value' => $entity->getVar('data', 'settings', 'sync_visitordata.fullcontact', INTEL_SYNC_VISITORDATA_FULLCONTACT_DEFAULT),
    '#description' => t('Check to enable syncing data via FullContact API. Only available for pro accounts.'),
  );

  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('intel_visitor_delete_submit'),
  );

  return $form;
}


/**
 * Validation handler for intel_contact_add_form form.
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */
function intel_visitor_edit_form_validate($form, &$form_state) {
  field_attach_form_validate('intel_visitor', $form_state['values']['entity'], $form, $form_state);
}


/**
 * Form submit handler: submits basic_add_form information
 */
function intel_visitor_edit_form_submit($form, &$form_state) {
  $entity = $form_state['values']['entity'];

  $entity->setName($form_state['values']['name']);
  $account = user_load_by_name($form_state['values']['user_name']);
  $email = '';
  if (!empty($account->uid)) {
    $entity->setIdentifier('uid', $account->uid);
    $email = $account->mail;
  }

  if ($form_state['values']['email']) {
    $email = $form_state['values']['email'];
  }
  if ($email) {
    $entity->setIdentifier('email', $email);
  }

  if (intel_api_access() && isset($form_state['values']['intel_sync_visitordata_fullcontact']))  {
    $entity->setVar('data', 'settings', 'sync_visitordata.fullcontact', !empty($form_state['values']['intel_sync_visitordata_fullcontact']));
  }

  field_attach_submit('intel_visitor', $entity, $form, $form_state);
  $entity = intel_visitor_save($entity);
  $form_state['redirect'] = $entity->uri();
}

/**
 * Button submit function: handle the 'Delete' button on the node form.
 */
function intel_visitor_delete_submit($form, &$form_state) {
  $entity = $form_state['values']['entity'];
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array($entity->uri() . '/delete', array('query' => $destination));
}

function intel_visitor_delete_confirm_form($form, &$form_state, $entity) {
  drupal_set_title(t('Are you sure you want to delete visitor @title?', array('@title' => $entity->label())));
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'intel_visitor_delete_confirm_form_submit';
  $confirm_question = t('Are you sure you want to delete @name?', array('@name' => $entity->label()));
  return confirm_form($form,
    $confirm_question,
    'admin/content',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Form deletion handler.
 *
 */
function intel_visitor_delete_confirm_form_submit($form , &$form_state ) {
  $entity = $form_state['values']['entity'];
  intel_visitor_delete($entity);
  drupal_set_message(t('The visitor %name (ID %id) has been deleted',
      array('%name' => $entity->label(), '%id' => $entity->vid))
  );
  $form_state['redirect'] = 'admin/people/contacts';
}

function intel_visitor_tab_scorecard($id) {
  if (!intel_api_access()) {
    $output = intel_api_access_error($vars = array());
    return $output;
  }
  require_once drupal_get_path('module', 'intel') . "/reports/intel.report_scorecard.inc";  
 
  if (is_string($id) && is_numeric($id) && strlen($id) < 8) {
    $id = (int)$id;
  }
  $visitor = intel_visitor_load($id);
  
  if ($visitor->getVar('ext', 'ga') && !$visitor->getVar('data', 'environment')) {
    require_once drupal_get_path('module', 'intel') . "/includes/intel.visitor_sync.inc";
    require_once drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";
    intel_ga_sync_visitordata($visitor);
    drupal_set_message(t('Google Analytics data resynced.'));
  }
  
  drupal_set_title(t('Visitor scorecard: @title', array('@title' => $visitor->getLabel())), PASS_THROUGH);
 
  $filters = array(
   'visitor' => 'visitorid:' . $visitor->vtkid,
  );
 
  $output = '';
  $output .= intel_scorecard_report($filters, 'visitor'); 
 
  return $output;
}


function intel_visitor_tab_clickstream($visitor) {
  if (!intel_api_access()) {
    $output = intel_api_access_error($vars = array());
    return $output;
  }
  
  // TODO re-org clickstream report
  $file = drupal_get_path('module', 'intel') . "/reports/intel.report_visitor_clickstream.inc";
  if (file_exists($file)) {
    require_once $file;
  }
  else {
    drupal_set_message(t('Clickstream report not found. Comming soon.'));
    return '';
  }

  if ($visitor->getVar('ext', 'ga') && !$visitor->getVar('data', 'environment')) {
    require_once drupal_get_path('module', 'intel') . "/includes/intel.visitor_sync.inc";
    require_once drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";
    intel_ga_sync_visitordata($visitor);
    drupal_set_message(t('Google Analytics data resynced.'));
  }
  
  drupal_set_title(t('Clickstream: @title', array('@title' => $visitor->label())), PASS_THROUGH);
 
  $output = '';
  $output .= intel_visitor_clickstream($visitor);
  
  return $output;
}

function intel_visitor_tab_submissions($visitorid) {
  require_once './' . drupal_get_path('module', 'intel') . "/admin/intel.admin_submission.inc";

  $visitor = intel_visitor_load_by_visitorid($visitorid, array());  
  
  $visitor_name = (!empty($visitor->name)) ? $visitor->name : $visitorid;
  drupal_set_title(t('Clickstream: @title', array('@type' => $visitor_name, '@title' => $visitor_name)), PASS_THROUGH);
  
  $output = '';
  $output .= intel_visitor_clickstream($visitorid);
  
  return $output;  
}

function intel_admin_people_contacts() {
  $output = array();

  $api_level = variable_get('intel_api_level', '');
  
  $header = array();
  $header[] = array(
    'data' => t('Name'),
    'type' => 'property',
    'specifier' => 'name',
  );
  $header[] = array(
    'data' => t('Email'),
  );
  if ($api_level == 'pro') {
    $header[] = array(
      'data' => t('Location'),
    );
  }
  $header[] = array(
    'data' => t('Contact created'),
    'type' => 'property',
    'specifier' => 'contact_created',
  );
  $header[] = array(
    'data' => t('Last activity'),
    'type' => 'property',
    'specifier' => 'last_activity',
    'sort' => 'desc',
  );
  $header[] = array(
    'data' => t('Operations'),
  );

  // load contacts using EntityFieldQuery
  // see: https://drupal.org/node/916776
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'intel_visitor');
  $query->propertyCondition('contact_created', 0, '>');
  $query->pager(50);
  $query->tableSort($header);

  $result = $query->execute();

  $visitors = array();
  if (isset($result['intel_visitor'])) {
    $visitors = intel_visitor_load_multiple(array_keys($result['intel_visitor']));
  }

  $rows = array();
  foreach ($visitors AS $visitor) {
    $row = array();
    $row[] = $visitor->label_link();
    $row[] = $visitor->email;
    if ($api_level == 'pro') {
      $row[] = $visitor->location();
    }
    $row[] = ((REQUEST_TIME - $visitor->contact_created) > 172800) ? format_date($visitor->contact_created, 'short') : format_interval(REQUEST_TIME  - $visitor->contact_created) . ' ' . t('ago');
    $row[] = ((REQUEST_TIME  - $visitor->last_activity) > 172800) ? format_date($visitor->last_activity, 'short') : format_interval(REQUEST_TIME  - $visitor->last_activity) . ' ' . t('ago');
    $ops = '';
    $ops .= l(t('Clickstream'), $visitor->uri() . '/clickstream');
    $hubspot_profile_url = $visitor->getVar('ext', 'hubspot', 'profile_url');
    if (!empty($hubspot_profile_url)) {
      $ops .= ' | ' . l(t('HubSpot'), $visitor->uri() . '/hubspot');
    }
    $row[] = $ops;
    $rows[] = $row;
  }

  $output['contacts'] = array(
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No contacts were returned'),
    '#theme' => 'table',
  );
  if (!empty($result['intel_visitor'])) {
    $output['pager']['#markup'] = theme('pager', $query->pager);
  }
  return $output;
}



function intel_admin_people_visitors() {
  $output = array();

  $api_level = variable_get('intel_api_level', '');

  $header = array(
    array(
      'data' => t('Visitor id'),
      'type' => 'property',
      'specifier' => 'vid',
    ),
    array(
      'data' => t('Location'),
    ),
    array(
      'data' => t('Last updated'),
      'type' => 'property',
      'specifier' => 'updated',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Operations'),
    ),
  );


  // load contacts using EntityFieldQuery
  // see: https://drupal.org/node/916776
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'intel_visitor');
  $query->pager(50);
  $query->tableSort($header);

  $result = $query->execute();

  $visitors = intel_visitor_load_multiple(array_keys($result['intel_visitor']));

  $rows = array();
  foreach ($visitors AS $visitor) {
    $row = array();
    $row[] = $visitor->label_link();
    if ($api_level == 'pro') {
      $row[] = $visitor->location();
    }
    //$row[] = ((REQUEST_TIME  - $visitor->last_activity) > 172800) ? format_date($visitor->last_activity, 'short') : format_interval(REQUEST_TIME  - $visitor->last_activity) . ' ' . t('ago');
    $row[] = ((REQUEST_TIME  - $visitor->updated) > 172800) ? format_date($visitor->updated, 'short') : format_interval(REQUEST_TIME  - $visitor->updated) . ' ' . t('ago');
    $ops = '';
    $ops .= l(t('Clickstream'), $visitor->uri() . '/clickstream');
    $row[] = $ops;
    $rows[] = $row;
  }
  $output = array();
  $output['contacts'] = array(
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No contacts were returned'),
    '#theme' => 'table',
  );
  if (!empty($result['intel_visitor'])) {
    $output['pager']['#markup'] = theme('pager', $query->pager);
  }
  return $output;
}
