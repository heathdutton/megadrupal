<?php

/**
 * @file
 * This file contains user forms for opac suggestions module.
 */

/**
 * New suggestion form.
 */
function opac_suggestions_add_form($form, $form_state) {
  global $user;

  if (!$user->uid) {
    drupal_set_message(t('You must be connected to access this page. Please Login'), 'warning');
    drupal_goto('user', array('query' => drupal_get_destination()));
  }

  $branch_vid = variable_get('suggestions_branch_vocabulary', '');
  $doctype_vid = variable_get('suggestions_doctype_vocabulary', '');
  if ($branch_vid) {
    $field = variable_get('suggestions_branch_field', 'name');
    $terms = taxonomy_get_tree($branch_vid, 0, NULL, TRUE);

    // Get term id of the user library.
    $default_user_branch = '';
    $default_branch_tid = '';
    $user_field = variable_get('suggestions_user_branch_field', '');
    if ($user_field) {
      $account = user_load($user->uid);
      $field_value = field_get_items('user', $account, $user_field);
      $default_branch_tid = $field_value[0]['tid'];
    }

    $branch_options = array('' => t('None'));
    foreach ($terms as $term) {
      $code = isset($term->{$field}) ? $term->{$field}['und'][0]['value'] : $term->name;
      $branch_options[$code] = $term->name;
      if ($term->tid == $default_branch_tid) {
        $default_user_branch = $code;
      }
    }

    $form['opac_suggestion_branchcode'] = array(
      '#type' => 'select',
      '#title' => t('Suggestion is for'),
      '#options' => $branch_options,
      '#default_value' => $default_user_branch,
      '#description' => t('Select the library this suggestion is for.'),
      '#required' => TRUE,
    );
  }

  $form['opac_suggestion_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Type here the document title.'),
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['opac_suggestion_author'] = array(
    '#type' => 'textfield',
    '#title' => t('Author'),
    '#size' => 40,
  );
  $form['opac_suggestion_publicationyear'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication year'),
    '#size' => 10,
  );
  $form['opac_suggestion_isbn'] = array(
    '#type' => 'textfield',
    '#title' => t('Standard number (ISBN, ISSN ...)'),
    '#size' => 20,
  );
  $form['opac_suggestion_publisher'] = array(
    '#type' => 'textfield',
    '#title' => t('Publisher'),
    '#size' => 20,
  );
  $form['opac_suggestion_collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection'),
    '#size' => 20,
  );
  $form['opac_suggestion_publisherplace'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication place'),
    '#size' => 20,
  );
  if ($doctype_vid) {
    $field = variable_get('suggestions_doctype_field', 'name');
    $terms = taxonomy_get_tree($doctype_vid, 0, NULL, TRUE);

    $doctype_options = array('' => t('None'));
    foreach ($terms as $term) {
      $code = isset($term->{$field}) ? $term->{$field}['und'][0]['value'] : $term->name;
      $doctype_options[$code] = $term->name;
    }

    $form['opac_suggestion_doctype'] = array(
      '#type' => 'select',
      '#title' => t('Document type'),
      '#options' => $doctype_options,
      '#description' => t('Select here the document type.'),
    );
  }
  $form['opac_suggestion_note'] = array(
    '#title' => t('Notes'),
    '#type' => 'textarea',
  );

  $form['opac_suggestion_creator'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitted by'),
    '#default_value' => $user->name,
    '#disabled' => TRUE,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Create'));

  return $form;
}

/**
 * Submit callback for opac_suggestions_add_form.
 */
