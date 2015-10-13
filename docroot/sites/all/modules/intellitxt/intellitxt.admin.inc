<?php

/**
 * @file
 * Provides form processing code for module settings
 */

/**
 * Builds the admin form for Intellitxt settings
 * @ingroup forms
 */
function intellitxt_admin_settings_form($form, &$form_state) {

  $form['description'] = array(
    '#markup' => '<div>' . t('Please enter the variables from the Javascript snippet supplied by Vibrant') . '</div>',
  );

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;

  $form['intellitxt_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable Intellitxt',
    '#default_value' => variable_get('intellitxt_configured', TRUE),
  );

  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom DNS'),
    '#default_value' => variable_get('intellitxt_domain', ''),
    '#required' => TRUE,
    '#description' => t('Domain string supplied from Vibrant. eg: mydrupalsite.uk.intellitxt.com'),
    '#maxlength' => 80,
  );

  // Multiple codes for each tag
  $form['tag_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tag settings'),
    '#prefix' => '<div id="tags-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  // Build the fieldset with the proper number of tags. We'll use
  // $form_state['num_tags'] to determine the number of textfields to build.
  $saved_tags = variable_get('intellitxt_tags', array());
  if (empty($form_state['num_tags'])) {
    // init
    $form_state['num_tags'] = (!empty($saved_tags)) ? count($saved_tags) : 1;
  }

  // Build the current number of text fields
  for ($i = 0; $i < $form_state['num_tags']; $i++) {
    $default_tag = (empty($saved_tags[$i])) ? '' : key($saved_tags[$i]);
    $form['tag_settings']['tags']['tag_' . $i]['machine_name'] = array(
      '#prefix' => '<div class="tags-tag"><div class="machine_name">',
      '#suffix' => '</div>',
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#description' => t('Enter a name for this tag'),
      '#default_value' => $default_tag,
      '#machine_name' => array(
        'exists' => 'intellitxt_admin_machine_name_exists',
      ),
      '#required' => FALSE,
    );

    $default_ipid = (empty($saved_tags[$i])) ? '' : current($saved_tags[$i]);
    $form['tag_settings']['tags']['tag_' . $i]['ipid'] = array(
      '#type' => 'textfield',
      '#title' => t('IPID'),
      '#prefix' => '<div class="ipid">',
      '#suffix' => '</div></div>',
      '#default_value' => $default_ipid,
    );
  }

  // Add more button
  $form['tag_settings']['add_tag'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('intellitxt_admin_settings_form_add_one'),
    '#ajax' => array(
      'callback' => 'intellitxt_admin_settings_form_callback',
      'wrapper' => 'tags-fieldset-wrapper',
    ),
  );

  if ($form_state['num_tags'] > 1) {
    // Remove button
    $form['tag_settings']['remove_tag'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('intellitxt_admin_settings_form_remove_one'),
      '#ajax' => array(
        'callback' => 'intellitxt_admin_settings_form_callback',
        'wrapper' => 'tags-fieldset-wrapper',
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'intellitxt') . '/intellitxt.css';

  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * This simply selects and returns the fieldset with the tags in it.
 */
function intellitxt_admin_settings_form_callback($form, $form_state) {
  return $form['tag_settings'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * It just increments the max counter and causes a rebuild.
 */
function intellitxt_admin_settings_form_add_one($form, &$form_state) {
  $form_state['num_tags']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function intellitxt_admin_settings_form_remove_one($form, &$form_state) {
  if ($form_state['num_tags'] > 1) {
    $form_state['num_tags']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Validates the admin form for Intellitxt settings
 */
function intellitxt_admin_settings_form_validate($form, &$form_state) {
  if (preg_match('/\s/', $form_state['values']['domain'])) {
    form_set_error('domain', 'Custom DNS must not contain any whitespace');
  }
  
  if ($form_state['values']['intellitxt_enabled']) {
    // Check we have at least one machine name and ipid pair
    $tags_state = $form_state['values']['tag_settings']['tags'];
    for ($i = 0; $i < count($tags_state); $i++) {
      $machine_name = $tags_state['tag_' . $i]['machine_name'];
      $ipid = $tags_state['tag_' . $i]['ipid'];
    
      // Check for missing values
      if ($machine_name && !$ipid) {
        form_set_error('tags][tag_' . $i, 'Tag is missing IPID code');
      } 
      elseif (!$machine_name && $ipid) {
        form_set_error('tags][tag_' . $i, 'Please add a machine name for IPID code');
      }
    
      // Validate values
      if (preg_match('/\s/', $machine_name)) {
        form_set_error('tag_settings][tags][tag_' . $i . '][machine_name', 'Machine name must not contain any whitespace');
      }
    
      if (preg_match('/\s/', $ipid) || ($ipid && !ctype_digit($ipid))) {
        form_set_error('tag_settings][tags][tag_' . $i . '][ipid', 'IPID must only contain numbers and no whitespace');
      }
    }
  }
}

/**
 * Saves Intellitxt settings into variables
 */
function intellitxt_admin_settings_form_submit($form, &$form_state) {
  $intellitxt_enabled = ($form_state['values']['intellitxt_enabled']) ? TRUE : FALSE;
  variable_set('intellitxt_configured', $intellitxt_enabled);
  variable_set('intellitxt_domain', $form_state['values']['domain']);
  $tags = array();
  $tags_state = $form_state['values']['tag_settings']['tags'];

  for ($i=0; $i < count($tags_state); $i++) {
    $tag_machine_name = trim($tags_state['tag_' . $i]['machine_name'], '/');
    $tag_ipid = $tags_state['tag_' . $i]['ipid'];
    if (!empty($tag_machine_name) && !empty($tag_ipid)) {
      $tags[] = array($tag_machine_name => $tag_ipid);
    }
  }

  if (!empty($tags)) {
    variable_set('intellitxt_tags', $tags);
  }
  else {
    variable_del('intellitxt_tags');
  }
  drupal_set_message(t('Configuration Saved'));
}

/**
 * Callback for machine name field to check whether it already exists
 */
function intellitxt_admin_machine_name_exists($machine_name) {
  return _intellitxt_get_tag_by_name($machine_name);
}