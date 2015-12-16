<?php
/**
 * @file
 * Admin settings.
 */

/**
 * Admin page callback.
 */
function user_relationship_locator_admin() {
  $form = array();

  $form['#attributes'] = array(
    'class' => array('user-relationship-locator-form'),
  );

  $rels = user_relationships_types_load();
  $options = array('please create a relationship');

  if (!empty($rels)) {
    $options = array();
    foreach ($rels as $key => $value) {
      $options[$key] = $value->name;
    }
  }

  $form['user_relationship_locator_relationship'] = array(
    '#title' => t('Relationship'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('user_relationship_locator_relationship', '0'),
    '#description' => t('Please select the relationship to use for matches.'),
  );

  $form['user_relationship_locator_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum number of matches to return.'),
    '#description' => t('If the enabled data submodules do not return this number random users will be added to the list until this number is reached.'),
    '#default_value' => variable_get('user_relationship_locator_min', '0'),
    '#maxlength' => 3,
    '#size' => 3,
  );

  $form['user_relationship_locator_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of matches to return.'),
    '#description' => t('The largest number of users that will be displayed regardless of the number of returned matches, reducing this number may increase performance on larger sites.'),
    '#default_value' => variable_get('user_relationship_locator_max', '0'),
    '#maxlength' => 3,
    '#size' => 3,
  );

  $form['#submit'][] = 'user_relationship_locator_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Clear theme registry when form submitted. See #1635514.
 */
function user_relationship_locator_admin_form_submit() {
  drupal_theme_rebuild();
  drupal_set_message(t('Theme settings rebuilt.'));
}
