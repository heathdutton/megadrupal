<?php
/**
 * @file
 * Code for administrative menu callbacks.
 */

define('VARIABLE_DEBUG_LARGE_LIMIT', 10);

/**
 * Menu callback form for admin/reports/variable-debug
 */
function variable_debug_report($form, &$form_state) {
  // Information fieldset.
  $item = db_query('SELECT count(name) AS count, SUM(LENGTH(value)) AS total FROM {variable}')->fetchObject();
  $form['information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Information'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['information']['information_markup'] = array(
    '#markup' => '<p>' . t('There are %count variables in the variable table, using total space of %size.', array(
      '%count' => number_format($item->count),
      '%size' => format_size($item->total),
    )) . '</p>',
  );

  // Large variables.
  $header = array(
    array('data' => t('Variable name')),
    array('data' => t('Size'), 'title' => t('This is the raw size in the database, when un-serialized in memory, it will be larger.')),
    array('data' => t('Drupal.org issue'), 'title' => t('If there is a known Drupal.org issue about this particular variable it is shown here. You may wish to consider patching if the variable is large enough.')),
  );

  $form['large'] = array(
    '#type' => 'fieldset',
    '#title' => t('Large variables'),
    '#description' => t('Here are the @count largest variables in the variable table. These should be candidates for porting to a custom table (as to save loading them into memory on each page request). If you know of a drupal.org issue that matches a known problem variable, please raise an issue on the <a href="!url">project page</a>.', array(
      '@count' => VARIABLE_DEBUG_LARGE_LIMIT,
      '!url' => url('http://drupal.org/node/add/project-issue/1994074'),
    )),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['large']['large_results'] = array(
    '#theme' =>'table',
    '#header' => $header,
    '#rows' => variable_debug_get_large_rows(),
    '#empty' => t('No variables available.'),
  );


  // Abandoned variables, these are variables that are not defined in any
  // active contrib modules.
  $header = array(
    'name' => t('Variable name'),
    'size' => t('Size'),
  );

  $form['abandoned'] = array(
    '#type' => 'fieldset',
    '#title' => t('(Potentially) abandoned variables'),
    '#description' => t('Here are variables that are potentially abandoned. They are not prefixed with any enabled contrib modules, and are not known core variables. These could either be abandoned variables (left over from previous contributed modules), or poorly namespaced variables from existing contributed modules. Only delete these variables if you are sure they should not be there.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['abandoned']['abandoned_results'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => variable_debug_get_abandoned_rows(),
    '#empty' => t('No variables available.'),
  );
  $form['abandoned']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected variables'),
  );

  return $form;
}

/**
 * Submit handler for the report page, that is responsible for deleting the
 * selected variables and displaying a confirmation message.
 */
function variable_debug_report_submit($form, &$form_state) {
  $variables_to_delete = array_keys(array_filter($form_state['values']['abandoned_results']));
  if (empty($variables_to_delete)) {
    drupal_set_message(t('No variables selected, nothing to delete.'));
  }
  else {
    foreach ($variables_to_delete as $variable) {
      variable_del($variable);
    }
    drupal_set_message(format_plural(count($variables_to_delete), t('Deleted the variable %name.'), t('Deleted the selected @count variables.'), array(
      '%name' => array_pop($variables_to_delete),
    )));
  }
}

/**
 * Select the the items in the variable table that use the most amount of space.
 */
function variable_debug_get_large_rows($count = VARIABLE_DEBUG_LARGE_LIMIT, $show_links = TRUE) {
  $query = db_select('variable', 'v');
  $query->addField('v', 'name');
  $query->addExpression('LENGTH(v.value)', 'length');
  $query->orderBy('length', 'desc');
  $query->range(0, $count);
  $items = $query->execute()->fetchAllAssoc('name');

  $rows = array();
  foreach ($items as $name => $item) {
    $link = variable_debug_get_drupal_org_issue($item->name);
    $rows[] = array($item->name, format_size($item->length), $show_links ? $link : strip_tags($link));
  }

  return $rows;
}

/**
 * Abandoned variables, these are variables that are not defined in any active
 * contrib modules.
 */
function variable_debug_get_abandoned_rows() {
  $query = db_select('variable','v')
    ->fields('v', array('name'))
    ->orderBy('name', 'ASC');
  $query->addExpression('LENGTH(v.value)', 'length');
  // Exclude active contributed modules.
  $and = db_and();
  foreach (module_list() as $module) {
    $and->condition('v.name', $module . '_%', 'NOT LIKE');
  }
  $query->condition($and);
  // Exclude known core variables.
  $and = db_and();
  foreach (variable_debug_get_core_variables() as $variable) {
    $and->condition('v.name', $variable, 'NOT LIKE');
  }
  $query->condition($and);
  $items = $query->execute()->fetchAllAssoc('name');

  $options = array();
  foreach ($items as $name => $item) {
    $options[$item->name] = array(
      'name' => $item->name,
      'size' => format_size($item->length),
    );
  }

  return $options;
}

/**
 * Provides a link between known large variables and Drupal.org issues.
 */
function variable_debug_get_drupal_org_issue($variable) {
  switch ($variable) {
    case 'features_codecache':
      return l('#1325288', 'https://www.drupal.org/node/1325288');
    case 'imagefield_crop_info':
      return l('#1993144', 'https://www.drupal.org/node/1993144');
    case 'advagg_bundler_info':
      return l('#2479755', 'https://www.drupal.org/node/2479755');
    default:
      return t('');
  }
}

/**
 * Lists all core variables that are known.
 */
function variable_debug_get_core_variables() {
  // This is generated from a nice bash command, running over a fresh Drupal
  // code base checkout. Some variables have been made dynamic with a wildcard.
  // grep -r "variable_[g|s]et('" * | sed "s/.*variable_[g|s]et('\([a-zA-Z0-9_]*\)'.*/'\1',/" | sort | uniq
  return array(
    '404_fast_html',
    '404_fast_paths',
    '404_fast_paths_exclude',
    'actions_max_stack',
    'admin_compact_mode',
    'admin_theme',
    'aggregator_allowed_html_tags',
    'aggregator_category_selector',
    'aggregator_clear',
    'aggregator_fetcher',
    'aggregator_parser',
    'aggregator_processors',
    'aggregator_summary_items',
    'aggregator_teaser_length',
    'allow_authorize_operations',
    'allow_insecure_uploads',
    'anonymous',
    'authorize_filetransfer_connection_settings_%',
    'authorize_filetransfer_default',
    'batch_test_stack',
    'block_cache',
    'blocked_ips',
    'block_test_caching',
    'block_test_content',
    'blog_block_count',
    'book_allowed_types',
    'book_block_mode',
    'book_child_type',
    'cache',
    'cache_backends',
    'cache_class_%',
    'cache_clear_threshold',
    'cache_default_class',
    'cache_flush_%',
    'cache_flush',
    'cache_lifetime',
    'clean_url',
    'color_%',
    'comment_%',
    'common_test_cron',
    'configurable_timezones',
    'contact_default_status',
    'contact_form_information',
    'contact_hourly_threshold',
    'contact_threshold_limit',
    'contact_threshold_window',
    'cron_key',
    'cron_last',
    'cron_safe_threshold',
    'cron_semaphore',
    'cron_threshold_error',
    'cron_threshold_warning',
    'css_js_query_string',
    'dashboard_stashed_blocks',
    'date_default_timezone',
    'date_default_timezone_id',
    'date_default_timezone_name',
    'date_first_day',
    'date_format_%',
    'date_temporary_timezone',
    'dblog_row_limit',
    'default_nodes_main',
    'dependencies',
    'dependency_test',
    'drupal_css_cache_files',
    'drupal_http_request_fails',
    'drupal_http_request_function',
    'drupal_js_cache_files',
    'drupal_js_version_query_string',
    'drupal_private_key',
    'drupal_stale_file_threshold',
    'drupal_test_email_collector',
    'drupal_weight_select_max',
    'empty_timezone_message',
    'entity_cache_test',
    'entity_cache_test_label',
    'error_level',
    'example_restricted_roles',
    'feed_default_items',
    'feed_description',
    'feed_item_length',
    'field_bundle_settings_%',
    'field_bundle_settings',
    'field_extra_weights',
    'field_purge_batch_size',
    'field_storage_default',
    'field_test_bundles',
    'field_test_clear_info_cache_in_hook_entity_info',
    'field_test_field_available_languages_alter',
    'field_test_language_fallback',
    'field_test_storage_data',
    'file_chmod_directory',
    'file_chmod_file',
    'file_default_scheme',
    'file_description_length',
    'file_description_type',
    'file_directory_path',
    'file_directory_temp',
    'file_downloads',
    'file_icon_directory',
    'file_private_path',
    'file_public_path',
    'file_temporary_path',
    'file_test_hook_file_url_alter',
    'file_test_results',
    'file_test_return',
    'filter_allowed_protocols',
    'filter_default_format',
    'filter_fallback_format',
    'filter_html_%',
    'forum_block_num_%',
    'forum_containers',
    'forum_hot_topic',
    'forum_nav_vocabulary',
    'forum_order',
    'forum_per_page',
    'front_page_output',
    'https',
    'image_allow_insecure_derivatives',
    'image_jpeg_quality',
    'image_module_test_file_download',
    'image_style_preview_image',
    'image_test_results',
    'image_toolkit',
    'install_current_batch',
    'install_locale_batch_components',
    'install_profile',
    'install_profile_modules',
    'install_task',
    'install_time',
    'javascript_parsed',
    'language_content_type_%',
    'language_count',
    'language_default',
    'language_native_enabled',
    'language_negotiation_%',
    'language_negotiation',
    'language_types',
    'locale_%',
    'lock_inc',
    'lousy_module_conflicting_variable',
    'mac_key',
    'mail_line_endings',
    'mail_system',
    'maintenance_mode',
    'maintenance_mode_message',
    'maximum_replication_lag',
    'menu_default_active_menus',
    'menu_default_node_menu',
    'menu_expanded',
    'menu_inc',
    'menu_main_links_source',
    'menu_masks',
    'menu_options_%',
    'menu_override_parent_selector',
    'menu_parent_%',
    'menu_primary_links_source',
    'menu_rebuild_needed',
    'menu_secondary_links_source',
    'menu_test_active_trail_final',
    'menu_test_active_trail_initial',
    'menu_test_hook_custom_theme_name',
    'menu_test_menu_tree_set_path',
    'menu_test_record_active_trail',
    'minimum_word_size',
    'mymodule_cron_last_run',
    'mymodule_image_style',
    'my_module_menus',
    'mymodule_num_per_page',
    'mymodule_shortcut_admin_default_set',
    'myprofile_needs_batch_processing',
    'node_access_needs_rebuild',
    'node_access_test_private',
    'node_admin_theme',
    'node_cron_comments_scale',
    'node_cron_last',
    'node_cron_views_scale',
    'node_options_%',
    'node_permissions_%',
    'node_preview_%',
    'node_preview',
    'node_rank_%',
    'node_recent_block_count',
    'node_submitted_%',
    'node_test_change_view_mode',
    'node_test_node_access_all_uid',
    'omit_vary_cookie',
    'openid_%',
    'overlap_cjk',
    'page_cache_invoke_hooks',
    'page_cache_maximum_age',
    'page_cache_without_database',
    'page_compression',
    'paranoia',
    'password_count_log2',
    'password_inc',
    'path_alias_whitelist',
    'path_inc',
    'path_test_results',
    'preprocess_css',
    'preprocess_js',
    'profile_block_author_fields',
    'proxy_exceptions',
    'proxy_password',
    'proxy_port',
    'proxy_server',
    'proxy_user_agent',
    'proxy_username',
    'queue_class_%',
    'queue_default_class',
    'queue_default_reliable_class',
    'reverse_proxy',
    'reverse_proxy_addresses',
    'reverse_proxy_header',
    'sample_search_force_keywords',
    'search_active_modules',
    'search_and_or_limit',
    'search_cron_limit',
    'search_default_module',
    'search_embedded_form_submitted',
    'search_tag_weights',
    'session_inc',
    'session_write_interval',
    'shortcut_max_slots',
    'simpletest_%',
    'site_403',
    'site_404',
    'site_default_country',
    'site_footer',
    'site_frontpage',
    'site_mail',
    'site_mission',
    'site_name',
    'site_offline',
    'site_offline_message',
    'site_slogan',
    'statistics_block_top_all_num',
    'statistics_block_top_day_num',
    'statistics_block_top_last_num',
    'statistics_count_content_views',
    'statistics_day_timestamp',
    'statistics_enable_access_log',
    'statistics_flush_accesslog_timer',
    'stream_public_path',
    'syslog_facility',
    'syslog_format',
    'syslog_identity',
    'taxonomy_%',
    'teaser_length_%',
    'teaser_length',
    'test_module_disable_order',
    'test_module_enable_order',
    'test_module_uninstall_order',
    'test_verbose_module_hooks',
    'theme_%',
    'tracker_batch_size',
    'tracker_index_nid',
    'translation_language_type',
    'trigger_test_generic_action',
    'trigger_test_generic_any_action',
    'trigger_test_system_cron_action',
    'trigger_test_system_cron_conf_action',
    'update_advanced_project_settings',
    'update_check_disabled',
    'update_check_frequency',
    'update_d6',
    'update_d7_requirements',
    'update_fetch_url',
    'update_last_check',
    'update_last_email_notification',
    'update_max_fetch_attempts',
    'update_max_fetch_time',
    'update_notification_threshold',
    'update_notify_emails',
    'update_script_test_requirement_type',
    'update_test_system_info',
    'update_test_update_status',
    'update_test_xml_map',
    'upload_%',
    'user_admin_role',
    'user_block_max_list_count',
    'user_block_seconds_online',
    'user_block_whois_new_count',
    'user_cancel_method',
    'user_default_timezone',
    'user_email_verification',
    'user_failed_login_%',
    'user_mail_%',
    'user_password_reset_timeout',
    'user_picture_%',
    'user_pictures',
    'user_register',
    'user_registration_help',
    'user_signatures',
    'vote_node_enabled',
    'vote_score_max',
    'xmlrpc_test_xmlrpc_alter',
    'xri_proxy_resolver',
  );
}