function opac_suggestions_add_form_submit($form, $form_state) {
  module_load_include('inc', 'opac_users', "includes/opac_users.user");
  global $user;
  $account = user_load($user->uid);
  $serverid = opac_user_get_server($account);

  // Create an instance of the connector.
  $server = opac_server_load($serverid);
  $connector = opac_get_instance($server);

  // Call itemsStatus method and return data.
  if (method_exists($connector, 'createSuggestion')) {
    $result = $connector->createSuggestion($form_state['values']);
    if (isset($result['suggestionid']) && $result['suggestionid']) {
      drupal_set_message(t('Your suggestion has been submitted to the library'));
      $form_state['values']['suggestionid'] = $result['suggestionid'];

      // Notify library.
      if (variable_get('opac_suggestions_send_email', FALSE)) {
        $email = _opac_suggestion_get_email($form_state['values']['opac_suggestion_branchcode']);
        _opac_suggestions_mail_notify($account, $form_state['values'], $email, 'opac_suggestions_notify');
      }

      // Notify creator.
      if (variable_get('opac_suggestions_send_user_email', FALSE) && $user->mail) {
        _opac_suggestions_mail_notify($account, $form_state['values'], $account->mail, 'opac_suggestions_user_notify');
      }
    }
    else {
      drupal_set_message(t('An error occurred while transmitting the suggestion: @error', array('@error' => $result['error'])));
    }
  }
}

/**
 * Page callback for user/suggestions and user/%user/suggestions.
 */
function opac_suggestions_form($form, $form_state, $account = NULL) {
  module_load_include('inc', 'opac_users', 'includes/opac_users.user');
  if (!isset($account)) {
    global $user;
    $account = user_load($user->uid);
  }
  $server_id = opac_user_get_server($account);
  $server = opac_server_load($server_id);
  $connector = opac_get_instance($server);

  if (!$connector->has('suggestions')) {
    $identity = $connector->identity();
    $message = t("@name has no support for suggestions.",
      array('@name' => $identity['name']));
    drupal_set_message($message, 'error');
    return $form;
  }

  $login = opac_user_get_login($account);
  $opac_user_id = $connector->lookupUser($login);

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $account->uid,
  );

  $suggestions = $connector->getSuggestions($opac_user_id);

  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'title'),
    'author' => array('data' => t('Author'), 'field' => 'author'),
    'publication_year' => array('data' => t('Publication year'), 'field' => 'publication_year'),
    'library' => array('data' => t('Library'), 'field' => 'library'),
    'date' => array('data' => t('Date'), 'field' => 'date', 'sort' => 'desc'),
    'notes' => array('data' => t('Notes'), 'field' => 'notes'),
    'status' => array('data' => t('Status'), 'field' => 'status'),
  );

  // Sort suggestoins array.
  $order = tablesort_get_order($header);
  $sort = tablesort_get_sort($header);
  $suggestions = _opac_suggestions_sort_suggestions($suggestions, $order['sql'], $sort);

  # Get Branch configuration (vid, code field).
  $vid = variable_get('suggestions_branch_vocabulary', '');
  $field_code = variable_get('suggestions_branch_field', '');

  $rows = array();
  foreach ($suggestions as $suggestion) {
    // Retrieves branchname.
    if ($vid && $field_code) {
      module_load_include('inc', 'opac', "includes/helper.taxonomy");
      $tid = _opac_taxonomy_get_term($vid, $field_code, $suggestion['library']);
      $term = taxonomy_term_load($tid);
      $branchname = is_object($term) && isset($term->name) ? $term->name : $suggestion['library'];
    }

    $row = array();
    $row['title'] = $suggestion['title'];
    $row['author'] = $suggestion['author'];
    $row['library'] = $branchname;
    $row['publication_year'] = $suggestion['publication_year'] ? $suggestion['publication_year'] : '';
    $row['date'] = $suggestion['date'];
    $row['notes'] = $suggestion['notes'];
    $row['status'] = t($suggestion['status']);
    if ($suggestion['reason']) {
      $row['status'] .= ' ' . t("(reason: @reason)", array('@reason' => $suggestion['reason']));
    }

    $rows[$suggestion['suggestionid']] = $row;
  }

  $form['suggestions'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t("No purchase suggestions found."),
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel selected suggestions'),
    '#name' => 'delete',
  );

  return $form;
}

/**
 * Submit callback for opac_suggestions_form.
 */
function opac_suggestions_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $account = user_load($values['uid']);
  $server_id = opac_user_get_server($account);
  $server = opac_server_load($server_id);
  $connector = opac_get_instance($server);

  $selected = array_values(array_filter($values['suggestions']));
  foreach ($selected as $suggestionid) {
    if (!$connector->deleteSuggestion($suggestionid)) {
      $message = t("Failed to delete suggestion @suggestionid", array('@suggestionid' => $suggestionid));
      drupal_set_message($message, 'error');
    }
  }
}
