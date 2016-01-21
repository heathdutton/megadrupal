<?php

/**
 * @file parse-automated.admin.inc
 * Functions for our automated push rules
 */

/**
 * List of configured rules for sending out post notifications. Doing it this way
 * because I don't want to make "Rules" a dependency.
 */
function parse_automated() {
  // Set up the right breadcrumbs
  $breadcrumbs = array();
  $breadcrumbs[] = l('Home', '<front>');
  $breadcrumbs[] = l('Administration', 'admin');
  $breadcrumbs[] = l('Configuration', 'admin/config');
  $breadcrumbs[] = l('Web Services', 'admin/config/services');
  $breadcrumbs[] = l('Parse', 'admin/config/services/parse');
  $breadcrumbs[] = l('Instant & Automated Rules', 'admin/config/services/parse/automated');
  drupal_set_breadcrumb($breadcrumbs);

  $rows = array();
  $header = array(
    'rule_name' => array('data' => t('Rule Name'), 'style' => 'text-align: center;'),
    'rule_type' => array('data' => t('Rule Type'), 'style' => 'text-align: center;'),
    'is_sent' => array('data' => t('Is Sent'), 'style' => 'text-align: center;'),
    'actions' => array('data' => t('Actions'), 'style' => 'text-align: center;'),
  );
  $result = db_select('parse_automation', 'p')
              ->fields('p')
              ->orderBy('p.rule_name', 'ASC')
              ->execute();
  while ($data = $result->fetchObject()) {
    // Define the display for our rule types.
    switch ($data->rule_type) {
      case 'immediate':
        $rule_type = t('Sent Immediately');
        break;
      case 'scheduled':
        $rule_type = t('Scheduled');
        break;
      default:
        $rule_upe = t('N/A');
        break;
    }
    // Set up our normal language display depending on if the text had been sent or not.
    $is_sent = (!empty($data->is_sent)) ? t('Yes') : t('No');
    
    $row = array();
    $row[] = $data->rule_name;
    $row[] = (array('data' => $rule_type, 'align' => 'center'));
    $row[] = (array('data' => $is_sent, 'align' => 'center'));
    
    if (!empty($data->is_sent)) {
      $row[] = array(
        'data' => l(t('View'), 'admin/config/services/parse/automated/view/' . $data->rid) . ' | ' . 
                  l(t('Resend'), 'admin/config/services/parse/automated/resend/' . $data->rid) . ' | ' .
                  l(t('Delete'), 'admin/config/services/parse/automated/delete/' . $data->rid) . ' | ' .
                  l(t('Duplicate'), 'admin/config/services/parse/automated/duplicate/' . $data->rid),
        'align' => 'center',
      );
    }
    else {
      $row[] = array(
        'data' => l(t('Edit'), 'admin/config/services/parse/automated/edit/' . $data->rid) . ' | ' . 
                  l(t('Delete'), 'admin/config/services/parse/automated/delete/' . $data->rid),
        'align' => 'center',
      );
    }
    $rows[] = $row;
  }

  if (count($rows) == 0) {
    $rows = array(
      array(
        'data' => array(array('align' => 'center', 'colspan' => 4, 'data' => t('THERE ARE CURRENTLY NO AUTOMATED OR INSTANT PUSH NOTIFICATIONS.')))
      ),
    );
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Form for adding or modifying an existing automation rule.
 *
 * @param array $form
 *   Our instantiated $form associative array.
 * @param array &$form_state
 *   Any values associated with this form.
 * @param int $rid
 *   Optional parameter. Passed when modifying an existing rule. The ID of the rule to
 *   be modified.
 * @return array $form
 *   An associative array with all of our configured form fields.
 */
function parse_add_automated_form($form, &$form_state, $rid = 0, $duplicate = FALSE) {
  // If we were passed a rule id then we are editing, so we need to put the rid in a
  // hidden field.
  if (!empty($rid)) {
    // If we are not duplicating, then we are editing, so place the rid in a hidden field
    // so that we know we are duplicating.
    if (empty($duplicate)) {
      $form['rid'] = array(
        '#type' => 'hidden',
        '#value' => $rid,
      );
    }
    
    // Load the form field default values. If, for some reason, there are none, then just
    // initialize the object.
    $result = db_select('parse_automation', 'r')
      ->fields('r')
      ->condition('rid', $rid)
      ->execute();
    if ($result->rowCount() > 0) {
      $rdata = $result->fetchObject();
      if ($duplicate == TRUE) {
        $button_value = t('Duplicate Rule');
        $rdata->rule_name = NULL;
      }
      else {
        $button_value = t('Update Rule');
      }
    }
    else {
      unset($form['rid']);
      $rdata = new stdClass();
      $button_value = t('Create Rule');
    }
  }
  else {
    $rdata = new stdClass();
    $button_value = t('Create Rule');
  }

  $form['rule_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Rule Name'),
    '#default_value' => !empty($rdata->rule_name) ? $rdata->rule_name : NULL,
    '#maxlength' => 240,
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['rule_type'] = array(
    '#type' => 'select',
    '#title' => 'Rule Type',
    '#options' => array(
      'immediate' => t('Send Immediately'),
      'scheduled' => t('Scheduled'),
    ),
    '#default_value' => !empty($rdata->rule_type) ? $rdata->rule_type : 'immediate',
    '#required' => TRUE,
    '#maxlength' => 250,
  );
  $form['geography'] = array(
    '#type' => 'fieldset',
    '#title' => t('Geographical Options'),
  );
  $form['geography']['geo_based'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do you wish to send this push based on a geographical location?'),
    '#default_value' => !empty($rdata->geo_based) ? 1 : 0,
  );
  $form['geography']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#description' => t('The latitude of the location you wish to send the push notification to (center point of radius).'),
    '#default_value' => !empty($rdata->latitude) ? $rdata->latitude : NULL,
    '#maxlength' => 32,
    '#states' => array(
      'visible' => array(
        ':input[name="geo_based"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="geo_based"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['geography']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#description' => t('The longitude of the location you wish to send the push notification to (center point of radius).'),
    '#default_value' => !empty($rdata->longitude) ? $rdata->longitude : NULL,
    '#maxlength' => 32,
    '#states' => array(
      'visible' => array(
        ':input[name="geo_based"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="geo_based"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['geography']['radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius'),
    '#description' => t('The distance out (radius in miles) from the coordinates to send push notification. Must be greater than zero.'),
    '#default_value' => !empty($rdata->radius) ? $rdata->radius : NULL,
    '#maxlength' => 4,
    '#states' => array(
      'visible' => array(
        ':input[name="geo_based"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="geo_based"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['send_date_time'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date & Time'),
    '#description' => t('The date and time the message is to be sent.'),
    '#date_format' => 'Y-m-d H:i',
    '#default_value' => !empty($rdata->send_date_time) ? $rdata->send_date_time : NULL,
    '#states' => array(
      'visible' => array(
        ':input[name="rule_type"]' => array('value' => 'scheduled'),
      ),
      'required' => array(
        ':input[name="rule_type"]' => array('value' => 'scheduled'),
      ),
    ),
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message To Be Sent'),
    '#default_value' => !empty($rdata->message) ? $rdata->message : NULL,
    '#required' => TRUE,
  );
  $form['badge_type'] = array(
    '#type' => 'select',
    '#title' => 'Badge Type',
    '#options' => array(
      'None' => t('None'),
      'Increment' => t('Increment'),
    ),
    '#default_value' => !empty($rdata->badge_type) ? $rdata->badge_type : 'Increment',
    '#required' => TRUE,
    '#maxlength' => 250,
  );
    
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $button_value,
  );
  return $form; 
}

/**
 * Rules form submission validation callback
 *
 * Basically check conditions such as if this is geographic the radius is > 0 and other
 * assorted rules.
 */
function parse_add_automated_form_validate($form, &$form_state) {
  if ($form_state['values']['rule_type'] != 'immediate') {
    if (empty($form_state['values']['send_date_time'])) {
      form_set_error('send_date_time', t('When scheduling a notification, we must specify a date and time to send.'));   
    }  
  }
  
  // Check the geographical fields if we are specifying that we're basing our criteria 
  // on them.
  if ($form_state['values']['geo_based'] == 1) {
    if (empty($form_state['values']['latitude'])) {
      form_set_error('latitude', t('When selecting a geographic rule type, you must proivide the latitude of your target area.'));
    }
    if (empty($form_state['values']['longitude'])) {
      form_set_error('longitude', t('When selecting a geographic rule type, you must proivide the longitude of your target area.'));
    }
    if (empty($form_state['values']['radius'])) {
      form_set_error('radius', t('When selecting a geographic rule type, you must proivide the radius from the latitude and longitude.'));    
    }
  }
}

/**
 * Rules form submission callback
 *
 * Processes the form fields and stores them in our database for future use.
*/
function parse_add_automated_form_submit($form, &$form_state) {  
  // Assign the form values to their respective database columns
  $rule = array(
    'rule_name' => $form_state['values']['rule_name'],
    'rule_type' => $form_state['values']['rule_type'],
    'geo_based' => $form_state['values']['geo_based'],
    'latitude' => $form_state['values']['latitude'],
    'longitude' => $form_state['values']['longitude'],
    'radius' => $form_state['values']['radius'],
    'send_date_time' => $form_state['values']['send_date_time'],
    'message' => $form_state['values']['message'],
    'badge_type' => $form_state['values']['badge_type'],
    'is_sent' => 0,
  );

  // If we are updating, then assign the primary key, otherwise we are assigning a new
  // rule.
  if (!empty($form_state['values']['rid'])) {
    $rule['rid'] = $form_state['values']['rid'];
    drupal_write_record('parse_automation', $rule, array('rid'));
    if ($rule['rule_type'] != 'immediate') {
      drupal_set_message('The configured rule has been successfully updated.');
    }
  }
  else {
    drupal_write_record('parse_automation', $rule);
    drupal_set_message('The new rule has been successfully saved.');
  }

  // If we are triggered for an immediate send, then perform it now.  
  if ($rule['rule_type'] == 'immediate') {
    $is_sent = parse_automated_send_push((object) $rule);

    // Set the rule to having been executed provided it was successful.
    if ($is_sent == TRUE) {
      db_update('parse_automation')
        ->fields(array('is_sent' => 1))
        ->condition('rid', $rule['rid'], '=')
        ->execute();
      drupal_set_message('Push notification for rule "' . $rule['rule_name'] . '" successfully sent.', 'status');
    } 
    else {
      drupal_set_message('The push notification could not be sent. ' . $is_sent['error'], 'error');
    }
  }

  // Redirect back to the main form rules list page/
  drupal_goto('admin/config/services/parse/automated'); 
}

/**
 * Our confirmation of delete form
 *
 * We need to make sure our user knows this will delete the rule and it cannot be undone.
 * No matter how badly they may want to do so.
 *
 * @param int $rid
 *   The rule id of the rule we are confirming for deletion
 * @return array $form
 *   The array of the form to be rendered.
 */
function parse_automated_delete_confirm_form($form, &$form_state, $rid) {
  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid,
  );
  return confirm_form($form, 
                      'Are you sure you wish to delete this rule? There is no going back!!',
                      'admin/config/services/parse/automated',
                      t('You cannot undo this. It will be a permanent mark on your record and the only way to fix it is to re-create the rule from scratch. I JUST want you to know what you are getting yourself into.'),
                      t('Confirm Delete'),
                      t('Don\'t Do It')
                     );
}

/**
 * The callback to our delete form
 *
 * This is where we take the confirmed form and delete the rule.
 */
function parse_automated_delete_confirm_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['rid'])) {
    // Delete the rule and notify the admin when it is completed.
    // Get the id of the rule from the submitted form.
    $rid = $form_state['values']['rid'];
    // Load the rule so we can have the name
    $rule = parse_rule_detail_load($rid);
    if ($rid != FALSE) {
      $rule_name = $rule->rule_name;
      $number_deleted = db_delete('parse_automation')
        ->condition('rid', $rid)
        ->execute();
      drupal_set_message('"' . $rule_name . '" rule has been successfully deleted.');
    }
    else {
      // Set the error message so the admin knows the rule could not be deleted.
      drupal_set_message('No rule could be deleted.', 'error');
    }
  }
  else {
    // Set the error message so the admin knows the rule could not be deleted.
    drupal_set_message('No rule could be deleted.', 'error');
  }
  // Go back to the list of rules. 
  drupal_goto('admin/config/services/parse/automated'); 
}

/**
 * View rule detail.
 *
 * @param int $rid
 *   The rule id that we are displaying the details for
 * @return string $html
 *   The html of the rendered table with the details.
 */
function parse_admin_view_rule_details($rid) {
  $rows = $header = array();
  $result = db_select('parse_automation', 'p')
              ->fields('p')
              ->condition('p.rid', $rid, '=')
              ->execute();
  $data = $result->fetchObject();

  // Define the display for our rule types.
  switch ($data->rule_type) {
    case 'immediate':
      $rule_type = t('Sent Immediately');
      break;
    case 'scheduled':
      $rule_type = t('Scheduled');
      break;
    default:
      $rule_type = t('N/A');
      break;
  }
  // Set up our normal language display depending on if the text had been sent or not.
  $is_sent = (!empty($data->is_sent)) ? t('Yes') : t('No');
  $geo_based = (!empty($data->geo_based)) ? t('Based on Geographic Coordinates') : t('Sent To Everyone');
  
  $row = array();
  $row[] = t('Rule Name');
  $row[] = $data->rule_name;
  $rows[] = $row;

  $row = array();
  $sel[] = t('Rule Type');
  $row[] = $rule_type;
  $rows[] = $row;

  $row = array();
  $row[] = t('Group Sent To');
  $row[] = $geo_based;
  $rows[] = $row;

  if (!empty($data->geo_based)) {
    $row = array();
    $row[] = t('Coordinates & Radius');
    $row[] = "Latitude: " . $data->latitude . "<br />Longitude: " . $data->longitude . "<br />Radius: " . $data->radius . " miles";
    $rows[] = $row;
  }
  
  if ($data->rule_type == 'scheduled') {
    $row = array();
    $row[] = t('Scheduled Send Time');
    $row[] = $data->send_date_time;
    $rows[] = $row;
  }
  
  $row = array();
  $row[] = t('Message');
  $row[] = $data->message;
  $rows[] = $row;

  $row = array();
  $row[] = t('Badge Type');
  $row[] = $data->badge_type;
  $rows[] = $row;

  $row = array();
  $row[] = t('Has Been Sent');
  $row[] = $is_sent;
  $rows[] = $row;
  
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
