<?php
/**
 * @file
 * Administration pages.
 */

/**
 * Admin settings page callback.
 */
function query_parameters_to_url_admin_settings() {
  $form = array();

  $default_options = array(
    '#states' => array(
      'enabled' => array(
        ':input[name="' . QUERY_PARAMETERS_TO_URL_ENABLED . '"]' => array('checked' => TRUE),
      ),
    )
  );

  $form[QUERY_PARAMETERS_TO_URL_ENABLED] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable rewriting'),
    '#description' => t('If checked, URLs containing query parameters will be rewritten to Clean URL components.'),
    '#default_value' => variable_get(QUERY_PARAMETERS_TO_URL_ENABLED, TRUE),
  );

  $form[QUERY_PARAMETERS_TO_URL_URL_AND_QUERY_DELIMITER] = array(
    '#type' => 'textfield',
    '#title' => t('URL and query parameters delimiter'),
    '#description' => t('This string will be placed in the URL, between real URL components and encoded query parameters.<br/><strong>Example:</strong> If the accessed URI is <string>events/all?category=1&page=2</string> the resulting clean URL will be <strong>events/all/p/category/1/page/2</strong>.'),
    '#default_value' => query_parameters_to_url_url_query_delimiter(),
  ) + $default_options;

  $form[QUERY_PARAMETERS_TO_URL_QUERY_NESTED_KEY_DELIMITER] = array(
    '#type' => 'textfield',
    '#title' => t('Query parameter nested key delimiter'),
    '#description' => t('This string will be used to delimit nested keys for a parameter value.<br/><strong>Example:</strong> If the accessed URI is <strong>events?category_id[0][1][2][3][4]=5</strong> the resulting clearn URL will be <strong>events/all/p/category_id/0__1__2__3__4__5</strong>.'),
    '#default_value' => query_parameters_to_url_nested_key(),
  ) + $default_options;

  $form[QUERY_PARAMETERS_TO_URL_QUERY_NESTED_VALUES_DELIMITER] = array(
    '#type' => 'textfield',
    '#title' => t('Query parameter nested values delimiter'),
    '#description' => t('This string will be used to delimit nested parameter values from one another.<br/><strong>Example:</strong> If the accessed URI is <strong>events?category_id[0][1]=2&category_id[3][4]=5</strong> the resulting clearn URL will be <strong>events/all/p/category_id/0__1__2--3__4__5</strong>.'),
    '#default_value' => query_parameters_to_url_nested_values_delimiter(),
  ) + $default_options;

  $form[QUERY_PARAMETERS_TO_URL_PATH_REG_EXP] = array(
    '#type' => 'textfield',
    '#title' => t('Regular expression to match paths, where rewriting should be enabled'),
    '#description' => t("While you can always reset this configuration and recover without permanent damage to your site, a change to this expression will break old bookmarked URLs. Change only when you know what you're doing.<br>Example: <strong>{(^home|^events|^news/all)}</strong>. To match all paths you can use: <strong>{.+}</strong>. To disable regular expression matching, leave it empty."),
    '#default_value' => query_parameters_to_url_path_reg_exp(),
  ) + $default_options;

  $form['advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $form['advanced_settings']['hook_hint'] = array(
    '#type' => 'item',
    '#title' => t('Additional paths'),
    '#description' => t('You can enable rewriting for additional paths, by implementing <strong>hook_query_parameters_to_url_rewrite_access()</strong>.'),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_ADDITIONAL_PATHS_HOOKS_ENABLED] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable hook invocation to find additional paths'),
    '#description' => t('If checked, <strong>hook_query_parameters_to_url_rewrite_access()</strong> implementations will be invoked, to allow support for additional rewrite paths.'),
    '#default_value' => query_parameters_to_url_additional_paths_hooks_enabled(),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_REWRITE_HOOKS_ENABLED] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable hook invocation to allow rewriting the final encoded paths'),
    '#description' => t('If checked, <strong>hook_query_parameters_to_url_rewrite_alter()</strong> implementations will be invoked, to allow rewriting the final query parameter encoded URLs. See <em>query_parameters_to_url.api.php</em> for an example and documentation.'),
    '#default_value' => query_parameters_to_url_rewrite_hooks_enabled(),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_REDIRECT_PROTECTION_ENABLED] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable protection against redirect loop'),
    '#description' => t('If checked, each redirect in hook_init() is logged, to protect against any possible redirect loops, just like redirect module does it.'),
    '#default_value' => query_parameters_to_url_redirect_protection_enabled(),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_IGNORE_ADMIN_PATHS] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore admin paths'),
    '#description' => t('Whether query parameter encoding should be skipped on admin paths.'),
    '#default_value' => query_parameters_to_url_ignore_admin_paths(),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_REDIRECT_PROTECTION_ENABLED] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect loop threshold'),
    '#description' => t('How many redirects are allowed in 15 seconds, before it is considered that a redirect loop has occurred.'),
    '#default_value' => query_parameters_to_url_redirect_threshold(),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_ALLOW_REWRITTEN_MENU_ITEM_SAVE] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow rewritten menu item save'),
    '#description' => t('If this is checked, you will be able to save an encoded query parameter path as a menu item. Which means you can add a menu item with a path like <em>events/p/page/1</em>. For saving an encoded query parameter path for the front page, make sure you prepend the front page path, like <em>home/p/page/1</em>. <strong>This is an experimental feature.</strong>'),
    '#default_value' => query_parameters_to_url_allow_rewritten_menu_item_save(),
  ) + $default_options;

  $form['advanced_settings'][QUERY_PARAMETERS_TO_URL_REDIRECT_STATUS_CODE] = array(
    '#type' => 'select',
    '#title' => t('HTTP code to use for redirects'),
    '#description' => t('Choose which HTTP Code should be issued for redirects done by this module.'),
    '#options' => query_parameters_to_url_status_code_options(),
    '#default_value' => query_parameters_to_url_redirect_status_code(),
  ) + $default_options;

  return system_settings_form($form);
}

