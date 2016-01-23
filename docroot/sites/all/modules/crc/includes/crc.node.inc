<?php

/**
 * @file
 * This handles the crc pseudo-hooks for the following modules:
 *  node & filter
 *
 * It implement the following pseudo-hooks
 *
 *  hook_crc_node_form($settings, $org_form, $form_state)
 *  hook_crc_node_prepopulate(&$node, $settings, $values, $base_node)
 *  hook_crc_type_settings($type, $settings, $org_form, $form_state)
 *  hook_crc_type_settings_submit($type, &$values, $form, &$form_state)
 *  hook_crc_settings($settings)
 *  // TODO
 *  hook_crc_node_form_validate($settings, $form_state, $base_node)
 */

/**
 * Implementation of hook_crc_node_form().
 *
 * This provides the additional form elements to show on the
 * create related content page.
 *
 * @param $settings Settings for all moudles
 * @param $form_state
 * @param $base_node Parent node
 * @return $form
 */
function node_crc_node_form($settings, $form_state, $base_node) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
  );
  return $form;
}

/**
 * Helper function to set the default values.
 *
 * @param $settings
 * @return void
 */
function node_crc_settings(&$settings = array()) {
  $settings += array(
    'body' => array(),
    'author' => array(),
  );
  $settings['body'] += array(
    'options' => 3,
    'lorem_ipsum' => array(),
    'format' => 2,
    'format_custom' => variable_get('filter_default_format', 1),
  );
  $settings['body']['lorem_ipsum'] += array(
    'prefix' => 1,
    'template' => '<p>[lorem_ipsum]</p>',
    'paragraph_count' => 2,
  );
  $settings['author'] += array(
    'user' => 2,
    'user_custom' => '',
    'time' => 2,
    'time_custom' => '',
  );
}

/**
 * Implementation of hook_crc_node_prepopulate().
 *
 * This prepopulates the node object before it is passed
 * to the node_form.
 *
 * @param $node
 * @param $settings
 * @param $values
 * @param $base_node
 * @return void
 */
function node_crc_node_prepopulate(&$node, $settings, $values, $base_node) {
  global $user;
  $node->nid = NULL;
  $node->vid = NULL;
  $node->title = $values['name'];
  switch ($settings['body']['options']) {
    case 1:
      $node->body = $base_node->body;
      break;
    case 2:
      $node->body = '';
      break;
    case 3:
    default:
      $node->body = crc_lorem_ipsum($settings['body']['lorem_ipsum']['paragraph_count'], $settings['body']['lorem_ipsum']['template'], $settings['body']['lorem_ipsum']['prefix']);
      break;
  }
  $node->teaser = node_teaser($node->body);

  switch ($settings['body']['format']) {
    case 1:
      $node->format = $base_node->format;
      break;
    case 3:
      $node->format = $settings['body']['format_custom'];
      break;
    case 2:
    default:
      $node->format = variable_get('filter_default_format', 1);
      break;
  }
  $node->filter = $node->format;

  switch ($settings['author']['user']) {
    case 1:
      $node->name = $base_node->name;
      $node->uid = $base_node->uid;
      break;
    case 3:
      $node->name = '';
      $node->uid = NULL;
      if ($account = user_load($settings['author']['user_custom'])) {
        $node->name = $account->name;
        $node->uid = $account->uid;
      }
      break;
    case 2:
    default:
      $node->name = $user->name;
      $node->uid = $user->uid;
      break;
  }
  switch ($settings['author']['time']) {
    case 1:
      $node->date = format_date($base_node->created, 'custom', 'Y-m-d H:i:s O');
      break;
    case 2:
    default:
      if (empty($settings['author']['time_custom'])) {
        $node->date = '';
      }
      else {
        $node->date = format_date($settings['author']['time_custom']);
      }

      break;
  }
}

/**
 * Implementation of hook_crc_type_settings().
 *
 * This provides the form elements to show on the edit content
 * type form. These are used to set the defaults for the
 * content type.
 *
 * @param $type
 * @param $settings
 * @param $org_form
 * @param $form_state
 * @return $form
 */
