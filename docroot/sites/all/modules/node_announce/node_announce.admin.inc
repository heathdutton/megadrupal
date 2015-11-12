<?php

/**
 * the log for the items
 */
function node_announce_view_log($id) {
  _node_announce_load_api();

  $ann = node_announce_load($id);
  $records = node_announce_load_records($id);
  return theme('node_announce_view_log',
    array('records' => $records));
}


/**
 * the page callback for the preview function.
 */
function node_announce_preview($id) {
  _node_announce_load_api();

  $ann = node_announce_load($id);

  $node = node_announce_next_match($ann);
  if (!$node) {
    $node = node_announce_next_match($ann, TRUE);
  }

  return theme('node_announce_preview',
    array('announce' => $ann, 'node' => $node));
}

/**
 * send a node announcement
 */
function node_announce_send($id) {
  _node_announce_load_api();

  $ann = node_announce_load($id);

  $node = node_announce_next_match($ann);
  if (!$node) {
    $node = node_announce_next_match($ann, TRUE);
  }
  node_announce_send_announce($ann, $node);
  drupal_set_message("Sent announcement");

  drupal_goto("admin/config/system/node_announce/$id");
}

/**
 * @file
 * This provides the admin interface for node_announce
 */

function node_announce_form_list($form_state) {
  _node_announce_load_api();

  $form = array();
  $form['introduction'] = array(
    '#markup' => t('Below are a list of active announcements.')
  );

  $announces = node_announce_list();

  $form['content'] = array(
    '#markup' => theme('node_announce_list',
              array('announces' => $announces)
    )
  );

  return $form;
}


function node_announce_form_display_delete($form_state, $aid) {
  $form['id'] = array(
    '#type' => 'value',
    '#default_value' => $aid
  );
  $form['really_delete'] = array(
    '#type' => 'submit',
    '#value' => t('Really Delete?')
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );
  return $form;
}

function node_announce_load_or_empty($aid) {
  if ($aid) {
    $ann = node_announce_load($aid);
  } else {
    // TODO: there must be a better pattern for a single form for add and edit
    $ann = (object)array(
                         "id" => NULL,
                         "name" => NULL,
                         "date_field" => NULL,
                         "email_token" => "",
                         "email_to" => NULL,
                         "email_from" => NULL,
                         "message" => NULL,
                         "subject" => NULL,
                         "days_before" => NULL
                         );
  }
  return $ann;
}

/**
 * Node add announcements
 */
function node_announce_form($form, $form_state) {
  $aid = 0;
  if (array_key_exists('0', $form_state['build_info']['args'])) {
    $aid = $form_state['build_info']['args']['0'];
  }
  _node_announce_load_api();

  if (isset($form_state['values']) &&
    isset($form_state['values']['confirm_delete'])
    && $aid) {
    return node_announce_form_display_delete($form_state, $aid);
  }

  $ann = node_announce_load_or_empty($aid);
  return node_announce_form_display_addedit($ann);
}