/**
 * Example URLs page callback.
 */
function query_parameters_to_url_admin_examples() {
  $form = array();

  global $conf;
  $conf[QUERY_PARAMETERS_TO_URL_PATH_REG_EXP] = '{(^events|^search|^download-files)}';
  $text_field_size = 180;

  $form['example_urls'] = array(
    '#type' => 'item',
    '#markup' => t('<h2>Below you can find example URLs and the new encoded paths.</h2>'),
  );

  $example = 'events?field_event_category_target_id[0]=1&field_event_category_target_id[1]=2&og_group_ref_target_id[0]=100';
  list($encoded, $options) = query_parameters_to_url_parse_uri($example);
  query_parameters_to_url_url_outbound_alter($encoded, $options, $example);
  $percentage_saved = query_parameters_to_url_compute_saved_char_percentage($encoded, $example);

  $form['example_1'] = array(
    '#type' => 'textfield',
    '#title' => 'URL Example 1',
    '#default_value' => $example,
    '#size' => $text_field_size,
  );

  $form['encoded_1'] = array(
    '#type' => 'textfield',
    '#title' => 'Encoded URL Example 1',
    '#default_value' => $encoded,
    '#size' => $text_field_size,
  );

  $form['characters_saved_1'] = array(
    '#type' => 'item',
    '#markup' => t('!count% characters saved with the new encoded path.', array('!count' => $percentage_saved)),
  );

  $example = 'search/site?page=2&f[0]=bundle%3Astandard_page&f[1]=dm_field_date%3A%5B2014-12-01T00%3A00%3A00Z%20TO%202015-01-01T00%3A00%3A00Z%5D';
  list($encoded, $options) = query_parameters_to_url_parse_uri($example);
  query_parameters_to_url_url_outbound_alter($encoded, $options, $example);
  $percentage_saved = query_parameters_to_url_compute_saved_char_percentage($encoded, $example);

  $form['example_2'] = array(
    '#type' => 'textfield',
    '#title' => 'URL Example 2',
    '#default_value' => $example,
    '#size' => $text_field_size,
  );

  $form['encoded_2'] = array(
    '#type' => 'textfield',
    '#title' => 'Encoded URL Example 2',
    '#default_value' => $encoded,
    '#size' => $text_field_size,
  );

  $form['characters_saved_2'] = array(
    '#type' => 'item',
    '#markup' => t('!count% characters saved with the new encoded path.', array('!count' => $percentage_saved)),
  );

  $example = 'download-files?file[0]=a&file[]=b&file[]=c&file[]=d&file[]=e';
  list($encoded, $options) = query_parameters_to_url_parse_uri($example);
  query_parameters_to_url_url_outbound_alter($encoded, $options, $example);
  $percentage_saved = query_parameters_to_url_compute_saved_char_percentage($encoded, $example);

  $form['example_3'] = array(
    '#type' => 'textfield',
    '#title' => 'URL Example 3',
    '#default_value' => $example,
    '#size' => $text_field_size,
  );

  $form['encoded_3'] = array(
    '#type' => 'textfield',
    '#title' => 'Encoded URL Example 3',
    '#default_value' => $encoded,
    '#size' => $text_field_size,
  );

  $form['characters_saved_3'] = array(
    '#type' => 'item',
    '#markup' => t('!count% characters saved with the new encoded path.', array('!count' => $percentage_saved)),
  );

  return $form;
}

/**
 * Computes the percentage of characters saved using an encoded string.
 */
function query_parameters_to_url_compute_saved_char_percentage($encoded, $example) {
  return $characters_saved = round(100 - (drupal_strlen($encoded) * 100 / drupal_strlen($example)), 2);
}

/**
 * Parses an URI string into path and query parameters.
 */
function query_parameters_to_url_parse_uri($uri) {
  $parsed_url = parse_url($uri);
  $path = $parsed_url['path'];
  $query = array();
  parse_str(urldecode($parsed_url['query']), $query);
  $options = array(
    'query' => $query,
  );
  return array($path, $options);
}
