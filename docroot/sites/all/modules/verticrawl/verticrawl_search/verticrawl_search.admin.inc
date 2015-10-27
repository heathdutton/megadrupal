<?php
/**
 * @file
 * Define Verticrawl adminitration page.
 */

/**
 * Callback function for admin menu drupal form.
 *
 * @see verticrawl_search_php_menu()
 */
function verticrawl_search_admin_form($form, &$form_state) {
  $form['intro'] = array(
    '#markup' => t('<p>You must have set your search API in <a href="http://www.verticrawl.com" target="_blank">Verticrawl</a> first.</p>'),
  );

  $api_key = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_API_CONFIG_ID);

  $form['search_php_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t("API Key - Verticrawl Search PHP"),
    '#description' => t("Enter your <strong>Verticrawl Search PHP</strong> API Key. You can find your own API Key or buy one <a href='http://www.verticrawl.com' target='_blank'>here</a>."),
    '#required' => TRUE,
    '#default_value' => $api_key,
  );

  $search_page_path = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_PATH_CONFIG_ID, VERTICRAWL_SEARCH_DEFAULT_PATH);

  $form['search_page_path'] = array(
    '#type' => 'textfield',
    '#title' => t("Search page path"),
    '#description' => t('You can set <strong>search</strong> as the search page to override default Drupal search page.<br />
                         Or you can use any path available. <em>E.g,</em> <strong>my-search-page</strong>.'),
    '#required' => TRUE,
    '#default_value' => $search_page_path,
  );

  $search_page_title = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_TITLE_CONFIG_ID, VERTICRAWL_SEARCH_DEFAULT_TITLE);

  $form['search_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t("Search page title"),
    '#required' => TRUE,
    '#default_value' => $search_page_title,
  );

  $search_page_description = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_DESCRIPTION_CONFIG_ID);
  $search_page_description_format = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_DESCRIPTION_FORMAT_CONFIG_ID);
  $form['search_page_description'] = array(
    '#type' => 'text_format',
    '#title' => t("Search page description"),
    '#default_value' => $search_page_description,
  );

  if (!empty($search_page_description_format)) {
    $form['search_page_description']['#format'] = $search_page_description_format;
  }

  $keyword_wrappers = array(
    'em' => t('EM'),
    'span' => t('SPAN'),
    'strong' => t('STRONG'),
  );

  $keyword_wrapper = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_KEYWORD_WRAPPER_CONFIG_ID);

  $form['search_result_keyword_wrapper'] = array(
    '#key_type' => 'associative',
    '#multiple_toggle' => '0',
    '#type' => 'select',
    '#options' => $keyword_wrappers,
    '#title' => t("Keyword wrapper"),
    '#description' => t('Keywords in results will be wrapped by this HTML element.'),
    '#default_value' => $keyword_wrapper,
  );

  // Advanced search - fieldset.
  $form['advanced_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced search'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['advanced_search']['intro'] = array(
    '#markup' => t('<p>Select each field displayed in the advanced search form, you can disable the advanced search with managing <a href="/admin/people/permissions#module-verticrawl_search_php">module permissions</a>:</p>'),
  );

  $advanced_search_items = array(
    'sort_by' => array(
      'title' => t('Sort by'),
      'description' => t('Choose how results are sorted : by relevance or by date (ascending or descending).'),
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_SORT_BY_CONFIG_ID,
    ),
    'match_mode' => array(
      'title' => t('Match mode'),
      'description' => t('Choose how keywords are matched : exact word, word contains, word begins with, word ends with.'),
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_MATCH_MODE_CONFIG_ID,
    ),
    'document_type' => array(
      'title' => t('Document type'),
      'description' => t('Choose whitch type of document displayed in results : PDF, Word, Excel, HTML, etc.'),
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_DOCUMENT_TYPE_CONFIG_ID,
    ),
    'site_url' => array(
      'title' => t('Site URL'),
      'description' => t('Filter results by one website URL. Useful with multiple site indexation only.'),
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_SITE_URL_CONFIG_ID,
    ),
    'group_by_site' => array(
      'title' => t('Group by site'),
      'description' => t('Filter results by disctints website URL. Useful with multiple site indexation only.'),
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_GROUP_BY_SITE_CONFIG_ID,
    ),
    'filter_by_date' => array(
      'title' => t('Filter by date'),
      'description' => t('Filter results older or newer than a date.'),
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_FILTER_BY_DATE_CONFIG_ID,
    ),
  );

  foreach ($advanced_search_items as $field => $config) {
    $default_value_field = VerticrawlHelper::getConfigValue($config['config_id'], 1);
    $form['advanced_search'][$field] = array(
      '#title' => $config['title'],
      '#description' => $config['description'],
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => $default_value_field,
    );
  }

  $default_value_powred_by = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_POWERED_BY, 1);
  $form['display_powered_by'] = array(
    '#title' => t('Show "Powered by VERTRICRAWL"'),
    '#description' => t('Display a branding link at the bottom of the page'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => $default_value_powred_by,
  );

  $form['#submit'][] = 'verticrawl_search_admin_form_submit';
  return system_settings_form($form);
}

