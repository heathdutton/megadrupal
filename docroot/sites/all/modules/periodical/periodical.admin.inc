<?php

/**
 * @file
 * Administrative interface of periodical module
 */

/**
 * Display an overview page
 */
function periodical_overview_page() {
  $enabled_vocabularies = periodical_get_enabled_vocabularies();

  if (count($enabled_vocabularies) == 0) {
    $page['#markup'] = t('No vocabularies have been enabled for periodical issues. Please create a vocabulary and enable it in the settings tab.');
  }
  else {
    $page['#markup'] = t('Consider installing the views bulk operations module.');
  }

  return $page;
}

function periodical_form_generate_dispatch($form, &$form_state) {
  $enabled_vocabularies = periodical_get_enabled_vocabularies();

  if (count($enabled_vocabularies) == 0) {
    drupal_goto('admin/structure/periodical/settings');
  }
  else if (count($enabled_vocabularies) == 1) {
    drupal_goto('admin/structure/periodical/generate/' . $enabled_vocabularies[0]->machine_name);
  }
  else {
    $options = array();
    foreach ($enabled_vocabularies as $vocabulary) {
      $options[$vocabulary->machine_name] = $vocabulary->name;
    }

    $form['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#options' => $options,
      '#description' => t('Select the vocabulary in which you want to generate issues in.'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    );
  }
  return $form;
}

function periodical_form_generate_dispatch_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/periodical/generate/' . $form_state['values']['vocabulary'];
}

function periodical_generate_terms_title($vocabulary) {
  return t('Generate @name', array('@name' => $vocabulary->name));
}

/**
 * Display a form with settings for generating new issue terms for the selected
 * vocabulary.
 */
function periodical_form_generate_terms($form, &$form_state, $vocabulary) {

  // Load issue date field and instance structure.
  $field = field_read_field('periodical_issue_date',
    array('include_inactive' => TRUE));

  $bundle = $vocabulary->machine_name;
  $instance = field_read_instance('taxonomy_term',
    'periodical_issue_date', $bundle, array('include_inactive' => TRUE));

  // Tweak field and instance settings
  $field['settings']['repeat'] = 1;
  $instance['label'] = t('First Issue Date');
  $instance['required'] = TRUE;
  $instance['description'] = '';
  $instance['settings']['repeat_collapsed'] = FALSE;
  $instance['widget']['settings']['display_all_day'] = TRUE;
  $instance['widget']['settings']['repeat_collapsed'] = FALSE;

  // Remember field and instance for this form
  $form_state['field'] = $field;
  $form_state['instance'] = $instance;
  $form_state['vocabulary'] = $vocabulary;

  // Load default settings
  $defaults = array(
    'issue_num' => 1,
    'issue_date_rule' => NULL,
    'term_name' => t('Issue No. [periodical_issue:number] - [periodical_issue:date]'),
    'term_description' => '',
    'term_format' => NULL,
  );
  $defaults = variable_get('periodical_settings_' . $vocabulary->machine_name, $defaults);

  $form['issue_date_rule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Issue generation rule'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#description' => t('Settings for generating issue dates'),
    // Stick to an empty 'parents' on this form in order not to breaks widgets
    // that do not use field_widget_[field|instance]() and still access
    // $form_state['field'] directly.
    '#parents' => array(),
    '#weight' => 0,
  );

  // see field_ui_default_value_widget
  $form['issue_date_rule'] += field_default_form('taxonomy_term', NULL, $field, $instance,
      LANGUAGE_NONE, $defaults['issue_date_rule'], $form['issue_date_rule'],
      $form_state, 0);

  $form['issue_date_rule']['periodical_issue_num'] = array(
    '#type' => 'textfield',
    '#title' => t('First Issue Number'),
    '#default_value' => $defaults['issue_num'],
    '#maxlength' => 10,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer'),
    '#required' => TRUE,
    '#weight' => 0,
  );

  $form['issue_template'] = array(
    '#type' => 'fieldset',
    '#title' => t('Term template'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#description' => t('Default values for generated terms'),
  );

  // Emulate taxonomy term form
  $form['issue_template']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $defaults['term_name'],
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  $form['issue_template']['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#default_value' => $defaults['term_description'],
    '#format' => $defaults['term_format'],
    '#weight' => 0,
  );

  // Show token help
  $form['issue_template']['token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('periodical_issue'),
    '#weight' => 98,
  );

  // Continue button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
    '#weight' => 99,
  );
  return $form;
}

/**
 * Collect generated dates and prepare generation of terms
 *
 * @see _date_field_widget_form()
 */
