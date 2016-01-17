<?php
/**
 * @file
 * 
 * This file contains the page callbacks for user-viewable content
 */

/**
 * Page callback for the shelter search page.
 */
function petfinder_shelter_search_page($form, &$form_state) {
  $cfg = _petfinder_get_cfg();
  $shelter_search = !empty($cfg['shelter_search']) ? $cfg['shelter_search'] : array();
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  // Check for pager query params
  if (!$form_state['submitted']) {
    $query_params = drupal_get_query_parameters();
    $fields = array(
      'location',
      'name',
    );
    foreach ($fields as $field) {
      if (!isset($values[$field]) && !empty($query_params[$field])) {
        $form_state['values'][$field] = trim(filter_xss($query_params[$field]));
      }
    }
  }

  // Build output for page
  $form['search'] = _petfinder_shelter_search_form($form_state);
  $form['results'] = _petfinder_shelter_search_results($form_state);

  // Set form attributes
  $form['#submit'][] = '_petfinder_shelter_search_form_submit';
  $form['#validate'][] = '_petfinder_shelter_search_form_validate';
  $form['#action'] = url('petfinder/search/shelters');
  
  // Inject custom CSS
  drupal_add_css(drupal_get_path('module', 'petfinder') . '/css/petfinder.css');

  return $form;
}

/**
 * Form builder utility function for shelter search form
 */
function _petfinder_shelter_search_form($form_state) {
  $form = array();
  $cfg = _petfinder_get_cfg();
  $shelter_search = !empty($cfg['shelter_search']) ? $cfg['shelter_search'] : array();
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  // Check to see if we're supposed to render the location search element
  if (!empty($shelter_search['show_location'])) {
    $form['location'] = array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#description' => t('ZIP -or- City, State/Province'),
      '#default_value' => !empty($values['location']) ? $values['location'] : (!empty($shelter_search['default_location']) ? $shelter_search['default_location'] : ''),
    );
  }
  elseif (!empty($shelter_search['default_location'])) {
    // If not, and we have a default location, set it
    $form['location'] = array(
      '#type' => 'value',
      '#value' => $shelter_search['default_location'],
    );
  }

  // Check to see if we're supposed to render the name search element
  if (!empty($shelter_search['show_name'])) {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Shelter Name'),
      '#description' => t('Partial names are OK'),
      '#default_value' => !empty($values['name']) ? $values['name'] : (!empty($shelter_search['default_name']) ? $shelter_search['default_name'] : ''),
    );
  }
  elseif (!empty($shelter_search['default_name'])) {
    // If not, and we have a default name, set it
    $form['location'] = array(
      '#type' => 'value',
      '#value' => $shelter_search['default_name'],
    );
  }

  // Generate the form submit elements
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Utility function to generate results list for shelter search page
 */