/**
 * Callback function after submitting drupal form.
 *
 * @see verticrawl_search_engine_form()
 */
function verticrawl_search_admin_form_submit($form, &$form_state) {
  $configs = array(
    // SEARCH PAGE.
    'config_search_php_api_key' => array(
      'config_id' => VERTICRAWL_SEARCH_API_CONFIG_ID,
      'config_name'  => "search_php_api_key",
      'config_value'   => $form_state['values']['search_php_api_key'],
    ),
    'config_search_page_path' => array(
      'config_id' => VERTICRAWL_SEARCH_PAGE_PATH_CONFIG_ID,
      'config_name'  => "search_page_path",
      'config_value'   => $form_state['values']['search_page_path'],
    ),
    'config_search_page_title' => array(
      'config_id' => VERTICRAWL_SEARCH_PAGE_TITLE_CONFIG_ID,
      'config_name'  => "search_page_title",
      'config_value'   => $form_state['values']['search_page_title'],
    ),
    'config_search_page_description' => array(
      'config_id' => VERTICRAWL_SEARCH_PAGE_DESCRIPTION_CONFIG_ID,
      'config_name'  => "search_page_description",
      'config_value'   => $form_state['values']['search_page_description']['value'],
    ),
    'config_search_page_description_format' => array(
      'config_id' => VERTICRAWL_SEARCH_PAGE_DESCRIPTION_FORMAT_CONFIG_ID,
      'config_name'  => "search_page_description_format",
      'config_value'   => $form_state['values']['search_page_description']['format'],
    ),
    'config_search_result_keyword_wrapper' => array(
      'config_id' => VERTICRAWL_SEARCH_KEYWORD_WRAPPER_CONFIG_ID,
      'config_name'  => "search_result_keyword_wrapper",
      'config_value'   => $form_state['values']['search_result_keyword_wrapper'],
    ),
    // ADVANCED SEARCH COMPONENTS.
    'config_advanced_search_sort_by' => array(
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_SORT_BY_CONFIG_ID,
      'config_name'  => "config_advanced_search_sort_by",
      'config_value'   => $form_state['values']['sort_by'],
    ),
    'config_advanced_search_match_mode' => array(
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_MATCH_MODE_CONFIG_ID,
      'config_name'  => "config_advanced_search_match_mode",
      'config_value'   => $form_state['values']['match_mode'],
    ),
    'config_advanced_search_document_type' => array(
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_DOCUMENT_TYPE_CONFIG_ID,
      'config_name'  => "config_advanced_search_document_type",
      'config_value'   => $form_state['values']['document_type'],
    ),
    'config_advanced_search_site_url' => array(
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_SITE_URL_CONFIG_ID,
      'config_name'  => "config_advanced_search_site_url",
      'config_value'   => $form_state['values']['site_url'],
    ),
    'config_advanced_search_group_by_site' => array(
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_GROUP_BY_SITE_CONFIG_ID,
      'config_name'  => "config_advanced_search_group_by_site",
      'config_value'   => $form_state['values']['group_by_site'],
    ),
    'config_advanced_search_filter_by_date' => array(
      'config_id' => VERTICRAWL_SEARCH_ADVANCED_FILTER_BY_DATE_CONFIG_ID,
      'config_name'  => "config_advanced_search_filter_by_date",
      'config_value'   => $form_state['values']['filter_by_date'],
    ),
    // POWERED BY.
    'config_display_powered_by' => array(
      'config_id' => VERTICRAWL_SEARCH_POWERED_BY,
      'config_name'  => "display_powered_by",
      'config_value'   => $form_state['values']['display_powered_by'],
    ),
  );

  foreach ($configs as $config) {
    VerticrawlHelper::saveConfig($config);
  }

  drupal_flush_all_caches();
}
