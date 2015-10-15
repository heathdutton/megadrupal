<?php
/**
 * @file
 * Integration configurations.
 */

/**
 * Form callback for module configuration.
 */
function semi_anonymous_settings_form($form_state) {
  // Init.
  $form = array();
  $m = 'semi_anonymous_';
  // Get available meta data.
  $meta_data = _semi_anonymous_property_list();

  $form['#prefix'] = t('Configuration options for using the !groucho library to leverage the !datalayer and create tracking records.',
    array(
      '!groucho' => l(t('Groucho'), 'https://github.com/tableau-mkt/groucho', array('external' => TRUE)),
      '!datalayer' => l(t('Data Layer'), 'https://www.drupal.org/project/datalayer', array('external' => TRUE)),
    )
  );

  $form[$m . 'track_browsing_extent'] = array(
    '#type' => 'select',
    '#states' => array(
      'enbled' => array(
        ':input[name="' . $m . 'include"]' => array('checked' => TRUE),
        ':input[name="' . $m . 'track_page"]' => array('checked' => TRUE),
      ),
    ),
    '#title' => t('History size'),
    '#options' => array(
      '10' => t('10 items'),
      '25' => t('25 items'),
      '50' => t('50 items'),
      '150' => t('150 items'),
      '500' => t('500 items'),
      '2500' => t('2500 items'),
      '0' => t('All'),
    ),
    '#default_value' => variable_get($m . 'track_browsing_extent', 25),
  );
  /*
  // @todo: Save and restore user data for past authenticated user sessions.
  $form[$m . 'user_save'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save/restore user data'),
    '#default_value' => variable_get($m . 'user_save', FALSE),
    '#description' => t('Stash data to DB. Restored/merged on login.'),
  );
  */

  $form[$m . 'track_meta'] = array(
    '#type' => 'checkboxes',
    '#states' => array(
      'enabled' => array(
        ':input[name="' . $m . 'add_page_meta"]' => array('checked' => TRUE),
      ),
    ),
    '#title' => 'Meta data to stash',
    '#default_value' => variable_get($m . 'track_meta', array()),
    '#options' => drupal_map_assoc($meta_data),
    '#description' => t('Meta data properties to include in record data. Marking none will output everything available.'),
  );

  $form[$m . 'track_term_hits'] = array(
    '#type' => 'checkbox',
    '#states' => array(
      'enabled' => array(
        ':input[name="' . $m . 'output_terms"]' => array('checked' => TRUE),
      ),
    ),
    '#title' => t('Include taxonomy terms'),
    '#default_value' => variable_get($m . 'track_term_hits', TRUE),
    '#description' => t('Adds taxonomy terms to list of tracked data. Allows anonymous favorites!'),
  );

  $form[$m . 'threshold'] = array(
    '#title' => t('Global threshold'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get($m . 'threshold', 2),
    '#description' => t('Count value a term must have before considering it a favorite.'),
  );
  $form[$m . 'record_form_submits'] = array(
    '#title' => t('Record form submits'),
    '#type' => 'checkbox',
    '#default_value' => variable_get($m . 'record_form_submits', 0),
    '#description' => t('Record all form submits as Groucho activity.'),
  );

  return system_settings_form($form);
}
