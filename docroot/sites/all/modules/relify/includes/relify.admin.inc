<?php

/**
 * @file
 * Administrative forms and page callbacks for Relify.
 */


/**
 * Builds the Relify settings form.
 */
function relify_settings_form($form, &$form_state) {
  $form['relify_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#description' => t('You can currently only connect this site to a single Relify client application.'),
    '#default_value' => relify_client_id(),
  );

  $form['relify_client_secret'] = array(
    '#type' => 'password',
    '#title' => t('Client secret'),
    '#description' => t('Your client secret can be found in the right sidebar of your account page.'),
    '#default_value' => relify_client_secret(),
  );

  $default_value = relify_access_token();

  $form['relify_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token'),
    '#default_value' => empty($default_value) ? t('Not authenticated yet') : $default_value,
    '#disabled' => TRUE,
  );

  $form['relify_api_logging'] = array(
    '#type' => 'radios',
    '#title' => t('Activity logging'),
    '#description' => t('If enabled, the specified information will be recorded in the system log.'),
    '#options' => array(
      0 => t('Only log errors'),
      1 => t('Log all API requests and responses'),
    ),
    '#default_value' => variable_get('relify_api_logging', 0),
  );

  $form['#validate'][] = 'relify_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Validate callback: ensure the client ID and client secret match.
 */
function relify_settings_form_validate($form, &$form_state) {
  // If both a client ID and client secret are given, attempt authentication.
  $client_id = $form_state['values']['relify_client_id'];
  $client_secret = $form_state['values']['relify_client_secret'];

  if (!empty($client_id) && !empty($client_secret)) {
    $response = relify_request_access_token($client_id, $client_secret);

    // If the request succeeded...
    if ($response->code == 200) {
      // Store the access token and its expiration timestamp as variables.
      $data = drupal_json_decode($response->data);
      $form_state['values']['relify_access_token'] = $data['access_token'];
      variable_set('relify_access_token_expiration', REQUEST_TIME + $data['expires_in']);

      drupal_set_message(t('Relify API access token generated for the specified client.'));
    }
    else {
      // Otherwise generate an error on the form.
      form_set_error('relify_client_id', t('Authentication failed. Please confirm your client ID and client secret and try again.'));
    }
  }
  elseif (!empty($client_id) && $client_id != variable_get('relify_client_id', '')) {
    // Clear the client secret and access token if the cliend ID changes.
    variable_del('relify_client_secret');
    $token = variable_get('relify_access_token', '');
    variable_del('relify_access_token');
    variable_del('relify_access_token_expiration');
    $form_state['values']['relify_access_token'] = '';

    if (!empty($token)) {
      drupal_set_message(t('Changing the client ID requires you to re-enter the client secret to generate a new access token.'));
    }
  }

  // Don't save the "Not authenticated yet" token value.
  if ($form_state['values']['relify_access_token'] == t('Not authenticated yet')) {
    $form_state['values']['relify_access_token'] = '';
  }
}

/**
 * Displays a table of all Relify related rule configurations.
 */
