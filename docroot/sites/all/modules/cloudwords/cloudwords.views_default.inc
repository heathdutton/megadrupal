<?php

/**
 * Implements hook_views_default_views().
 */
function cloudwords_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cloudwords_translatable';
  $view->description = 'Cloudwords content view';
  $view->tag = 'default';
  $view->base_table = 'cloudwords_translatable';
  $view->human_name = 'Cloudwords';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Cloudwords queue';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'label' => 'label',
    'status' => 'status',
    'language' => 'language',
    'textgroup' => 'textgroup',
    'type' => 'type',
    'objectid' => 'objectid',
    'translation_status' => 'translation_status',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'language' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'textgroup' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'objectid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'translation_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Cloudwords translatable: Content type */
  $handler->display->display_options['relationships']['content_type']['id'] = 'content_type';
  $handler->display->display_options['relationships']['content_type']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['relationships']['content_type']['field'] = 'content_type';
  /* Relationship: Cloudwords translatable: Vocabulary */
  $handler->display->display_options['relationships']['vocabulary']['id'] = 'vocabulary';
  $handler->display->display_options['relationships']['vocabulary']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['relationships']['vocabulary']['field'] = 'vocabulary';
  /* Field: Bulk operations: Cloudwords translatable */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::cloudwords_project_add' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::cloudwords_add_to_queue' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::cloudwords_project_create' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Remove from queue',
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
    'action::cloudwords_project_remove' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::cloudwords_queue_remove_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Cloudwords translatable: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Source name';
  /* Field: Cloudwords translatable: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Marked for Translation';
  /* Field: Cloudwords translatable: Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  /* Field: Cloudwords translatable: Textgroup */
  $handler->display->display_options['fields']['textgroup']['id'] = 'textgroup';
  $handler->display->display_options['fields']['textgroup']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['textgroup']['field'] = 'textgroup';
  $handler->display->display_options['fields']['textgroup']['label'] = 'Group';
  /* Field: Cloudwords translatable: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Cloudwords translatable: Objectid */
  $handler->display->display_options['fields']['objectid']['id'] = 'objectid';
  $handler->display->display_options['fields']['objectid']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['objectid']['field'] = 'objectid';
  $handler->display->display_options['fields']['objectid']['label'] = '';
  $handler->display->display_options['fields']['objectid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['objectid']['element_label_colon'] = FALSE;
  /* Field: Cloudwords translatable: Translation_status */
  $handler->display->display_options['fields']['translation_status']['id'] = 'translation_status';
  $handler->display->display_options['fields']['translation_status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['translation_status']['field'] = 'translation_status';
  $handler->display->display_options['fields']['translation_status']['label'] = 'Translation status';
  /* Filter criterion: Cloudwords translatable: Label */
  $handler->display->display_options['filters']['label']['id'] = 'label';
  $handler->display->display_options['filters']['label']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['label']['field'] = 'label';
  $handler->display->display_options['filters']['label']['operator'] = 'contains';
  $handler->display->display_options['filters']['label']['group'] = 1;
  $handler->display->display_options['filters']['label']['exposed'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator_id'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['label'] = 'Source name';
  $handler->display->display_options['filters']['label']['expose']['operator'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['identifier'] = 'label';
  $handler->display->display_options['filters']['label']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Marked for Translation';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['status']['expose']['reduce'] = TRUE;
  /* Filter criterion: Cloudwords translatable: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['group'] = 1;
  $handler->display->display_options['filters']['language']['exposed'] = TRUE;
  $handler->display->display_options['filters']['language']['expose']['operator_id'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['label'] = 'Language';
  $handler->display->display_options['filters']['language']['expose']['operator'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['identifier'] = 'language';
  $handler->display->display_options['filters']['language']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Textgroup */
  $handler->display->display_options['filters']['textgroup']['id'] = 'textgroup';
  $handler->display->display_options['filters']['textgroup']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['textgroup']['field'] = 'textgroup';
  $handler->display->display_options['filters']['textgroup']['group'] = 1;
  $handler->display->display_options['filters']['textgroup']['exposed'] = TRUE;
  $handler->display->display_options['filters']['textgroup']['expose']['operator_id'] = 'textgroup_op';
  $handler->display->display_options['filters']['textgroup']['expose']['label'] = 'Group';
  $handler->display->display_options['filters']['textgroup']['expose']['operator'] = 'textgroup_op';
  $handler->display->display_options['filters']['textgroup']['expose']['identifier'] = 'textgroup';
  $handler->display->display_options['filters']['textgroup']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['textgroup']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'content_type';
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Content type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Taxonomy term: Vocabulary */
  $handler->display->display_options['filters']['vid']['id'] = 'vid';
  $handler->display->display_options['filters']['vid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['vid']['field'] = 'vid';
  $handler->display->display_options['filters']['vid']['relationship'] = 'vocabulary';
  $handler->display->display_options['filters']['vid']['group'] = 1;
  $handler->display->display_options['filters']['vid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['vid']['expose']['operator_id'] = 'vid_op';
  $handler->display->display_options['filters']['vid']['expose']['label'] = 'Vocabulary';
  $handler->display->display_options['filters']['vid']['expose']['operator'] = 'vid_op';
  $handler->display->display_options['filters']['vid']['expose']['identifier'] = 'vid';
  $handler->display->display_options['filters']['vid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['vid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Translation_status */
  $handler->display->display_options['filters']['translation_status']['id'] = 'translation_status';
  $handler->display->display_options['filters']['translation_status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['translation_status']['field'] = 'translation_status';
  $handler->display->display_options['filters']['translation_status']['group'] = 1;
  $handler->display->display_options['filters']['translation_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['translation_status']['expose']['operator_id'] = 'translation_status_op';
  $handler->display->display_options['filters']['translation_status']['expose']['label'] = 'Translation status';
  $handler->display->display_options['filters']['translation_status']['expose']['operator'] = 'translation_status_op';
  $handler->display->display_options['filters']['translation_status']['expose']['identifier'] = 'translation_status';
  $handler->display->display_options['filters']['translation_status']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['translation_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: In translation */
  $handler->display->display_options['filters']['in_translation']['id'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['in_translation']['field'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['exposed'] = TRUE;
  $handler->display->display_options['filters']['in_translation']['expose']['operator_id'] = 'in_translation_op';
  $handler->display->display_options['filters']['in_translation']['expose']['label'] = 'In translation';
  $handler->display->display_options['filters']['in_translation']['expose']['operator'] = 'in_translation_op';
  $handler->display->display_options['filters']['in_translation']['expose']['identifier'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['in_translation']['group_info']['label'] = 'In translation';
  $handler->display->display_options['filters']['in_translation']['group_info']['identifier'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['group_info']['remember'] = 1;
  $handler->display->display_options['filters']['in_translation']['group_info']['default_group'] = '2';
  $handler->display->display_options['filters']['in_translation']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '2',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => 'No',
      'operator' => '<',
      'value' => array(
        'value' => '2',
        'min' => '',
        'max' => '',
      ),
    ),
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Cloudwords translatable */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '1000';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::cloudwords_project_add' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::cloudwords_add_to_queue' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::cloudwords_project_create' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Remove',
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
    'action::cloudwords_project_remove' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::cloudwords_queue_remove_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Cloudwords translatable: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Source name';
  /* Field: Cloudwords translatable: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Marked for Translation';
  /* Field: Cloudwords translatable: Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  /* Field: Cloudwords translatable: Textgroup */
  $handler->display->display_options['fields']['textgroup']['id'] = 'textgroup';
  $handler->display->display_options['fields']['textgroup']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['textgroup']['field'] = 'textgroup';
  $handler->display->display_options['fields']['textgroup']['label'] = 'Group';
  /* Field: Cloudwords translatable: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Cloudwords translatable: Objectid */
  $handler->display->display_options['fields']['objectid']['id'] = 'objectid';
  $handler->display->display_options['fields']['objectid']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['objectid']['field'] = 'objectid';
  $handler->display->display_options['fields']['objectid']['label'] = '';
  $handler->display->display_options['fields']['objectid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['objectid']['element_label_colon'] = FALSE;
  /* Field: Cloudwords translatable: Translation_status */
  $handler->display->display_options['fields']['translation_status']['id'] = 'translation_status';
  $handler->display->display_options['fields']['translation_status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['fields']['translation_status']['field'] = 'translation_status';
  $handler->display->display_options['fields']['translation_status']['label'] = 'Translation status';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Cloudwords translatable: Label */
  $handler->display->display_options['filters']['label']['id'] = 'label';
  $handler->display->display_options['filters']['label']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['label']['field'] = 'label';
  $handler->display->display_options['filters']['label']['operator'] = 'contains';
  $handler->display->display_options['filters']['label']['group'] = 1;
  $handler->display->display_options['filters']['label']['exposed'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator_id'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['label'] = 'Source name';
  $handler->display->display_options['filters']['label']['expose']['operator'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['identifier'] = 'label';
  $handler->display->display_options['filters']['label']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Marked for Translation';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['status']['expose']['reduce'] = TRUE;
  /* Filter criterion: Cloudwords translatable: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['group'] = 1;
  $handler->display->display_options['filters']['language']['exposed'] = TRUE;
  $handler->display->display_options['filters']['language']['expose']['operator_id'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['label'] = 'Language';
  $handler->display->display_options['filters']['language']['expose']['operator'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['identifier'] = 'language';
  $handler->display->display_options['filters']['language']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Textgroup */
  $handler->display->display_options['filters']['textgroup']['id'] = 'textgroup';
  $handler->display->display_options['filters']['textgroup']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['textgroup']['field'] = 'textgroup';
  $handler->display->display_options['filters']['textgroup']['group'] = 1;
  $handler->display->display_options['filters']['textgroup']['exposed'] = TRUE;
  $handler->display->display_options['filters']['textgroup']['expose']['operator_id'] = 'textgroup_op';
  $handler->display->display_options['filters']['textgroup']['expose']['label'] = 'Group';
  $handler->display->display_options['filters']['textgroup']['expose']['operator'] = 'textgroup_op';
  $handler->display->display_options['filters']['textgroup']['expose']['identifier'] = 'textgroup';
  $handler->display->display_options['filters']['textgroup']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['textgroup']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'content_type';
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Content type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Taxonomy term: Vocabulary */
  $handler->display->display_options['filters']['vid']['id'] = 'vid';
  $handler->display->display_options['filters']['vid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['vid']['field'] = 'vid';
  $handler->display->display_options['filters']['vid']['relationship'] = 'vocabulary';
  $handler->display->display_options['filters']['vid']['group'] = 1;
  $handler->display->display_options['filters']['vid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['vid']['expose']['operator_id'] = 'vid_op';
  $handler->display->display_options['filters']['vid']['expose']['label'] = 'Vocabulary';
  $handler->display->display_options['filters']['vid']['expose']['operator'] = 'vid_op';
  $handler->display->display_options['filters']['vid']['expose']['identifier'] = 'vid';
  $handler->display->display_options['filters']['vid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['vid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Added to project';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: Translation_status */
  $handler->display->display_options['filters']['translation_status']['id'] = 'translation_status';
  $handler->display->display_options['filters']['translation_status']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['translation_status']['field'] = 'translation_status';
  $handler->display->display_options['filters']['translation_status']['group'] = 1;
  $handler->display->display_options['filters']['translation_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['translation_status']['expose']['operator_id'] = 'translation_status_op';
  $handler->display->display_options['filters']['translation_status']['expose']['label'] = 'Translation status';
  $handler->display->display_options['filters']['translation_status']['expose']['operator'] = 'translation_status_op';
  $handler->display->display_options['filters']['translation_status']['expose']['identifier'] = 'translation_status';
  $handler->display->display_options['filters']['translation_status']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['translation_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Cloudwords translatable: In translation */
  $handler->display->display_options['filters']['in_translation']['id'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['table'] = 'cloudwords_translatable';
  $handler->display->display_options['filters']['in_translation']['field'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['exposed'] = TRUE;
  $handler->display->display_options['filters']['in_translation']['expose']['operator_id'] = 'in_translation_op';
  $handler->display->display_options['filters']['in_translation']['expose']['label'] = 'In translation';
  $handler->display->display_options['filters']['in_translation']['expose']['operator'] = 'in_translation_op';
  $handler->display->display_options['filters']['in_translation']['expose']['identifier'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['in_translation']['group_info']['label'] = 'In translation';
  $handler->display->display_options['filters']['in_translation']['group_info']['identifier'] = 'in_translation';
  $handler->display->display_options['filters']['in_translation']['group_info']['remember'] = 1;
  $handler->display->display_options['filters']['in_translation']['group_info']['default_group'] = '2';
  $handler->display->display_options['filters']['in_translation']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '2',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => 'No',
      'operator' => '<',
      'value' => array(
        'value' => '2',
        'min' => '',
        'max' => '',
      ),
    ),
  );

  /* Display: Block 2 */
  $handler = $view->new_display('block', 'Block 2', 'block_2');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $translatables['cloudwords_translatable'] = array(
    t('Master'),
    t('Cloudwords queue'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('content type'),
    t('Vocabulary'),
    t('Source name'),
    t('Marked for Translation'),
    t('Language'),
    t('Group'),
    t('Type'),
    t('Translation status'),
    t('Content type'),
    t('In translation'),
    t('Block'),
    t('Added to project'),
    t('Block 2'),
  );

  $export[$view->name] = $view;

  return $export;
}
