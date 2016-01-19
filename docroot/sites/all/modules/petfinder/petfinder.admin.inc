<?php
/**
 * @file
 *
 * Contains the administrative callback handlers
 */

/**
 * Implements the callback for the petfinder_admin_form
 */
function petfinder_admin_form($form, &$form_state) {
  $cfg = _petfinder_get_cfg();
  $dev_url = 'http://www.petfinder.com/developers';

  // Prompt text at top of form
  $form['help_prompt'] = array(
    '#markup' => t(
      'Please fill in the following information in order to enable !pf integration.',
      array(
        '!pf' => '<strong>Petfinder.com</strong>',
      )
    ),
    '#prefix' => '<div><p>',
    '#suffix' => '</p></div>',
  );
  $form['help_more_info'] = array(
    '#markup' => t(
      'The API and Secret keys may be obtained by setting up a developer account at !url',
      array(
        '!url' => l($dev_url, $dev_url, array('attributes' => array('target' => '_blank'))),
      )
    ),
    '#prefix' => '<div><p>',
    '#suffix' => '</p></div>',
  );

  // Main module config elements
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Petfinder API key'),
    '#default_value' => (!empty($cfg['api_key'])) ? $cfg['api_key'] : '',
    '#required' => TRUE,
  );
  $form['secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Petfinder API secret'),
    '#default_value' => (!empty($cfg['secret_key'])) ? $cfg['secret_key'] : '',
    '#required' => TRUE,
  );

  // Shelter search page config items
  $form['shelter_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shelter search page settings'),
    '#description' => t('Use these settings to control the behavior of the Petfinder shelter search page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['shelter_search']['show_location'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show search location field on page'),
    '#default_value' => isset($cfg['shelter_search']['show_location']) ? $cfg['shelter_search']['show_location'] : TRUE,
  );
  $form['shelter_search']['default_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Default search location'),
    '#description' => t('Default location to execute search on'),
    '#default_value' => (!empty($cfg['shelter_search']['default_location'])) ? $cfg['shelter_search']['default_location'] : '',
  );
  $form['shelter_search']['show_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show search name field on page'),
    '#default_value' => isset($cfg['shelter_search']['show_name']) ? $cfg['shelter_search']['show_name'] : TRUE,
  );
  $form['shelter_search']['default_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default search name'),
    '#description' => t('Default name to execute search on'),
    '#default_value' => (!empty($cfg['shelter_search']['default_name'])) ? $cfg['shelter_search']['default_name'] : '',
  );
  $form['shelter_search']['use_pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => isset($cfg['shelter_search']['use_pager']) ? $cfg['shelter_search']['use_pager'] : TRUE,
  );
  $form['shelter_search']['count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items to show per page'),
    '#default_value' => (!empty($cfg['shelter_search']['count'])) ? $cfg['shelter_search']['count'] : PETFINDER_SEARCH_DEFAULT_COUNT,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['shelter_search']['max_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of items to return'),
    '#default_value' => (!empty($cfg['shelter_search']['max_count'])) ? $cfg['shelter_search']['max_count'] : PETFINDER_SEARCH_DEFAULT_MAX_COUNT,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  // Pet search page config items
  $form['pet_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pet search page settings'),
    '#description' => t('Use these settings to control the behavior of the Petfinder pet search page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['pet_search']['show_location'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show search location field on page'),
    '#default_value' => isset($cfg['pet_search']['show_location']) ? $cfg['pet_search']['show_location'] : TRUE,
  );
  $form['pet_search']['default_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Default search location'),
    '#description' => t('Default location to execute search on'),
    '#default_value' => (!empty($cfg['pet_search']['default_location'])) ? $cfg['pet_search']['default_location'] : '',
  );
  $form['pet_search']['show_animal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show search animal field on page'),
    '#default_value' => isset($cfg['pet_search']['show_animal']) ? $cfg['pet_search']['show_animal'] : TRUE,
  );
  $default_animal = isset($form_state['values']['pet_search']['default_animal']) ? $form_state['values']['pet_search']['default_animal'] : (!empty($cfg['pet_search']['default_animal']) ? $cfg['pet_search']['default_animal'] : '');
  $form['pet_search']['default_animal'] = array(
    '#type' => 'select',
    '#title' => t('Default search animal type'),
    '#description' => t('Default animal type to execute search on'),
    '#options' => _petfinder_get_animals(),
    '#empty_value' => '',
    '#default_value' => $default_animal,
    '#ajax' => array(
      'callback' => '_petfinder_default_animal',
      'wrapper' => 'ajax-petfinder-default-breed'
    ),
  );
  $form['pet_search']['show_breed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show search breed field on page'),
    '#default_value' => isset($cfg['pet_search']['show_breed']) ? $cfg['pet_search']['show_breed'] : TRUE,
  );
  $form['pet_search']['default_breed'] = array(
    '#type' => 'select',
    '#title' => t('Default search breed'),
    '#description' => t('Default animal breed to execute search on.  Only valid if animal type is given above.'),
    '#options' => _petfinder_get_breeds($default_animal),
    '#empty_value' => PETFINDER_SELECT_NONE,
    '#default_value' => (!empty($cfg['pet_search']['default_breed'])) ? $cfg['pet_search']['default_breed'] : PETFINDER_SELECT_NONE,
    '#prefix' => '<div id="ajax-petfinder-default-breed">',
    '#suffix' => '</div>',
  );
  $form['pet_search']['use_pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => isset($cfg['pet_search']['use_pager']) ? $cfg['pet_search']['use_pager'] : TRUE,
  );
  $form['pet_search']['count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items to show per page'),
    '#default_value' => (!empty($cfg['pet_search']['count'])) ? $cfg['pet_search']['count'] : PETFINDER_SEARCH_DEFAULT_COUNT,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['pet_search']['max_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of items to return'),
    '#default_value' => (!empty($cfg['pet_search']['max_count'])) ? $cfg['pet_search']['max_count'] : PETFINDER_SEARCH_DEFAULT_MAX_COUNT,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  // Submit elements
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Implements the form validation callback for petfinder_admin_form
 */
function petfinder_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Create a dummy Petfinder session to test the keys provided
  $pf = new PetfinderSession();
  $retval = $pf->init($values['api_key'], $values['secret_key']);
  if (!$retval) {
    form_set_error('keys_validation', t('The provided API key + secret combination is invalid, unable to create a signed test connection.  Please check your values and try again.'));
  }
}

/**
 * Implements the form submit callback for petfinder_admin_form
 */
function petfinder_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  _petfinder_set_cfg($values);
  drupal_set_message(t('Petfinder settings successfully saved'));
}