function relify_rules_page() {
  RulesPluginUI::$basePath = 'admin/config/services/relify/rules';
  $options = array('show execution op' => FALSE);

  // Add the table for enabled Relify rule configurations.
  $content['enabled']['title']['#markup'] = '<h3>' . t('Enabled Relify rule configurations') . '</h3>';

  $conditions = array('tags' => array('Relify', 'relify'), 'active' => TRUE);
  $content['enabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['enabled']['rules']['#empty'] = t('There are no active Relify rule configurations.');

  // Add the table for disabled payment method rules.
  $content['disabled']['title']['#markup'] = '<h3>' . t('Disabled Relify rule configurations') . '</h3>';

  $conditions['active'] = FALSE;
  $content['disabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['disabled']['rules']['#empty'] = t('There are no disabled Relify rule configurations.');

  return $content;
}

/**
 * Displays a table of all data sets defined through the site.
 */
function relify_data_sets_page() {
  drupal_add_css(drupal_get_path('module', 'relify') . '/theme/relify.admin.css');

  $header = array(
    t('Name'),
    t('Keys'),
    t('Operations'),
  );

  $rows = array();

  foreach (relify_data_set_load_multiple(relify_client_id()) as $data_set_id => $data_set) {
    // Get the operations links.
    $links = menu_contextual_links('relify-data-set', 'admin/config/services/relify/data-sets', array($data_set_id));

    // Format the keys array into a width limited comma-separated list.
    $csv_keys = array();
    $items = array();

    foreach ($data_set['keys'] as $data_set_key) {
      $items[] = t('@name (@type)', array('@name' => $data_set_key['name'], '@type' => $data_set_key['type']));

      if (count($items) == 5) {
        $csv_keys[] = implode(', ', $items);
        $items = array();
      }
    }

    if (count($items) > 0) {
      $csv_keys[] = implode(', ', $items);
    }

    // Add the data set's row to the table's rows array.
    $rows[] = array(
      check_plain($data_set['name']),
      implode('<br />', $csv_keys),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no data sets are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new data set.
    $rows[] = array(
      array(
        'data' => t('There are no data sites defined locally yet. <a href="!link">Add data set</a>.', array('!link' => url('admin/config/services/relify/data-sets/add'))),
        'colspan' => 3,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('relify-data-sets'))));
}

/**
 * Form callback: add / edit form for a data set.
 *
 * @todo:
 * - Replace the textarea with a table-based interface for adding keys and
 *   setting their data types.
 */
function relify_data_set_form($form, &$form_state, $data_set) {
  $form_state['data_set'] = $data_set;

  if (!empty($data_set['data_set_id'])) {
    $form['data_set_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Data set ID'),
      '#default_value' => $data_set['data_set_id'],
      '#disabled' => TRUE,
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $data_set['name'],
    '#required' => TRUE,
    '#weight' => 0,
  );

  $form['keys'] = array(
    '#type' => 'textarea',
    '#title' => t('Keys'),
    '#description' => t('Enter a comma-separated list of keys per the Relify documentation. Keys cannot be deleted after a data set is saved, only appended.'),
    '#default_value' => relify_data_set_keys_array_to_string($data_set['keys']),
    '#required' => TRUE,
    '#weight' => 10,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save data set'),
    '#suffix' => l(t('Cancel'), 'admin/config/services/relify/data-sets'),
  );

  return $form;
}

/**
 * Form validate handler: validates the name and keys used in the data set.
 *
 * @todo:
 * - Ensure a duplicate name isn't given.
 * - Ensure no keys have been deleted.
 * - Ensure all keys have valid machine-names.
 * - Ensure the data types given are valid.
 * - Do not allow settings or state as key names to prevent collisions in Rules.
 */
function relify_data_set_form_validate($form, &$form_state) {
}

/**
 * Form submit handler: saves a data set.
 *
 * @todo:
 * - Add support for data set updates.
 */
function relify_data_set_form_submit($form, &$form_state) {
  // Extract and update the data set.
  $data_set = $form_state['data_set'];
  $data_set['name'] = $form_state['values']['name'];
  $data_set['keys'] = $form_state['values']['keys'];

  // If this is a new data set...
  if (empty($data_set['data_set_id'])) {
    // Initialize the client ID.
    $data_set['client_id'] = relify_client_id();

    // Attempt to create the new data set at Relify.
    $response = relify_request_data_set_create($data_set['client_id'], $data_set['name'], $data_set['keys']);

    // If it succeeded, save the data set and key data locally.
    if ($response->code == 200) {
      // Add the new data set ID to the info array.
      $data = drupal_json_decode($response->data);
      $data_set['data_set_id'] = $data['data_set_id'];

      // Convert the keys string into an array.
      $data_set['keys'] = relify_data_set_keys_string_to_array($data_set['keys']);

      relify_data_set_save($data_set);

      drupal_set_message(t('Data set %name saved', array('%name' => $data_set['name'])));
    }
    else {
      // If it failed, rebuild the form with an error message.
      $form_state['rebuild'] = TRUE;
      $data = drupal_json_decode($response->data);

      drupal_set_message(t('Data set could not be saved.'), 'error');
      drupal_set_message(t('<a href="!url">Error @code</a>: @message', array('!url' => $data['help'], '@code' => $data['code'], '@message' => $data['message'])), 'error');
    }
  }
  else {
    drupal_set_message(t('Data set updates are not yet supported.'), 'warning');
  }

  $form_state['redirect'] = 'admin/config/services/relify/data-sets';
}

/**
 * Form callback: confirmation form for deleting a data set.
 *
 * @param $data_set
 *   The data set to be deleted.
 *
 * @see confirm_form()
 */
function relify_data_set_delete_form($form, &$form_state, $data_set) {
  $form_state['data_set'] = $data_set;

  $form = confirm_form($form,
    t('Are you sure you want to delete %name?', array('%name' => $data_set['name'])),
    'admin/config/services/relify/data-sets',
    '<p>' . t('Deleting this data set cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Button submit handler: deletes a data set.
 */
function relify_data_set_delete_form_submit($form, &$form_state) {
  // First attempt to delete the data set at Relify.
  $data_set = $form_state['data_set'];
  $response = relify_request_data_set_delete($data_set['data_set_id']);

  // If it succeeded, delete the local data set and key data.
  if ($response->code == 200) {
    relify_data_set_delete($data_set);
    drupal_set_message(t('Data set %name deleted.', array('%name' => $data_set['name'])));
  }
  else {
    $data = drupal_json_decode($response->data);

    drupal_set_message(t('Data set %name could not be deleted.', array('%name' => $data_set['name'])), 'error');
    drupal_set_message(t('<a href="!url">Error @code</a>: @message', array('!url' => $data['help'], '@code' => $data['code'], '@message' => $data['message'])), 'error');
  }

  $form_state['redirect'] = 'admin/config/services/relify/data-sets';
}

/**
 * Displays an overview of all recommenders defined through the site.
 */
function relify_recommenders_page() {
  drupal_add_css(drupal_get_path('module', 'relify') . '/theme/relify.admin.css');

  $header = array(
    t('Name'),
    t('Data set'),
    t('User identifier / keys'),
    t('Item identifier / keys'),
    t('Operations'),
  );

  $rows = array();

  foreach (relify_recommender_load_multiple(relify_client_id()) as $recommender_id => $recommender) {
    // Load the data set.
    $data_set = relify_data_set_load($recommender['data_set_id']);

    // Get the operations links.
    $links = menu_contextual_links('relify-recommender', 'admin/config/services/relify/recommenders', array($recommender_id));

    // Format the user keys into a width limited comma-separated list.
    $csv_user_keys = array();
    $items = array(t('@name (identifier)', array('@name' => $recommender['user_identifier'])));

    foreach ($recommender['user_keys'] as $user_key) {
      $items[] = check_plain($user_key);

      if (count($items) == 5) {
        $csv_user_keys[] = implode(', ', $items);
        $items = array();
      }
    }

    if (count($items) > 0) {
      $csv_user_keys[] = implode(', ', $items);
    }

    // Format the item keys into a width limited comma-separated list.
    $csv_item_keys = array();
    $items = array(t('@name (identifier)', array('@name' => $recommender['item_identifier'])));

    foreach ($recommender['item_keys'] as $item_key) {
      $items[] = check_plain($item_key);

      if (count($items) == 5) {
        $csv_item_keys[] = implode(', ', $items);
        $items = array();
      }
    }

    if (count($items) > 0) {
      $csv_item_keys[] = implode(', ', $items);
    }

    // Add the recommender's row to the table's rows array.
    $rows[] = array(
      check_plain($recommender['name']),
      check_plain($data_set['name']),
      implode('<br />', $csv_user_keys),
      implode('<br />', $csv_item_keys),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no recommenders are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new recommender.
    $rows[] = array(
      array(
        'data' => t('There are no recommenders defined locally yet. <a href="!link">Add recommender</a>.', array('!link' => url('admin/config/services/relify/recommenders/add'))),
        'colspan' => 5,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('relify-recommenders'))));
}

/**
 * Form callback: add / edit form for a recommender.
 *
 * @todo:
 * - Replace the textareas with a table-based interface for adding keys and
 *   signifying the identifier.
 */
function relify_recommender_form($form, &$form_state, $recommender) {
  $form_state['recommender'] = $recommender;

  if (!empty($recommender['recommender_id'])) {
    $form['recommender_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Recommender ID'),
      '#default_value' => $recommender['recommender_id'],
      '#disabled' => TRUE,
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $recommender['name'],
    '#required' => TRUE,
    '#weight' => 0,
  );

  $options = relify_data_set_load_multiple(relify_client_id());

  foreach ($options as $data_set_id => $data_set) {
    $options[$data_set_id] = check_plain($data_set['name'] . ': ' . $data_set_id);
  }

  reset($options);

  $form['data_set_id'] = array(
    '#type' => 'radios',
    '#title' => t('Data set'),
    '#options' => $options,
    '#default_value' => empty($recommender['data_set_id']) ? key($options) : $recommender['data_set_id'],
    '#required' => TRUE,
    '#weight' => 10,
  );

  $form['user_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('User identifier'),
    '#description' => t('The name of the data set key that identifies a user to this recommender.'),
    '#default_value' => $recommender['user_identifier'],
    '#required' => TRUE,
    '#weight' => 20,
  );

  $form['user_keys'] = array(
    '#type' => 'textarea',
    '#title' => t('User keys'),
    '#description' => t('Enter a comma-separated list of data set key names that describe the user.'),
    '#default_value' => implode(',', $recommender['user_keys']),
    '#weight' => 30,
  );

  $form['item_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Item identifier'),
    '#description' => t('The name of the data set key that identifies an item to this recommender.'),
    '#default_value' => $recommender['item_identifier'],
    '#required' => TRUE,
    '#weight' => 40,
  );

  $form['item_keys'] = array(
    '#type' => 'textarea',
    '#title' => t('Item keys'),
    '#description' => t('Enter a comma-separated list of data set key names that describe the item.'),
    '#default_value' => implode(',', $recommender['item_keys']),
    '#weight' => 50,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save recommender'),
    '#suffix' => l(t('Cancel'), 'admin/config/services/relify/recommenders'),
  );

  return $form;
}

/**
 * Form validate handler: validates the name of the recommender.
 *
 * @todo:
 * - Ensure a duplicate name isn't given.
 */
function relify_recommender_form_validate($form, &$form_state) {
}

/**
 * Form submit handler: saves a recommender.
 *
 * @todo:
 * - Add support for recommender updates.
 */
function relify_recommender_form_submit($form, &$form_state) {
  // Extract and update the recommender.
  $recommender = $form_state['recommender'];
  $recommender['name'] = $form_state['values']['name'];
  $recommender['data_set_id'] = $form_state['values']['data_set_id'];
  $recommender['user_identifier'] = trim($form_state['values']['user_identifier']);
  $recommender['item_identifier'] = trim($form_state['values']['item_identifier']);

  if (trim($form_state['values']['user_keys']) != '') {
    $recommender['user_keys'] = explode(',', $form_state['values']['user_keys']);

    foreach ($recommender['user_keys'] as $key => &$value) {
      $value = trim($value);
    }
  }
  else {
    $recommender['user_keys'] = array();
  }

  if (trim($form_state['values']['item_keys']) != '') {
    $recommender['item_keys'] = explode(',', $form_state['values']['item_keys']);

    foreach ($recommender['item_keys'] as $key => &$value) {
      $value = trim($value);
    }
  }
  else {
    $recommender['item_keys'] = array();
  }

  // If this is a new recommender...
  if (empty($recommender['recommender_id'])) {
    // Attempt to create the new recommender at Relify.
    $response = relify_request_recommender_create($recommender);

    // If it succeeded, save the recommender data locally.
    if ($response->code == 200) {
      // Add the new recommender ID to the info array.
      $data = drupal_json_decode($response->data);
      $recommender['recommender_id'] = $data['recommender_id'];

      relify_recommender_save($recommender);

      drupal_set_message(t('Recommender %name saved', array('%name' => $recommender['name'])));
    }
    else {
      // If it failed, rebuild the form with an error message.
      $form_state['rebuild'] = TRUE;
      $data = drupal_json_decode($response->data);

      drupal_set_message(t('Recommender could not be saved.'), 'error');
      drupal_set_message(t('<a href="!url">Error @code</a>: @message', array('!url' => $data['help'], '@code' => $data['code'], '@message' => $data['message'])), 'error');
    }
  }
  else {
    drupal_set_message(t('Recommender updates are not yet supported.'), 'warning');
  }

  $form_state['redirect'] = 'admin/config/services/relify/recommenders';
}

/**
 * Form callback: confirmation form for deleting a recommender.
 *
 * @param $recommender
 *   The recommender to be deleted.
 *
 * @see confirm_form()
 */
function relify_recommender_delete_form($form, &$form_state, $recommender) {
  $form_state['recommender'] = $recommender;

  $form = confirm_form($form,
    t('Are you sure you want to delete %name?', array('%name' => $recommender['name'])),
    'admin/config/services/relify/recommenders',
    '<p>' . t('Deleting this recommender cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Button submit handler: deletes a recommender.
 */
function relify_recommender_delete_form_submit($form, &$form_state) {
  // First attempt to delete the recommender at Relify.
  $recommender = $form_state['recommender'];
  $response = relify_request_recommender_delete($recommender['recommender_id']);

  // If it succeeded, delete the local recommender.
  if ($response->code == 200) {
    relify_recommender_delete($recommender);
    drupal_set_message(t('Recommender %name deleted.', array('%name' => $recommender['name'])));
  }
  else {
    $data = drupal_json_decode($response->data);

    drupal_set_message(t('Recommender %name could not be deleted.', array('%name' => $recommender['name'])), 'error');
    drupal_set_message(t('<a href="!url">Error @code</a>: @message', array('!url' => $data['help'], '@code' => $data['code'], '@message' => $data['message'])), 'error');
  }

  $form_state['redirect'] = 'admin/config/services/relify/recommenders';
}
