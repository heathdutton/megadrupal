<?php
/**
 * @file
 * instagram_feeds_moderation.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function instagram_feeds_moderation_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'instagram_feeds_moderation';
  $view->description = 'Creates administrative views for moderation instagram media items.';
  $view->tag = 'instagram';
  $view->base_table = 'node';
  $view->human_name = 'Instagram Feeds Moderation';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Instagram Media Items';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10, 25, 50, 100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_instf_image' => 'field_instf_image',
    'name' => 'name',
    'tid' => 'tid',
    'field_instf_blacklist_type' => 'field_instf_blacklist_type',
    'field_instf_created' => 'field_instf_created',
    'nothing' => 'nothing',
    'sticky' => 'sticky',
    'status' => 'status',
    'views_bulk_operations' => 'views_bulk_operations',
    'timestamp' => 'status',
    'timestamp_1' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = 'field_instf_created';
  $handler->display->display_options['style_options']['info'] = array(
    'field_instf_image' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_instf_blacklist_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_instf_created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sticky' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ',<br />',
      'empty_column' => 0,
    ),
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* Relationship: image_username */
  $handler->display->display_options['relationships']['field_instf_user_tid']['id'] = 'field_instf_user_tid';
  $handler->display->display_options['relationships']['field_instf_user_tid']['table'] = 'field_data_field_instf_user';
  $handler->display->display_options['relationships']['field_instf_user_tid']['field'] = 'field_instf_user_tid';
  $handler->display->display_options['relationships']['field_instf_user_tid']['ui_name'] = 'image_username';
  $handler->display->display_options['relationships']['field_instf_user_tid']['label'] = 'image_username';
  /* Relationship: image_hashtag */
  $handler->display->display_options['relationships']['field_instf_hash_tags_tid']['id'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['relationships']['field_instf_hash_tags_tid']['table'] = 'field_data_field_instf_hash_tags';
  $handler->display->display_options['relationships']['field_instf_hash_tags_tid']['field'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['relationships']['field_instf_hash_tags_tid']['ui_name'] = 'image_hashtag';
  $handler->display->display_options['relationships']['field_instf_hash_tags_tid']['label'] = 'image_hashtag';
  /* Relationship: Flags: flag_as_inappropriate */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['ui_name'] = 'Flagged';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'flagged';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'flag_as_inappropriate';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: image_archived */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['ui_name'] = 'Flag Moderated';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'flag_moderated';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'image_archived';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::instagram_feeds_moderation_flag_archive' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Archive',
      'postpone_processing' => 0,
    ),
    'action::instagram_feeds_moderation_flag_block' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Block',
      'postpone_processing' => 0,
    ),
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::flag_node_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Favorite',
      'postpone_processing' => 0,
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Unfavorite',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
      'postpone_processing' => 0,
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
    'action::node_promote_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
  );
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_plain';
  /* Field: Thumbnail URL */
  $handler->display->display_options['fields']['field_instf_image_url_1']['id'] = 'field_instf_image_url_1';
  $handler->display->display_options['fields']['field_instf_image_url_1']['table'] = 'field_data_field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url_1']['field'] = 'field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url_1']['ui_name'] = 'Thumbnail URL';
  $handler->display->display_options['fields']['field_instf_image_url_1']['label'] = '';
  $handler->display->display_options['fields']['field_instf_image_url_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_image_url_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_instf_image_url_1']['alter']['text'] = '[field_instf_image_url_1-url]<div class="hover">[body]</div>';
  $handler->display->display_options['fields']['field_instf_image_url_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_image_url_1']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_instf_image_url_1']['type'] = 'link_plain';
  /* Field: Image URL */
  $handler->display->display_options['fields']['field_instf_image_url']['id'] = 'field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url']['table'] = 'field_data_field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url']['field'] = 'field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url']['ui_name'] = 'Image URL';
  $handler->display->display_options['fields']['field_instf_image_url']['label'] = '';
  $handler->display->display_options['fields']['field_instf_image_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_image_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_image_url']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_instf_image_url']['type'] = 'link_plain';
  /* Field: Popup */
  $handler->display->display_options['fields']['colorbox']['id'] = 'colorbox';
  $handler->display->display_options['fields']['colorbox']['table'] = 'colorbox';
  $handler->display->display_options['fields']['colorbox']['field'] = 'colorbox';
  $handler->display->display_options['fields']['colorbox']['ui_name'] = 'Popup';
  $handler->display->display_options['fields']['colorbox']['label'] = 'Image';
  $handler->display->display_options['fields']['colorbox']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['colorbox']['trigger_field'] = 'field_instf_image_url_1';
  $handler->display->display_options['fields']['colorbox']['popup'] = '[field_instf_image_url]';
  $handler->display->display_options['fields']['colorbox']['caption'] = '[body]';
  $handler->display->display_options['fields']['colorbox']['gid'] = 0;
  $handler->display->display_options['fields']['colorbox']['width'] = '';
  $handler->display->display_options['fields']['colorbox']['height'] = '';
  /* Field: Instagram User */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'field_instf_user_tid';
  $handler->display->display_options['fields']['name']['ui_name'] = 'Instagram User';
  $handler->display->display_options['fields']['name']['label'] = 'Instagram User';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'http://instagram.com/[name]#';
  $handler->display->display_options['fields']['name']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['external'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['target'] = '_blank';
  /* Field: User term id */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'field_instf_user_tid';
  $handler->display->display_options['fields']['tid']['ui_name'] = 'User term id';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: User Status */
  $handler->display->display_options['fields']['field_instf_blacklist_type']['id'] = 'field_instf_blacklist_type';
  $handler->display->display_options['fields']['field_instf_blacklist_type']['table'] = 'field_data_field_instf_blacklist_type';
  $handler->display->display_options['fields']['field_instf_blacklist_type']['field'] = 'field_instf_blacklist_type';
  $handler->display->display_options['fields']['field_instf_blacklist_type']['relationship'] = 'field_instf_user_tid';
  $handler->display->display_options['fields']['field_instf_blacklist_type']['ui_name'] = 'User Status';
  $handler->display->display_options['fields']['field_instf_blacklist_type']['label'] = 'User Status';
  $handler->display->display_options['fields']['field_instf_blacklist_type']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_instf_blacklist_type']['alter']['path'] = 'taxonomy/term/[tid]/edit';
  /* Field: Date when Instagram image created */
  $handler->display->display_options['fields']['field_instf_created']['id'] = 'field_instf_created';
  $handler->display->display_options['fields']['field_instf_created']['table'] = 'field_data_field_instf_created';
  $handler->display->display_options['fields']['field_instf_created']['field'] = 'field_instf_created';
  $handler->display->display_options['fields']['field_instf_created']['ui_name'] = 'Date when Instagram image created';
  $handler->display->display_options['fields']['field_instf_created']['label'] = 'Image Created';
  $handler->display->display_options['fields']['field_instf_created']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Feed name */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Feed name';
  $handler->display->display_options['fields']['nothing']['label'] = 'Feed Name';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[custom_instagram_feed_name], [name]';
  /* Field: Sticky */
  $handler->display->display_options['fields']['sticky']['id'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['table'] = 'node';
  $handler->display->display_options['fields']['sticky']['field'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['ui_name'] = 'Sticky';
  $handler->display->display_options['fields']['sticky']['label'] = 'Favorite';
  $handler->display->display_options['fields']['sticky']['not'] = 0;
  /* Field: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['ui_name'] = 'Published';
  $handler->display->display_options['fields']['status']['label'] = 'Status of Image';
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Inappropriate (flagged) */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['timestamp']['ui_name'] = 'Inappropriate (flagged)';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Flagged';
  $handler->display->display_options['fields']['timestamp']['exclude'] = TRUE;
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['timestamp']['alter']['text'] = 'Flagged';
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'html5_tools_iso8601';
  /* Field: Archived */
  $handler->display->display_options['fields']['timestamp_1']['id'] = 'timestamp_1';
  $handler->display->display_options['fields']['timestamp_1']['table'] = 'flagging';
  $handler->display->display_options['fields']['timestamp_1']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp_1']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['fields']['timestamp_1']['ui_name'] = 'Archived';
  $handler->display->display_options['fields']['timestamp_1']['label'] = 'Archived time';
  $handler->display->display_options['fields']['timestamp_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['timestamp_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['timestamp_1']['alter']['text'] = 'Archived';
  $handler->display->display_options['fields']['timestamp_1']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['timestamp_1']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['timestamp_1']['date_format'] = 'html5_tools_iso8601';
  /* Sort criterion: Date when Instagram image created */
  $handler->display->display_options['sorts']['field_instf_created_value']['id'] = 'field_instf_created_value';
  $handler->display->display_options['sorts']['field_instf_created_value']['table'] = 'field_data_field_instf_created';
  $handler->display->display_options['sorts']['field_instf_created_value']['field'] = 'field_instf_created_value';
  $handler->display->display_options['sorts']['field_instf_created_value']['ui_name'] = 'Date when Instagram image created';
  $handler->display->display_options['sorts']['field_instf_created_value']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'instagram_media_item' => 'instagram_media_item',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Instagram Username */
  $handler->display->display_options['filters']['field_instf_user_tid']['id'] = 'field_instf_user_tid';
  $handler->display->display_options['filters']['field_instf_user_tid']['table'] = 'field_data_field_instf_user';
  $handler->display->display_options['filters']['field_instf_user_tid']['field'] = 'field_instf_user_tid';
  $handler->display->display_options['filters']['field_instf_user_tid']['ui_name'] = 'Instagram Username';
  $handler->display->display_options['filters']['field_instf_user_tid']['value'] = '';
  $handler->display->display_options['filters']['field_instf_user_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_instf_user_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_user_tid']['expose']['operator_id'] = 'field_instf_user_tid_op';
  $handler->display->display_options['filters']['field_instf_user_tid']['expose']['label'] = 'Instagram Username';
  $handler->display->display_options['filters']['field_instf_user_tid']['expose']['operator'] = 'field_instf_user_tid_op';
  $handler->display->display_options['filters']['field_instf_user_tid']['expose']['identifier'] = 'user';
  $handler->display->display_options['filters']['field_instf_user_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_instf_user_tid']['vocabulary'] = 'instagram_users';
  /* Filter criterion: Instagram Hashtag */
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['id'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['table'] = 'field_data_field_instf_hash_tags';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['field'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['ui_name'] = 'Instagram Hashtag';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['expose']['operator_id'] = 'field_instf_hash_tags_tid_op';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['expose']['label'] = 'Instagram Hashtag';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['expose']['operator'] = 'field_instf_hash_tags_tid_op';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['expose']['identifier'] = 'tag';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['group_info']['label'] = 'Hash Tags (field_instf_hash_tags)';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['group_info']['identifier'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  $handler->display->display_options['filters']['field_instf_hash_tags_tid']['vocabulary'] = 'instagram_hashtags';
  /* Filter criterion: Image Creation Date */
  $handler->display->display_options['filters']['field_instf_created_value']['id'] = 'field_instf_created_value';
  $handler->display->display_options['filters']['field_instf_created_value']['table'] = 'field_data_field_instf_created';
  $handler->display->display_options['filters']['field_instf_created_value']['field'] = 'field_instf_created_value';
  $handler->display->display_options['filters']['field_instf_created_value']['ui_name'] = 'Image Creation Date';
  $handler->display->display_options['filters']['field_instf_created_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_instf_created_value']['group'] = 1;
  $handler->display->display_options['filters']['field_instf_created_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_created_value']['expose']['operator_id'] = 'field_instf_created_value_op';
  $handler->display->display_options['filters']['field_instf_created_value']['expose']['label'] = 'Image Creation Date ( From / To )';
  $handler->display->display_options['filters']['field_instf_created_value']['expose']['operator'] = 'field_instf_created_value_op';
  $handler->display->display_options['filters']['field_instf_created_value']['expose']['identifier'] = 'date';
  $handler->display->display_options['filters']['field_instf_created_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_instf_created_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_instf_created_value']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['field_instf_created_value']['year_range'] = '-9:+0';
  /* Filter criterion: Favorite (sticky) */
  $handler->display->display_options['filters']['sticky']['id'] = 'sticky';
  $handler->display->display_options['filters']['sticky']['table'] = 'node';
  $handler->display->display_options['filters']['sticky']['field'] = 'sticky';
  $handler->display->display_options['filters']['sticky']['ui_name'] = 'Favorite (sticky)';
  $handler->display->display_options['filters']['sticky']['value'] = 'All';
  $handler->display->display_options['filters']['sticky']['group'] = 1;
  $handler->display->display_options['filters']['sticky']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sticky']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['sticky']['expose']['label'] = 'Favorite';
  $handler->display->display_options['filters']['sticky']['expose']['operator'] = 'sticky_op';
  $handler->display->display_options['filters']['sticky']['expose']['identifier'] = 'favorite';
  $handler->display->display_options['filters']['sticky']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  /* Filter criterion: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['ui_name'] = 'Published';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Moderation Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'node_status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  /* Filter criterion: Is video (boolean) */
  $handler->display->display_options['filters']['field_instf_marker_value']['id'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['table'] = 'field_data_field_instf_marker';
  $handler->display->display_options['filters']['field_instf_marker_value']['field'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['ui_name'] = 'Is video (boolean)';
  $handler->display->display_options['filters']['field_instf_marker_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['operator_id'] = 'field_instf_marker_value_op';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['label'] = 'Featured (field_instf_marker)';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['operator'] = 'field_instf_marker_value_op';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['identifier'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['label'] = 'Media type';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['identifier'] = 'video';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Image',
      'operator' => 'or',
      'value' => array(
        0 => '0',
      ),
    ),
    2 => array(
      'title' => 'Video',
      'operator' => 'or',
      'value' => array(
        1 => '1',
      ),
    ),
  );
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged']['id'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['filters']['flagged']['value'] = 'All';
  $handler->display->display_options['filters']['flagged']['exposed'] = TRUE;
  $handler->display->display_options['filters']['flagged']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['flagged']['expose']['label'] = 'Flagged';
  $handler->display->display_options['filters']['flagged']['expose']['operator'] = 'flagged_op';
  $handler->display->display_options['filters']['flagged']['expose']['identifier'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  /* Filter criterion: Archived */
  $handler->display->display_options['filters']['flagged_1']['id'] = 'flagged_1';
  $handler->display->display_options['filters']['flagged_1']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged_1']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged_1']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['filters']['flagged_1']['ui_name'] = 'Archived';
  $handler->display->display_options['filters']['flagged_1']['value'] = 'All';
  $handler->display->display_options['filters']['flagged_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['flagged_1']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['flagged_1']['expose']['label'] = 'Archived';
  $handler->display->display_options['filters']['flagged_1']['expose']['operator'] = 'flagged_1_op';
  $handler->display->display_options['filters']['flagged_1']['expose']['identifier'] = 'archived';
  $handler->display->display_options['filters']['flagged_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );

  /* Display: Instagram */
  $handler = $view->new_display('page', 'Instagram', 'admin_instagram_feeds');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Manage Feeds';
  $handler->display->display_options['display_description'] = 'Instagram Feeds management page';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['hide_admin_links'] = TRUE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10, 25, 50, 100';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'created' => 'created',
    'field_instf_favorites' => 'field_instf_favorites',
    'field_instf_marker' => 'field_instf_marker',
    'nid' => 'nid',
    'edit_node' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_instf_favorites' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_instf_marker' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' &nbsp; &nbsp; &nbsp; ',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::instagram_feeds_moderation_flag_archive' => array(
      'selected' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Archive',
      'postpone_processing' => 0,
    ),
    'action::instagram_feeds_moderation_flag_block' => array(
      'selected' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Block',
      'postpone_processing' => 0,
    ),
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Delete Feed',
      'postpone_processing' => 0,
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::flag_node_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
      'postpone_processing' => 0,
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
    'action::node_promote_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
  );
  /* Field: Node Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['ui_name'] = 'Node Title';
  $handler->display->display_options['fields']['title']['label'] = 'Feed Name';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Type of Feed */
  $handler->display->display_options['fields']['field_instf_favorites']['id'] = 'field_instf_favorites';
  $handler->display->display_options['fields']['field_instf_favorites']['table'] = 'field_data_field_instf_favorites';
  $handler->display->display_options['fields']['field_instf_favorites']['field'] = 'field_instf_favorites';
  $handler->display->display_options['fields']['field_instf_favorites']['label'] = 'Feed Type';
  /* Field: Status */
  $handler->display->display_options['fields']['field_instf_marker']['id'] = 'field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['table'] = 'field_data_field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['field'] = 'field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['ui_name'] = 'Status';
  $handler->display->display_options['fields']['field_instf_marker']['label'] = 'Status';
  $handler->display->display_options['fields']['field_instf_marker']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['alter']['text'] = 'Suspended';
  $handler->display->display_options['fields']['field_instf_marker']['empty'] = 'Live';
  $handler->display->display_options['fields']['field_instf_marker']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['type'] = 'list_key';
  /* Field: Action */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['ui_name'] = 'Action';
  $handler->display->display_options['fields']['nid']['label'] = 'Actions';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'Manage Images';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'admin/content/instagram/media?feed=[nid]';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit Settings';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'instagram_feed' => 'instagram_feed',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Feed Type */
  $handler->display->display_options['filters']['field_instf_favorites_value']['id'] = 'field_instf_favorites_value';
  $handler->display->display_options['filters']['field_instf_favorites_value']['table'] = 'field_data_field_instf_favorites';
  $handler->display->display_options['filters']['field_instf_favorites_value']['field'] = 'field_instf_favorites_value';
  $handler->display->display_options['filters']['field_instf_favorites_value']['ui_name'] = 'Feed Type';
  $handler->display->display_options['filters']['field_instf_favorites_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['operator_id'] = 'field_instf_favorites_value_op';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['label'] = 'Feed Type';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['operator'] = 'field_instf_favorites_value_op';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  /* Filter criterion: Feeds Status */
  $handler->display->display_options['filters']['field_instf_marker_value']['id'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['table'] = 'field_data_field_instf_marker';
  $handler->display->display_options['filters']['field_instf_marker_value']['field'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['ui_name'] = 'Feeds Status';
  $handler->display->display_options['filters']['field_instf_marker_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['operator_id'] = 'field_instf_marker_value_op';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['label'] = 'Featured (field_instf_marker)';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['operator'] = 'field_instf_marker_value_op';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['identifier'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['label'] = 'Status';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['identifier'] = 'active';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Live',
      'operator' => 'or',
      'value' => array(
        0 => '0',
      ),
    ),
    2 => array(
      'title' => 'Suspended',
      'operator' => 'or',
      'value' => array(
        1 => '1',
      ),
    ),
  );
  $handler->display->display_options['path'] = 'admin/content/instagram';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Instagram';
  $handler->display->display_options['menu']['description'] = 'Manage Instagram Feeds';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'Instagram';
  $handler->display->display_options['tab_options']['description'] = 'Manage Instagram Feeds and Images';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Instagram Media Items */
  $handler = $view->new_display('page', 'Instagram Media Items', 'admin_instagram_media_items');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['hide_admin_links'] = TRUE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Empty text */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['ui_name'] = 'Empty text';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No content from the specified filter criteria.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['path'] = 'admin/content/instagram/media';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Instagram Media Items';
  $handler->display->display_options['menu']['description'] = 'Manage Instagram Images and Videos';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Instagram Feeds */
  $handler = $view->new_display('page', 'Instagram Feeds', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Manage Feeds';
  $handler->display->display_options['display_description'] = 'Instagram Feeds management page';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['hide_admin_links'] = TRUE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10, 25, 50, 100';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'created' => 'created',
    'field_instf_favorites' => 'field_instf_favorites',
    'field_instf_marker' => 'field_instf_marker',
    'nid' => 'nid',
    'edit_node' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_instf_favorites' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_instf_marker' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' &nbsp; &nbsp; &nbsp; ',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  /* Field: Node Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['ui_name'] = 'Node Title';
  $handler->display->display_options['fields']['title']['label'] = 'Feed Name';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Type of Feed */
  $handler->display->display_options['fields']['field_instf_favorites']['id'] = 'field_instf_favorites';
  $handler->display->display_options['fields']['field_instf_favorites']['table'] = 'field_data_field_instf_favorites';
  $handler->display->display_options['fields']['field_instf_favorites']['field'] = 'field_instf_favorites';
  $handler->display->display_options['fields']['field_instf_favorites']['label'] = 'Feed Type';
  /* Field: Status */
  $handler->display->display_options['fields']['field_instf_marker']['id'] = 'field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['table'] = 'field_data_field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['field'] = 'field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['ui_name'] = 'Status';
  $handler->display->display_options['fields']['field_instf_marker']['label'] = 'Status';
  $handler->display->display_options['fields']['field_instf_marker']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['alter']['text'] = 'Archived';
  $handler->display->display_options['fields']['field_instf_marker']['empty'] = 'Pull New Items';
  $handler->display->display_options['fields']['field_instf_marker']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['type'] = 'list_key';
  /* Field: Action */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['ui_name'] = 'Action';
  $handler->display->display_options['fields']['nid']['label'] = 'Actions';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'Manage Images';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'admin/content/instagram/media?feed=[nid]';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit Settings';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'instagram_feed' => 'instagram_feed',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Feed Type */
  $handler->display->display_options['filters']['field_instf_favorites_value']['id'] = 'field_instf_favorites_value';
  $handler->display->display_options['filters']['field_instf_favorites_value']['table'] = 'field_data_field_instf_favorites';
  $handler->display->display_options['filters']['field_instf_favorites_value']['field'] = 'field_instf_favorites_value';
  $handler->display->display_options['filters']['field_instf_favorites_value']['ui_name'] = 'Feed Type';
  $handler->display->display_options['filters']['field_instf_favorites_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['operator_id'] = 'field_instf_favorites_value_op';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['label'] = 'Feed Type';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['operator'] = 'field_instf_favorites_value_op';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['field_instf_favorites_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    11 => 0,
    6 => 0,
    3 => 0,
  );
  /* Filter criterion: Feeds Status */
  $handler->display->display_options['filters']['field_instf_marker_value']['id'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['table'] = 'field_data_field_instf_marker';
  $handler->display->display_options['filters']['field_instf_marker_value']['field'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['ui_name'] = 'Feeds Status';
  $handler->display->display_options['filters']['field_instf_marker_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['operator_id'] = 'field_instf_marker_value_op';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['label'] = 'Featured (field_instf_marker)';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['operator'] = 'field_instf_marker_value_op';
  $handler->display->display_options['filters']['field_instf_marker_value']['expose']['identifier'] = 'field_instf_marker_value';
  $handler->display->display_options['filters']['field_instf_marker_value']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['label'] = 'Status';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['identifier'] = 'active';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['default_group'] = '1';
  $handler->display->display_options['filters']['field_instf_marker_value']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Updatable Feeds',
      'operator' => 'or',
      'value' => array(
        0 => '0',
      ),
    ),
    2 => array(
      'title' => 'Non-Updatable Feeds',
      'operator' => 'or',
      'value' => array(
        1 => '1',
      ),
    ),
  );
  $handler->display->display_options['path'] = 'admin/content/instagram/feeds';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Instagram Feeds';
  $handler->display->display_options['menu']['description'] = 'Manage Instagram Feeds';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'Instagram';
  $handler->display->display_options['tab_options']['description'] = 'Manage Instagram Feeds and Images';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['instagram_feeds_moderation'] = $view;

  return $export;
}
