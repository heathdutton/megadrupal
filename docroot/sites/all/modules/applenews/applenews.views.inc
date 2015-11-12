<?php
/**
 * @file
 * Views hook implementations.
 */

/**
 * Implements hook_views_api().
*/
function applenews_views_api() {
  return [
    'api' => 3,
  ];
}

/**
 * Implements hook_views_data().
 */
function applenews_views_data() {
  module_load_include('inc', 'applenews');

  // Apple News post information.
  $data['applenews_entities']['table']['group'] = t('Apple News Article');

  $data['applenews_entities']['table']['base'] = [
    'field' => 'entity_id',
    'title' => t('Apple News'),
    'help' => t('A list of Apple News published articles.'),
    'weight' => 10,
  ];

  $data['applenews_entities']['post_id'] = [
    'title' => t('Post ID'),
    'help' => t('Apple News post ID'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
    ],
  ];

  $entity_types = ['node'];
  $fields = ['entity_id', 'revision_id'];

  // Add relationship to entity base via entity_id and revision_id fields.
  foreach ($entity_types as $entity_type) {
    $entity_info = entity_get_info($entity_type);
    foreach ($fields as $field_name) {

      if ($field_name == 'entity_id') {
        $data['applenews_entities']['table']['join'][$entity_info['base table']] = [
          'left_field' => $entity_info['entity keys']['id'],
          'field' => $field_name,
          'type' => 'LEFT'
        ];
      }

      $data['applenews_entities']['reverse_' . $field_name . '_' . $entity_type]['relationship'] = [
        'handler' => 'views_handler_relationship_entity_reverse',
        'field_name' => $field_name,
        'field table' => 'applenews_entities',
        'field field' => $field_name,
        'base' => $entity_info['base table'],
        'base field' => ($field_name == 'entity_id') ? $entity_info['entity keys']['id'] : $entity_info['entity keys']['revision'],
        'label' => t('Apple News articles referencing Drupal entities'),
        'group' => t('Apple News'),
        'title' => t('Apple News Referencing !field_name', ['!field_name' => $field_name]),
        'help' => t('A bridge to the @entity_type entity that is referencing Apple News article via !field_name',
          [
            '@entity_type' => $entity_type,
            '!field_name' => $field_name,
          ]
        ),
        'join_extra' => [
          0 => [
            'field' => 'entity_type',
            'value' => $entity_type,
          ],
        ],
      ];

    }
  }

  $data['applenews_entities']['article_id'] = [
    'title' => t('Article ID'),
    'help' => t('Apple News Article ID'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  $data['applenews_entities']['article_revision_id'] = [
    'title' => t('Revision ID'),
    'help' => t('Apple News Article Revision ID'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  $data['applenews_entities']['share_url'] = [
    'title' => t('Share URL'),
    'help' => t('Apple News Article Share URL'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  $data['applenews_entities']['postdate'] = [
    'title' => t('Post date'),
    'help' => t('Apple News post timestamp'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort_date',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_date',
    ],
  ];

  // Custom filter. Apple News article publish status.
  $data['applenews_entities']['views_handler_filter_applenews_status'] = [
    'title' => t('Entity is published to Apple News'),
    'help' => t('Apple News Article Status'),
    'group' => t('Apple News'),
    'real field' => 'article_id',
    'filter' => [
      'handler' => 'views_handler_filter_applenews_status',
    ],
  ];

  // Custom field. Apple News article publish status.
  $data['applenews_entities']['views_handler_field_applenews_status'] = [
    'title' => t('Entity is published to Apple News'),
    'help' => t('Apple News Article Status'),
    'group' => t('Apple News'),
    'real field' => 'article_id',
    'field' => [
      'handler' => 'views_handler_field_applenews_status',
      'click sortable' => TRUE,
    ],
  ];

  // Custom field. Apple News article download previewable link.
  $data['applenews_entities']['views_handler_field_applenews_previewable'] = [
    'title' => t('Download Previewable File'),
    'help' => t('Apple News Download Previewable Files (use News Preview app to preview article files).'),
    'group' => t('Apple News'),
    'real field' => 'article_id',
    'field' => [
      'handler' => 'views_handler_field_applenews_previewable',
    ],
  ];

  // Metadata table.
  $data['applenews_metadata']['table']['join'] = [
    'applenews_entities' => [
      'left_field' => 'post_id',
      'field' => 'post_id'
    ]
  ];

  $data['applenews_metadata']['post_id'] = [
    'title' => t('Post ID'),
    'help' => t('Apple News Post ID  (metadata)'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field_serialized',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
    ],
  ];

  $data['applenews_metadata']['data'] = [
    'title' => t('Metadata'),
    'help' => t('Apple News metadata'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field_serialized',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
    ],
  ];

  // Channels table.
  $data['applenews_channels']['table']['join'] = [
    'applenews_entities' => [
      'left_field' => 'post_id',
      'field' => 'post_id'
    ]
  ];

  $data['applenews_channels']['post_id'] = [
    'title' => t('Post ID'),
    'help' => t('Apple News Post ID (channels)'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
    ],
  ];

  $data['applenews_channels']['channel_id'] = [
    'title' => t('Channel ID'),
    'help' => t('Apple News Channel ID (channels)'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  // Sections table
  $data['applenews_sections']['table']['join'] = [
    'applenews_entities' => [
      'left_field' => 'post_id',
      'field' => 'post_id'
    ]
  ];

  $data['applenews_sections']['post_id'] = [
    'title' => t('Post ID'),
    'help' => t('Apple News Post ID'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
    ],
  ];

  $data['applenews_sections']['channel_id'] = [
    'title' => t('Channel ID'),
    'help' => t('Apple News Channel ID (sections)'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  $data['applenews_sections']['section_id'] = [
    'title' => t('Section ID'),
    'help' => t('Apple News Channel ID (sections)'),
    'group' => t('Apple News'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  return $data;

}

/**
 * Implements hook_views_default_views().
 */
function applenews_views_default_views() {
  $view = new view();
  $view->name = 'applenews_content';
  $view->description = 'Apple News published content';
  $view->tag = 'Apple News';
  $view->base_table = 'node';
  $view->human_name = 'Apple News Content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Content';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer apple news';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'type' => 'type',
    'status' => 'status',
    'changed' => 'changed',
    'views_handler_field_applenews_status' => 'views_handler_field_applenews_status',
    'postdate' => 'postdate',
    'share_url' => 'share_url',
    'delete_node' => 'delete_node',
    'edit_node' => 'edit_node',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
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
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'views_handler_field_applenews_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'postdate' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'share_url' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no Apple News published articles.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Apple News: Apple News Referencing entity_id */
  $handler->display->display_options['relationships']['reverse_entity_id_node']['id'] = 'reverse_entity_id_node';
  $handler->display->display_options['relationships']['reverse_entity_id_node']['table'] = 'applenews_entities';
  $handler->display->display_options['relationships']['reverse_entity_id_node']['field'] = 'reverse_entity_id_node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Created/Updated date';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: Apple News: Entity is published to Apple News */
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['id'] = 'views_handler_field_applenews_status';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['table'] = 'applenews_entities';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['field'] = 'views_handler_field_applenews_status';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['label'] = 'Apple News Status';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['type'] = 'custom';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['type_custom_true'] = 'Published';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['type_custom_false'] = 'Not published';
  $handler->display->display_options['fields']['views_handler_field_applenews_status']['not'] = 0;
  /* Field: Apple News: Post date */
  $handler->display->display_options['fields']['postdate']['id'] = 'postdate';
  $handler->display->display_options['fields']['postdate']['table'] = 'applenews_entities';
  $handler->display->display_options['fields']['postdate']['field'] = 'postdate';
  $handler->display->display_options['fields']['postdate']['label'] = 'Apple News post date';
  $handler->display->display_options['fields']['postdate']['date_format'] = 'short';
  $handler->display->display_options['fields']['postdate']['second_date_format'] = 'long';
  /* Field: Apple News: Share URL */
  $handler->display->display_options['fields']['share_url']['id'] = 'share_url';
  $handler->display->display_options['fields']['share_url']['table'] = 'applenews_entities';
  $handler->display->display_options['fields']['share_url']['field'] = 'share_url';
  $handler->display->display_options['fields']['share_url']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['share_url']['alter']['path'] = '[share_url]';
  $handler->display->display_options['fields']['share_url']['alter']['external'] = TRUE;
  $handler->display->display_options['fields']['share_url']['alter']['target'] = '_blank';
  /* Field: Apple News: Download Previewable File */
  $handler->display->display_options['fields']['views_handler_field_applenews_previewable']['id'] = 'views_handler_field_applenews_previewable';
  $handler->display->display_options['fields']['views_handler_field_applenews_previewable']['table'] = 'applenews_entities';
  $handler->display->display_options['fields']['views_handler_field_applenews_previewable']['field'] = 'views_handler_field_applenews_previewable';
  $handler->display->display_options['fields']['views_handler_field_applenews_previewable']['label'] = '';
  $handler->display->display_options['fields']['views_handler_field_applenews_previewable']['exclude'] = TRUE;
  $handler->display->display_options['fields']['views_handler_field_applenews_previewable']['element_label_colon'] = FALSE;
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[views_handler_field_applenews_previewable] [edit_node] [delete_node]';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['description'] = 'Search by content title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Content type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  $handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status_1']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status_1']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
  $handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Apple News: Entity is published to Apple News */
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['id'] = 'views_handler_filter_applenews_status';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['table'] = 'applenews_entities';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['field'] = 'views_handler_filter_applenews_status';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['value'] = '1';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['group'] = 1;
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['expose']['label'] = 'Apple News Status';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['expose']['operator'] = 'views_handler_filter_applenews_status_op';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['expose']['identifier'] = 'views_handler_filter_applenews_status';
  $handler->display->display_options['filters']['views_handler_filter_applenews_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Content */
  $handler = $view->new_display('page', 'Content', 'page');
  $handler->display->display_options['path'] = 'admin/config/content/applenews/content';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Content';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
