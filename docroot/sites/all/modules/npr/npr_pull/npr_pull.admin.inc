<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers.
 */

/**
 * Form constructor for the npr pull cron config form.
 *
 * Generates a form to choose what types of stories will be pulled.
 * Callback for admin/config/services/npr/cron.
 *
 * @see npr_pull_menu()
 * @ingroup forms
 */
function npr_pull_cron_config() {

  if (!variable_get('npr_api_api_key')) {
    drupal_set_message(t('Story pulls will not work without a valid NPR API key. !npr_link. !admin_link.', array('!npr_link' => l(t('Get a key from NPR'), 'http://www.npr.org/templates/reg/'), '!admin_link' => l(t('Add key to NPR settings'), 'admin/config/services/npr/api_config'))), 'warning');
  }

  $form['npr_pull_cron_query_ids'] = array(
    '#type' => 'checkboxes',
    '#title' => t('IDs'),
    '#default_value' => variable_get('npr_pull_cron_query_ids', array()),
    '#options' => array(
      '15709577'  => t('All Songs Considered'),
      '102920358' => t('All Tech Considered'),
      '7282089'   => t('Blog of the Nation'),
      '93568166'  => t('Monkey See'),
      '103943429' => t('The Two Way'),
      '1001'      => t('NPR Top Stories'),
    ),
    '#description' => t('Choose one or more topics. New stories from these topic(s) will be be pulled down at every cron run.')
  );

  $form['node_settings'] = array(
    '#title' => t('Node settings'),
    '#type' => 'fieldset',
  );

  $options = variable_get('node_options_' . NPR_PULL_MAPPING_CONTENT_TYPE, array('status', 'promote'));

  // Skipping 'publish', as that value
  // is overridden by 'npr_pull_cron_publish_flag' (below)
  $labels = array(
    'promote' => t('Promoted to front page'),
    'sticky' => t('Sticky at top of lists'),
    'revision' => t('Create a new revision'),
  );

  $items = array();
  foreach ($labels as $key => $label) {
    $setting = in_array($key, $options) ? t('Yes') : t('No');
    $items[] = array('data' => $label . ": " . $setting);
  }

  $markup = t("Stories pulled down from the NPR API will be turned into !type nodes, with the following additional settings", array('!type' => NPR_PULL_MAPPING_CONTENT_TYPE));

  if (user_access('administer content types')) {
    $markup .= ' [' . l(t('Edit settings'), 'admin/structure/types/manage/' . str_replace('-', '_', NPR_PULL_MAPPING_CONTENT_TYPE)) . ']';
  }

  $markup .= ':' . theme('item_list', array('items' => $items));

  $form['node_settings']['npr_pull_config_explanation'] = array(
    '#markup' => $markup,
  );

  $form['node_settings']['npr_pull_cron_publish_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Publish stories upon retrieval?'),
    '#default_value' => variable_get('npr_pull_cron_publish_flag', 1),
    '#description' => t('If checked stories will automatically be published. If not, stories will still be retrieved and saved in your database - but not published. This setting overrides the default Publish setting for this content type.')
  );

  return system_settings_form($form);
}

/**
 * Form constructor for the npr pull tags config form.
 *
 * Generates a form to map NPR API tags to local terms.
 * Callback for admin/config/services/npr/tags.
 *
 * @see npr_pull_menu()
 * @ingroup forms
 */
function npr_pull_tags_config() {

  $form['npr_pull_map_tags'] = array(
    '#type' => 'checkbox',
    '#title' => 'Turn on tag mapping',
    '#default_value' => variable_get('npr_pull_map_tags'),
    '#description' => 'Map NPR tags from pulled stories to local taxonomy terms.',
  );

  $vocabs = array();
  foreach (taxonomy_vocabulary_get_names() as $vocab) {
    $vocabs[$vocab->vid] = $vocab->name;
  }
  $form['npr_pull_mapped_vid'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('npr_pull_mapped_vid'),
    '#options' => $vocabs,
    '#description' => 'Map NPR tags to terms in the chosen category. (Be sure that the vocabulary you choose has been attached as a term_reference field to the NPR Story content type, is not a required field, can accept unlimited values, and uses the "Autocomplete term widget (tagging)" widget.)'
  );
  return system_settings_form($form);
}

