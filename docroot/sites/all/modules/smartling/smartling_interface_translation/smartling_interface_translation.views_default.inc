<?php
/**
 * @file
 * smartling_interface_translation.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function smartling_interface_translation_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'smartling_bulk_submit_interface';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'smartling_interface_entity';
  $view->human_name = 'Smartling Bulk Submit Interface';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Smartling Bulk Submit Interface';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'translate interface';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Smartling: Smartling Entity */
  $handler->display->display_options['relationships']['smartling_entity_data']['id'] = 'smartling_entity_data';
  $handler->display->display_options['relationships']['smartling_entity_data']['table'] = 'smartling_interface_entity';
  $handler->display->display_options['relationships']['smartling_entity_data']['field'] = 'smartling_entity_data';
  /* Field: Bulk operations: Smartling Interface Entity */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'smartling_interface_entity';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Smartling translate';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::smartling_do_translate_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Translate',
    ),
  );
  /* Field: Smartling Interface Entity: Textgroup */
  $handler->display->display_options['fields']['textgroup']['id'] = 'textgroup';
  $handler->display->display_options['fields']['textgroup']['table'] = 'smartling_interface_entity';
  $handler->display->display_options['fields']['textgroup']['field'] = 'textgroup';
  $handler->display->display_options['fields']['textgroup']['exclude'] = TRUE;
  /* Field: Smartling Interface Entity: Label */
  $handler->display->display_options['fields']['textgroup_name']['id'] = 'textgroup_name';
  $handler->display->display_options['fields']['textgroup_name']['table'] = 'smartling_interface_entity';
  $handler->display->display_options['fields']['textgroup_name']['field'] = 'textgroup_name';
  $handler->display->display_options['fields']['textgroup_name']['label'] = 'Text group';
  /* Field: Smartling: Locales for interface entity */
  $handler->display->display_options['fields']['locale_info']['id'] = 'locale_info';
  $handler->display->display_options['fields']['locale_info']['table'] = 'smartling_interface_entity';
  $handler->display->display_options['fields']['locale_info']['field'] = 'locale_info';
  $handler->display->display_options['fields']['locale_info']['label'] = 'Locales';
  /* Filter criterion: Smartling Entity Data: Smartling status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'smartling_entity_data';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'smartling_entity_data';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/smartling-content/interface';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Translation interface';
  $handler->display->display_options['menu']['weight'] = '100';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['smartling_bulk_submit_interface'] = array(
    t('Master'),
    t('Smartling Bulk Submit Interface'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Smartling related ID'),
    t('Smartling translate'),
    t('- Choose an operation -'),
    t('Translate'),
    t('Textgroup'),
    t('Text group'),
    t('Locales'),
    t('Status'),
    t('Page'),
  );
  $export['smartling_bulk_submit_interface'] = $view;

  return $export;
}