function node_crc_type_settings($type, $settings, $org_form, $form_state) {
  $type_settings = $org_form['#node_type'];
  $form = array('body' => array(), 'author' => array());

  // Only include empty option if min_word_count is not set
  $body_options = array(1 => t('Clone'), 2 => t('Empty'), 3 => t('Lorem ipsum'));
  if ($type_settings->min_word_count) {
    unset($body_options[2]);
  }
  $form['body']['options'] = array(
    '#title' => t('%label settings', array('%label' => $type_settings->body_label)),
    '#type' => 'radios',
    '#options' =>  $body_options,
    '#default_value' => $settings['body']['options'],
  );
  $form['body']['lorem_ipsum']['prefix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefix the first paragraph with the text "Lorem ipsum...".'),
    '#default_value' => $settings['body']['lorem_ipsum']['prefix'],
    '#description' => t('This option appends the text %ll to the first paragraph of lorem ipsum.', array('%ll' => t('Lorem ipsum dolor sit amet, consectetur adipiscing elit.'))),
  );

  $form['body']['lorem_ipsum']['template'] = array(
    '#type' => 'textfield',
    '#title' => t('Default lorem ipsum paragraph template'),
    '#default_value' => $settings['body']['lorem_ipsum']['template'],
    '#description' => t('Insert the tag !tag where you want the lorem ipsum text to go. If the tag is missing, only the template text is inserted. Note that a single newline to appended to every paragraph inserted.', array('!tag' => '[lorem_ipsum]')),
  );

  $form['body']['lorem_ipsum']['paragraph_count'] = array(
    '#type' => 'select',
    '#title' => t('Number of paragraphs to add'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => $settings['body']['lorem_ipsum']['paragraph_count'],
  );

  $form['body']['format'] = array(
    '#title' => t('%label filter settings', array('%label' => $type_settings->body_label)),
    '#type' => 'radios',
    '#options' =>  array('1' => t('Clone'), '2' => t('Default'), '3' => t('Custom')),
    '#default_value' => $settings['author']['user'],
  );
  $formats = array();
  foreach (filter_formats() as $format) {
    $formats[$format->format] = $format->name;
  }
  $form['body']['format_custom'] = array(
    '#title' => t('%label filter custom settings', array('%label' => $type_settings->body_label)),
    '#type' => 'radios',
    '#options' =>  $formats,
    '#default_value' => $settings['body']['format_custom'] ? $settings['body']['format_custom'] : variable_get('filter_default_format', 1),
  );

  $form['author']['user'] = array(
    '#title' => t('Author settings'),
    '#type' => 'radios',
    '#options' =>  array('1' => t('Clone'), '2' => t('Logged in user'), '3' => t('Custom')),
    '#default_value' => $settings['author']['user'],
  );

  $user_default = '';
  if ($settings['author']['user_custom']) {
    if ($user = user_load($settings['author']['user_custom'])) {
      $user_default = $user->name;
    }
  }
  $form['author']['user_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom authored by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $user_default,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );

  $form['author']['time'] = array(
    '#title' => t('Timestamp settings'),
    '#type' => 'radios',
    '#options' =>  array('1' => t('Clone'), '2' => t('Custom')),
    '#default_value' => $settings['author']['time'],
  );
  $time_default = $settings['author']['time_custom'] ? format_date($settings['author']['time_custom'], 'custom', 'Y-m-d H:i:s O') : '';
  $form['author']['time_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom authored on'),
    '#maxlength' => 25,
    '#default_value' => $time_default,
    '#description' => t('Format: %time. Leave blank to use the time of form submission.', array('%time' => format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O'))),
  );
  return $form;
}

/**
 * Implementation of hook_crc_type_settings_submit().
 *
 * This updates any of the form values before they are saved.
 *
 * @param $type
 * @param $values
 * @param $form
 * @param $form_state
 * @return void
 */
function node_crc_type_settings_submit($type, &$values, $form, &$form_state) {
  if ($account = user_load_by_name($values['author']['user_custom'])) {
    $values['author']['user_custom'] = $account->uid;
  }
  else {
    $values['author']['user_custom'] = '';
  }
  $values['author']['time_custom'] = !empty($values['author']['time_custom']) ? strtotime($values['author']['time_custom']) : '';
}