/**
 * Form constructor for the ad-hoc npr pull form.
 *
 * Generates a form to retrieve an NPR story by URL.
 * Callback for admin/content/npr.
 *
 * @see npr_pull_menu()
 * @see npr_pull_get_story_validate()
 * @see npr_pull_get_story_submit()
 * @ingroup forms
 */
function npr_pull_get_story() {

  if (!variable_get('npr_api_api_key')) {
    drupal_set_message(t('Story pulls will not work without a valid NPR API key. !npr_link. !admin_link.', array('!npr_link' => l(t('Get a key from NPR'), 'http://www.npr.org/templates/reg/'), '!admin_link' => l(t('Add key to NPR settings'), 'admin/config/services/npr/api_config'))), 'warning');
  }

  $form = array();

  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('NPR API story URL'),
    '#required' => TRUE,
    '#description' => t('Full URL for a story on NPR.org.'),
  );

  $form['date_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Publish stories upon retrieval?'),
    '#default_value' => variable_get('npr_pull_cron_publish_flag', 1),
    '#description' => t('If checked stories will automatically be published. If not, stories will still be retrieved and saved in your database - but not published.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Get story'),
  );

  return $form;
}

/**
 * Form validation handler for npr_pull_get_story().
 *
 * @see npr_pull_get_story_submit()
 */
function npr_pull_get_story_validate($form, &$form_state) {

  $story_id = 0;
  $url_value = $form_state['values']['url'];

  if (!valid_url($url_value, TRUE)) {
    form_set_error('url', t('Does not appear to be a valid URL.'));
    return;
  }

  if (!npr_pull_extract_id($url_value)) {
    form_set_error('url', t('Could not extract an NPR ID from given URL.'));
  }
}

/**
 * Form submission handler for npr_pull_get_story().
 *
 * @see npr_pull_get_story_validate()
 */
function npr_pull_get_story_submit($form, &$form_state) {
  $story_id = 0;
  $url_value = $form_state['values']['url'];
  $story_id = npr_pull_extract_id($url_value);

  //build the params to query the API
  $params = array(
    'id' => $story_id,
  );

  $NPR = npr_api_fetch_object($params);
  $failure = !empty($NPR->message) && $NPR->message->id == 201;

  if (is_object($NPR) && !empty($NPR->stories) && !$failure) {
    $NPR->flatten();
    $story = array_shift($NPR->stories);
    $date_flag = $form_state['values']['date_flag'];
    $story->created = ($date_flag) ? strtotime($story->pubDate->value) : REQUEST_TIME;
    npr_pull_insert_story($story);
  }
  else {
    drupal_set_message(t('No story could be found in the NPR API.'));
  }
}

/**
 * Extracts an NPR ID from an NPR URL.
 *
 * @param string $url
 *   A URL.
 */
function npr_pull_extract_id($url) {
  // url format: /yyyy/mm/dd/id
  // url format: /blogs/name/yyyy/mm/dd/id
  preg_match('/http\:\/\/[^\s\/]*npr\.org\/((([^\/]*\/){3,5})([0-9]{8,12}))\/.*/', $url, $date_matches);
  if (!empty($date_matches[4])) {
    return $date_matches[4];
  }
  else {
    // url format: /templates/story/story.php?storyId=id
    preg_match('/http\:\/\/[^\s\/]*npr\.org\/([^&\s\<]*storyId\=([0-9]+)).*/', $url, $template_matches);
    if (!empty($template_matches[2])) {
      return $template_matches[2];
    }
  }
}