function _petfinder_shelter_search_results($form_state) {
  $form = array();
  $cfg = _petfinder_get_cfg();
  $shelter_search = !empty($cfg['shelter_search']) ? $cfg['shelter_search'] : array();
  $values = isset($form_state['values']) ? $form_state['values'] : array();
  $max_count = (int) (!empty($shelter_search['max_count']) ? $shelter_search['max_count'] : PETFINDER_SEARCH_DEFAULT_MAX_COUNT);
  $count = (int) (!empty($shelter_search['count']) ? $shelter_search['count'] : PETFINDER_SEARCH_DEFAULT_COUNT);
  $use_pager = !empty($shelter_search['use_pager']);
  $offset = 0;

  // Selectively build our search argument array from form values
  $args = array(
    'location' => '',
    'name' => '',
  );
  foreach (array_keys($args) as $field) {
    if (isset($values[$field])) {
      $args[$field] = $values[$field];
    }
  }

  // Generate results based on search form fields
  $shelters = petfinder_shelter_search($args + array('count' => $max_count));
  if ($shelters) {
    // Initialize pager element if optioned
    if ($use_pager) {
      $total = count($shelters['shelters']);
      $page = pager_default_initialize($total, $count);
      $offset = $count * $page;
    }

    // Generate themed output for record list
    $form['shelters'] = array(
      '#prefix' => '<div class="petfinder-shelter-search-results-list">',
      '#suffix' => '</div>',
      '#markup' => theme('petfinder_shelter_list', array('shelters' => $shelters['shelters'], 'offset' => $offset, 'count' => $count)),
    );

    // Generate pager markup
    if ($use_pager) {
      $form['pager'] = array(
        '#prefix' => '<div class="petfinder-shelter-search-results-pager">',
        '#suffix' => '</div>',
        '#theme' => 'pager',
        '#parameters' => $args,
      );
    }
  }
  else {
    $form['message'] = array(
      '#markup' => t('No matches found.  Please change your search criteria and try again.'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }

  return $form;
}

/**
 * Validation handler for shelter search page form
 */
function _petfinder_shelter_search_form_validate($form, &$form_state) {
  // Scrub our form values
  $fields = array(
    'location',
    'name',
  );
  foreach ($fields as $field) {
    if (isset($form_state['values'][$field])) {
      $form_state['values'][$field] = trim(filter_xss($form_state['values'][$field]));
    }
  }
}

/**
 * Submit handler for shelter search page form
 */
function _petfinder_shelter_search_form_submit($form, &$form_state) {
  $fields = array(
    'location',
    'name',
  );
  $args = array();
  foreach ($fields as $field) {
    if (!empty($form_state['values'][$field])) {
      $args[$field] = $form_state['values'][$field];
    }
  }
  $form_state['redirect'] = array('petfinder/search/shelters', array('query' => $args));
}

/**
 * Page callback for the pet search page.
 */
function petfinder_pet_search_page($form, &$form_state) {
  $cfg = _petfinder_get_cfg();
  $pet_search = !empty($cfg['pet_search']) ? $cfg['pet_search'] : array();
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  // Check for pager query params
  if (!$form_state['submitted']) {
    $query_params = drupal_get_query_parameters();
    $fields = array(
      'location',
      'animal',
      'breed',
    );
    foreach ($fields as $field) {
      if (!isset($values[$field]) && !empty($query_params[$field])) {
        $form_state['values'][$field] = trim(filter_xss($query_params[$field]));
      }
    }
  }

  // Build output for page
  $form['search'] = _petfinder_pet_search_form($form_state);
  $form['results'] = _petfinder_pet_search_results($form_state);

  // Set form attributes
  $form['#submit'][] = '_petfinder_pet_search_form_submit';
  $form['#validate'][] = '_petfinder_pet_search_form_validate';
  $form['#action'] = url('petfinder/search/pets');

  return $form;
}

/**
 * Form builder utility function for pet search form
 */
function _petfinder_pet_search_form($form_state) {
  $form = array();
  $cfg = _petfinder_get_cfg();
  $pet_search = !empty($cfg['pet_search']) ? $cfg['pet_search'] : array();
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  // Check to see if we're supposed to render the location search element
  if (!empty($pet_search['show_location'])) {
    $form['location'] = array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#description' => t('ZIP -or- City, State/Province'),
      '#default_value' => !empty($values['location']) ? $values['location'] : (!empty($pet_search['default_location']) ? $pet_search['default_location'] : ''),
      '#required' => TRUE,
    );
  }
  elseif (!empty($pet_search['default_location'])) {
    // If not, and we have a default location, set it
    $form['location'] = array(
      '#type' => 'value',
      '#value' => $pet_search['default_location'],
    );
  }

  // Check to see if we're supposed to render the animal type search element
  $animal = !empty($values['animal']) ? $values['animal'] : (!empty($pet_search['default_animal']) ? $pet_search['default_animal'] : PETFINDER_SELECT_NONE);
  if (!empty($pet_search['show_animal'])) {
    $form['animal'] = array(
      '#type' => 'select',
      '#title' => t('Animal'),
      '#options' => _petfinder_get_animals(),
      '#empty_value' => PETFINDER_SELECT_NONE,
      '#default_value' => $animal,
      '#ajax' => array(
        'callback' => '_petfinder_search_animal',
        'wrapper' => 'ajax-petfinder-search-breed'
      ),
    );
  }
  elseif (!empty($pet_search['default_animal'])) {
    // If not, and we have a default animal type, set it
    $form['animal'] = array(
      '#type' => 'value',
      '#value' => $pet_search['default_animal'],
    );
  }

  // Check to see if we're supposed to render the breed search element
  if (!empty($pet_search['show_breed'])) {
    $form['breed'] = array(
      '#type' => 'select',
      '#title' => t('Breed'),
      '#options' => _petfinder_get_breeds($animal),
      '#empty_value' => PETFINDER_SELECT_NONE,
      '#default_value' => !empty($values['breed']) ? $values['breed'] : (!empty($pet_search['default_breed']) ? $pet_search['default_breed'] : PETFINDER_SELECT_NONE),
      '#prefix' => '<div id="ajax-petfinder-search-breed">',
      '#suffix' => '</div>',
    );
  }
  elseif (!empty($pet_search['default_breed'])) {
    // If not, and we have a default breed, set it
    $form['breed'] = array(
      '#type' => 'value',
      '#value' => $pet_search['default_breed'],
    );
  }

  // Generate the form submit elements
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Utility function to generate results list for pet search page
 */
function _petfinder_pet_search_results($form_state) {
  $form = array();
  $cfg = _petfinder_get_cfg();
  $pet_search = !empty($cfg['pet_search']) ? $cfg['pet_search'] : array();
  $values = isset($form_state['values']) ? $form_state['values'] : array();
  $max_count = (int) (!empty($pet_search['max_count']) ? $pet_search['max_count'] : PETFINDER_SEARCH_DEFAULT_MAX_COUNT);
  $count = (int) (!empty($pet_search['count']) ? $pet_search['count'] : PETFINDER_SEARCH_DEFAULT_COUNT);
  $use_pager = !empty($pet_search['use_pager']);
  $offset = 0;

  // Selectively build our search argument array from form values
  $args = array(
    'location' => '',
    'animal' => '',
    'breed' => '',
  );
  foreach (array_keys($args) as $field) {
    if (isset($values[$field])) {
      $args[$field] = $values[$field];
    }
  }

  // Generate results based on search form fields
  $pets = petfinder_pet_search($args + array('count' => $max_count));
  if ($pets) {
    // Initialize pager element if optioned
    if ($use_pager) {
      $total = count($pets['pets']);
      $page = pager_default_initialize($total, $count);
      $offset = $count * $page;
    }

    // Generate themed output for record list
    $form['pets'] = array(
      '#prefix' => '<div class="petfinder-pet-search-results-list">',
      '#suffix' => '</div>',
      '#markup' => theme('petfinder_pet_list', array('pets' => $pets['pets'], 'offset' => $offset, 'count' => $count)),
    );

    // Generate pager markup
    if ($use_pager) {
      $form['pager'] = array(
        '#prefix' => '<div class="petfinder-pet-search-results-pager">',
        '#suffix' => '</div>',
        '#theme' => 'pager',
        '#parameters' => $args,
      );
    }
  }
  else {
    $form['message'] = array(
      '#markup' => t('No matches found.  Please change your search criteria and try again.'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }

  return $form;
}

/**
 * Validation handler for pet search page form
 */
function _petfinder_pet_search_form_validate($form, &$form_state) {
  // Scrub our form values
  $fields = array(
    'location',
  );
  foreach ($fields as $field) {
    if (isset($form_state['values'][$field])) {
      $form_state['values'][$field] = trim(filter_xss($form_state['values'][$field]));
    }
  }
}

/**
 * Submit handler for pet search page form
 */
function _petfinder_pet_search_form_submit($form, &$form_state) {
  $fields = array(
    'location',
    'animal',
    'breed',
  );
  $args = array();
  foreach ($fields as $field) {
    if (!empty($form_state['values'][$field])) {
      $args[$field] = $form_state['values'][$field];
    }
  }
  $form_state['redirect'] = array('petfinder/search/pets', array('query' => $args));
}
