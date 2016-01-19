<?php

/**
 * @file
 * Mediabox views inc.
 */

/**
 * Implements hook_views_default_views().
 */
function mediabox_views_default_views() {
  $views = array();
  
  $view_library = new view;
  $view_library->name = 'mediabox_library';
  $view_library->description = '';
  $view_library->tag = 'default';
  $view_library->base_table = 'mediabox';
  $view_library->human_name = 'Mediabox library';
  $view_library->core = 7;
  $view_library->api_version = '3.0';
  $view_library->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view_library->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Mediabox library';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access mediabox library';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'alternate_text' => 'alternate_text',
    'created' => 'created',
    'modified' => 'modified',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'alternate_text' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'modified' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: mediabox: Author */
  $handler->display->display_options['relationships']['user_id']['id'] = 'user_id';
  $handler->display->display_options['relationships']['user_id']['table'] = 'mediabox';
  $handler->display->display_options['relationships']['user_id']['field'] = 'user_id';
  $handler->display->display_options['relationships']['user_id']['required'] = 0;
  /* Field: mediabox: mediabox ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'mediabox';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['id']['format_plural'] = 0;
  /* Field: Bulk operations: mediabox */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'mediabox';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'use_queue' => 0,
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
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Field: Field: mediabox image */
  $handler->display->display_options['fields']['mediabox_image']['id'] = 'mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['table'] = 'field_data_mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['field'] = 'mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['mediabox_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['mediabox_image']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['mediabox_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['mediabox_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['mediabox_image']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['mediabox_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['mediabox_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['mediabox_image']['field_api_classes'] = 0;
  /* Field: mediabox: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'mediabox';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'mediabox/[id]/edit';
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[title]';
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'user_id';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: mediabox: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'mediabox';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: mediabox: Modified */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'mediabox';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  $handler->display->display_options['fields']['modified']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['external'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['modified']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['modified']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['modified']['alter']['html'] = 0;
  $handler->display->display_options['fields']['modified']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['modified']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['modified']['hide_empty'] = 0;
  $handler->display->display_options['fields']['modified']['empty_zero'] = 0;
  $handler->display->display_options['fields']['modified']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['modified']['date_format'] = 'short';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'user_id';
  $handler->display->display_options['filters']['uid']['value'] = '';
  
  /* Field: Crop */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Crop';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'admin/structure/mediabox-library/crop/[id]';

  /* Display: Page */
  $handler = $view_library->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/structure/mediabox-library';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Mediabox library';
  $handler->display->display_options['menu']['description'] = 'List of uploaded mediabox files.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  $views['mediabox_library'] = $view_library;

  $mediabox_browser = new view();
  $mediabox_browser->name = 'mediabox_browser';
  $mediabox_browser->description = '';
  $mediabox_browser->tag = 'default';
  $mediabox_browser->base_table = 'mediabox';
  $mediabox_browser->human_name = 'Mediabox browser';
  $mediabox_browser->core = 7;
  $mediabox_browser->api_version = '3.0';
  $mediabox_browser->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $mediabox_browser->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Mediabox browser';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '9';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'mediabox';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Mediabox: Author */
  $handler->display->display_options['relationships']['user_id']['id'] = 'user_id';
  $handler->display->display_options['relationships']['user_id']['table'] = 'mediabox';
  $handler->display->display_options['relationships']['user_id']['field'] = 'user_id';
  /* Field: Mediabox: Mediabox ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'mediabox';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Field: Mediabox image */
  $handler->display->display_options['fields']['mediabox_image']['id'] = 'mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['table'] = 'field_data_mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['field'] = 'mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['label'] = '';
  $handler->display->display_options['fields']['mediabox_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['mediabox_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['mediabox_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => '',
  );
  /* Field: Mediabox: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'mediabox';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Title';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'user_id';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Mediabox: Caption */
  $handler->display->display_options['fields']['caption']['id'] = 'caption';
  $handler->display->display_options['fields']['caption']['table'] = 'mediabox';
  $handler->display->display_options['fields']['caption']['field'] = 'caption';
  $handler->display->display_options['fields']['caption']['empty'] = 'No caption provided';
  /* Field: Mediabox: Alternate_text */
  $handler->display->display_options['fields']['alternate_text']['id'] = 'alternate_text';
  $handler->display->display_options['fields']['alternate_text']['table'] = 'mediabox';
  $handler->display->display_options['fields']['alternate_text']['field'] = 'alternate_text';
  $handler->display->display_options['fields']['alternate_text']['label'] = 'Alternate text';
  $handler->display->display_options['fields']['alternate_text']['empty'] = 'No alternate text provided';
  /* Field: Mediabox: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'mediabox';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['empty'] = 'No description provided';
  /* Sort criterion: Mediabox: Mediabox ID */
  $handler->display->display_options['sorts']['id']['id'] = 'id';
  $handler->display->display_options['sorts']['id']['table'] = 'mediabox';
  $handler->display->display_options['sorts']['id']['field'] = 'id';
  $handler->display->display_options['sorts']['id']['order'] = 'DESC';

  /* Display: Admin Browser */
  $handler = $mediabox_browser->new_display('page', 'Admin Browser', 'admin_browser');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'user_id';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Author name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'admin/mediabox/browser';

  /* Display: User Browser */
  $handler = $mediabox_browser->new_display('block', 'User Browser', 'user_browser');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mediabox_grid';
  $handler->display->display_options['style_options']['columns'] = '4';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Mediabox: Mediabox ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'mediabox';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Field: Mediabox image */
  $handler->display->display_options['fields']['mediabox_image']['id'] = 'mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['table'] = 'field_data_mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['field'] = 'mediabox_image';
  $handler->display->display_options['fields']['mediabox_image']['label'] = '';
  $handler->display->display_options['fields']['mediabox_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['mediabox_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['mediabox_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => '',
  );
  /* Field: Mediabox: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'mediabox';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Mediabox: Author */
  $handler->display->display_options['arguments']['user_id']['id'] = 'user_id';
  $handler->display->display_options['arguments']['user_id']['table'] = 'mediabox';
  $handler->display->display_options['arguments']['user_id']['field'] = 'user_id';
  $handler->display->display_options['arguments']['user_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['user_id']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['user_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user_id']['summary_options']['items_per_page'] = '25';

  $views['mediabox_browser'] = $mediabox_browser;

  return $views;
}