function periodical_form_generate_terms_submit($form, &$form_state) {
  $field = $form_state['field'];
  $instance = $form_state['instance'];
  $vocabulary = $form_state['vocabulary'];
  $items = $form_state['values']['periodical_issue_date'][LANGUAGE_NONE];

  // see _date_field_widget_form
  $timezone = date_get_timezone($field['settings']['tz_handling'],
    isset($items[0]['timezone']) ? $items[0]['timezone'] : date_default_timezone());
  if (is_array($timezone)) {
    $timezone = $timezone['timezone'];
  }

  // Validate and construct term prototype
  $start = intval($form_state['values']['periodical_issue_num']);
  $terms = array();

  $values = $form_state['values']['issue_template'];

  $defaults = array(
    'issue_num' => $start,
    'issue_date_rule' => array($items[0]),
    'term_name' => $values['name'],
    'term_description' => $values['description']['value'],
    'term_format' => $values['description']['format'],
  );
  variable_set('periodical_settings_' . $vocabulary->machine_name, $defaults);

  for ($i=0; $i<count($items); $i++) {
    // Create date object
    $date = date_local_date($items[$i], $timezone, $field, $instance);

    // Token substitution parameter
    $token_data = array(
      'periodical_issue' => array(
        'date' => $date->format('U'),
        'number' => $start + $i,
      ),
    );

    // Setup term object
    $term = new stdClass();
    $term->name = token_replace($values['name'], $token_data);
    $term->description =
      token_replace($values['description']['value'], $token_data);
    $term->format = $values['description']['format'];

    $term->tid = NULL;
    $term->vocabulary_machine_name = $vocabulary->machine_name;
    $term->vid = $vocabulary->vid;
    $term->weight = 0;

    $term->periodical_issue_num[LANGUAGE_NONE][0]['value'] = $start + $i;
    $term->periodical_issue_date[LANGUAGE_NONE][0] = $items[$i];

    $status = taxonomy_term_save($term);
    switch ($status) {
    case FALSE:
      drupal_set_message("Failed to insert new taxonomy term");
      break;
    case SAVED_NEW:
      drupal_set_message("Insert new record");
      break;
    }
  }
}


function periodical_form_settings($form, &$form_state) {
  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->machine_name] = $vocabulary->name;
  }

  $selection = array();
  foreach ($vocabularies as $vocabulary) {
    if (_periodical_fields_exist($vocabulary)) {
      $selection[] = $vocabulary->machine_name;
    }
  }
  $form_state['previous_vocabularies'] = $selection;

  $form['enabled_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Vocabulary'),
    '#default_value' => $selection,
    '#options' => $options,
    '#description' => t('Select the vocabularies which should be enabled for periodical issues.'),
  );

  $form['alter_term_order'] = array(
    '#type' => 'radios',
    '#title' => t('Alter term order'),
    '#default_value' => variable_get('periodical_alter_term_order', PERIODICAL_ALTER_TERM_ORDER_DATE_DESC),
    '#options' => array(
      PERIODICAL_ALTER_TERM_ORDER_NO => t('Do not alter term order'),
      PERIODICAL_ALTER_TERM_ORDER_DATE_DESC => t('Issue date newest first'),
      PERIODICAL_ALTER_TERM_ORDER_DATE_ASC => t('Issue date oldest first'),
      PERIODICAL_ALTER_TERM_ORDER_NUM_DESC => t('Issue number highest first'),
      PERIODICAL_ALTER_TERM_ORDER_NUM_ASC => t('Issue number lowest first'),
    ),
    '#description' => t('Order terms by issue date or issue number in most taxonomy reference widgets'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

function periodical_form_settings_submit($form, &$form_state) {
  $old_selection = $form_state['previous_vocabularies'];
  $new_selection = $form_state['values']['enabled_vocabularies'];

  $turn_off = array_filter(array_diff($old_selection, $new_selection));
  foreach($turn_off as $machine_name) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
    _periodical_fields_disable($vocabulary);
    drupal_set_message(t('Periodical issues have been disabled for %vocabulary.', array('%vocabulary' => $vocabulary->name)));
  }

  $turn_on = array_filter(array_diff($new_selection, $old_selection));
  foreach($turn_on as $machine_name) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
    _periodical_fields_create($vocabulary);
    drupal_set_message(t('Periodical issues have been enabled for %vocabulary.', array('%vocabulary' => $vocabulary->name)));
  }

  variable_set('periodical_alter_term_order', $form_state['values']['alter_term_order']);

  menu_cache_clear_all();
}