function node_announce_form_display_addedit($ann) {
  $form['id'] = array(
    '#type' => 'value',
    '#default_value' => $ann->id
  );

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $ann->name,
    '#description' => t('Name for this announcement')
  );

  $form['date_field'] = array(
    '#title' => t('Node type / Date field'),
    '#type' => 'select',
    '#options' => node_announce_date_field_options(),
    '#default_value' => $ann->date_field,
    '#description' => t('The node type and datefield to trigger the announce on')
  );

  $form['days_before'] = array(
    '#title' => t('Announce Schedule (Days before event)'),
    '#type' => 'textfield',
    '#default_value' => $ann->days_before,
    '#description' => t('Number of Days prior to event to send announce'),
  );

  $form['email_token'] = array(
    '#title' => t('Email Key'),
    '#type' => 'textfield',
    '#default_value' => $ann->email_token,
    '#description' => t('If you are using mimemail, setting an email key will let you use different templates per announce. Leave blank otherwise')
  );

  $form['email_from'] = array(
    '#title' => t('Sender Email'),
    '#type' => 'textfield',
    '#default_value' => $ann->email_from,
    '#description' => t('Email address of sender')
  );

  $form['email_to'] = array(
    '#title' => t('Recipient Email'),
    '#type' => 'textfield',
    '#default_value' => $ann->email_to,
    '#description' => t('Email address of sender')
  );

  $form['subject'] = array(
    '#title' => t('Announcement Subject'),
    '#default_value' => $ann->subject,
    '#maxlength' => 250,
    '#type' => 'textfield',
  );

  $form['message'] = array(
    '#title' => t('Announcement Text'),
    '#default_value' => $ann->message,
    '#type' => 'textarea',
  );

  if (module_exists('token')) {
    $form['view']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      //      '#description' => t('Prefer raw-text replacements for text to avoid problems with HTML entities!'),
    );

    $form['view']['token_help']['help'] = array(
      '#markup' => theme('token_tree', array('token_types' => 'all', 'click_insert' => FALSE, 'show_restricted' => TRUE))
    );
  }

  if (!$ann->id) {
    $form['buttons']['create'] = array(
      '#type' => 'submit',
      '#value' => t('Create')
    );
  } else {
    $form['buttons']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete')
    );
  }
  return $form;
}

function node_announce_form_validate($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == "Save" || $op == "Create") {
    if (!$form_state['values']['name']) {
      form_set_error('name', t('Name can not be blank'));
    }
    if (($form_state['values']['days_before'] == 0) || !preg_match("/^\d+$/", $form_state['values']['days_before'])) {
      form_set_error('days_before', t('Days must be a positive integer'));
    }
    if ($form_state['values']['email_token'] != "" && !preg_match("/^[\w-]+$/", $form_state['values']['email_token'])) {
      form_set_error('email_key', t('Email key can only contain letters, numbers, dashes, and underscores'));
    }

    // TODO: should see if there is a sane way to validate email addresses with tokens
    /**
       if (!valid_email_address($form_state['values']['email_from'])) {
       form_set_error('email_from', t('Email from must be valid email address'));
       }
       if (!valid_email_address($form_state['values']['email_to'])) {
       form_set_error('email_to', t('Email to must be valid email address'));
       }
    */
  }
}

function node_announce_form_submit($form_id, &$form_state) {
  _node_announce_load_api();


  $obj = (object)$form_state['values'];

  switch ($obj->op) {
    case "Create":
      if(node_announce_save($obj)) {
        drupal_set_message(t('Successfully created announcment: @name',
            array('@name' => $obj->name)));
      } else {
        drupal_set_message(t('Failed to create announcment: @name',
            array('@name' => $obj->name)), 'error');
      }
      $form_state['redirect'] = 'admin/config/system/node_announce/list';
      break;

    case "Save":
      if(node_announce_save($obj)) {
        drupal_set_message(t('Successfully saved announcment: @name',
            array('@name' => $obj->name)));
      } else {
        drupal_set_message(t('Failed to saved announcment: @name',
            array('@name' => $obj->name)), 'error');
      }
      $form_state['redirect'] = 'admin/config/system/node_announce/' . $obj->id;
      break;

    case "Delete":
      $form_state['values']['confirm_delete'] = TRUE;
      $form_state['rebuild'] = TRUE;
      drupal_set_message("Are you sure you want to delete this announcement?");
      break;

    case "Really Delete?":
      $ann = node_announce_load($obj->id);
      node_announce_delete($ann->id);
      $form_state['redirect'] = 'admin/config/system/node_announce/list';
      drupal_set_message("Deleted $ann->name");
      break;

    case "Cancel":
      $form_state['redirect'] = 'admin/config/system/node_announce/list';
      drupal_set_message("Delete cancelled");
      break;

  }


}
