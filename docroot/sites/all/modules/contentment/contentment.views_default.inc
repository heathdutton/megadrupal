<?php
/**
 * @file
 * Provides default views for the Contentment module.
 */

/**
 * Implements hook_views_default_views().
 */
function contentment_views_default_views() {
  $views = array();

  /**
   * Contentment: Manage
   */
  $view = new view;
  $view->name = 'contentment_manage';
  $view->description = 'View and manage different types of content.';
  $view->tag = 'contentment';
  $view->base_table = 'node';
  $view->human_name = 'Contentment: Manage';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;

  /*** Display: Master ***/
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'timestamp' => 'title',
    'created' => 'created',
    'name' => 'name',
    'changed' => 'changed',
    'status' => 'status',
    'promote' => 'status',
    'sticky' => 'status',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
  );
  if (module_exists('comment')) {
    $handler->display->display_options['style_options']['columns']['comment'] = 'comment';
    $handler->display->display_options['style_options']['columns']['comment_count'] = 'comment';
  }
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
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
    'name' => array(
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
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'promote' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sticky' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '&nbsp;&nbsp;&nbsp;',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  if (module_exists('comment')) {
    $handler->display->display_options['style_options']['info']['comment'] = array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    );
    $handler->display->display_options['style_options']['info']['comment_count'] = array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    );
  }
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  if (module_exists('views_bulk_operations')) {
    /* Field: Bulk operations: Content */
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
    $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
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
    $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
      'action::node_assign_owner_action' => array(
        'selected' => 0,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::views_bulk_operations_delete_item' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Delete',
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
      'action::node_make_sticky_action' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Make sticky',
      ),
      'action::node_make_unsticky_action' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Make unsticky',
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
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Promote',
      ),
      'action::node_publish_action' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Publish',
      ),
      'action::system_goto_action' => array(
        'selected' => 0,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::node_unpromote_action' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Unpromote',
      ),
      'action::node_save_action' => array(
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
      'action::node_unpublish_action' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Unpublish',
      ),
      'action::node_unpublish_by_keyword_action' => array(
        'selected' => 0,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
    );
    if (module_exists('comment')) {
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations']['action::views_bulk_operations_modify_action'] = array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Change comments status',
        'settings' => array(
          'show_all_tokens' => 0,
          'display_values' => array(
            'comment' => 'comment',
          ),
        ),
      );
    }
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  }
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Has new content */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'history';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = '';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['link_to_node'] = 0;
  if (module_exists('comment')) {
    $handler->display->display_options['fields']['timestamp']['comments'] = 0;
  }
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
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
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
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
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  if (module_exists('comment')) {
    /* Field: Content: Updated date */
    $handler->display->display_options['fields']['changed']['table'] = 'node_comment_statistics';
    $handler->display->display_options['fields']['changed']['field'] = 'last_updated';
    /* Field: Content: Comment status */
    $handler->display->display_options['fields']['comment']['id'] = 'comment';
    $handler->display->display_options['fields']['comment']['table'] = 'node';
    $handler->display->display_options['fields']['comment']['field'] = 'comment';
    $handler->display->display_options['fields']['comment']['label'] = 'Comments';
    $handler->display->display_options['fields']['comment']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['external'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['comment']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['comment']['alter']['more_link'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['html'] = 0;
    $handler->display->display_options['fields']['comment']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['comment']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['comment']['hide_empty'] = 0;
    $handler->display->display_options['fields']['comment']['empty_zero'] = 0;
    $handler->display->display_options['fields']['comment']['hide_alter_empty'] = 1;
    /* Field: Content: Comment count */
    $handler->display->display_options['fields']['comment_count']['id'] = 'comment_count';
    $handler->display->display_options['fields']['comment_count']['table'] = 'node_comment_statistics';
    $handler->display->display_options['fields']['comment_count']['field'] = 'comment_count';
    $handler->display->display_options['fields']['comment_count']['label'] = '';
    $handler->display->display_options['fields']['comment_count']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['comment_count']['alter']['path'] = 'node/[nid]#comments';
    $handler->display->display_options['fields']['comment_count']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['external'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['comment_count']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['comment_count']['alter']['more_link'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['html'] = 0;
    $handler->display->display_options['fields']['comment_count']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['comment_count']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['comment_count']['hide_empty'] = 1;
    $handler->display->display_options['fields']['comment_count']['empty_zero'] = 0;
    $handler->display->display_options['fields']['comment_count']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['comment_count']['format_plural'] = 1;
    $handler->display->display_options['fields']['comment_count']['format_plural_singular'] = '1 comment';
    $handler->display->display_options['fields']['comment_count']['format_plural_plural'] = '@count comments';
  }
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Promoted to front page */
  $handler->display->display_options['fields']['promote']['id'] = 'promote';
  $handler->display->display_options['fields']['promote']['table'] = 'node';
  $handler->display->display_options['fields']['promote']['field'] = 'promote';
  $handler->display->display_options['fields']['promote']['label'] = '';
  $handler->display->display_options['fields']['promote']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['external'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['promote']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['promote']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['html'] = 0;
  $handler->display->display_options['fields']['promote']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['promote']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['promote']['hide_empty'] = 0;
  $handler->display->display_options['fields']['promote']['empty_zero'] = 0;
  $handler->display->display_options['fields']['promote']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['promote']['type'] = 'promoted-notpromoted';
  $handler->display->display_options['fields']['promote']['not'] = 0;
  /* Field: Content: Sticky */
  $handler->display->display_options['fields']['sticky']['id'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['table'] = 'node';
  $handler->display->display_options['fields']['sticky']['field'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['label'] = '';
  $handler->display->display_options['fields']['sticky']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['external'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['sticky']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['sticky']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['html'] = 0;
  $handler->display->display_options['fields']['sticky']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['sticky']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['sticky']['hide_empty'] = 0;
  $handler->display->display_options['fields']['sticky']['empty_zero'] = 0;
  $handler->display->display_options['fields']['sticky']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['sticky']['type'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['not'] = 0;
  /* Field: Node: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
  /* Field: Node: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_node']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delete_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['delete_node']['text'] = 'delete';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'not empty';
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title contains';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['required'] = 0;
  $handler->display->display_options['filters']['title']['expose']['multiple'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  if (module_exists('comment')) {
    /* Filter criterion: Content: Comment status */
    $handler->display->display_options['filters']['comment']['id'] = 'comment';
    $handler->display->display_options['filters']['comment']['table'] = 'node';
    $handler->display->display_options['filters']['comment']['field'] = 'comment';
    $handler->display->display_options['filters']['comment']['group'] = 1;
    $handler->display->display_options['filters']['comment']['exposed'] = TRUE;
    $handler->display->display_options['filters']['comment']['expose']['operator_id'] = 'comment_op';
    $handler->display->display_options['filters']['comment']['expose']['label'] = 'Comments';
    $handler->display->display_options['filters']['comment']['expose']['operator'] = 'comment_op';
    $handler->display->display_options['filters']['comment']['expose']['identifier'] = 'comment';
    $handler->display->display_options['filters']['comment']['expose']['reduce'] = 0;
  }
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 'All';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = FALSE;
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = 'All';
  $handler->display->display_options['filters']['promote']['group'] = 1;
  $handler->display->display_options['filters']['promote']['exposed'] = TRUE;
  $handler->display->display_options['filters']['promote']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['promote']['expose']['label'] = 'Promoted';
  $handler->display->display_options['filters']['promote']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['promote']['expose']['operator'] = 'promote_op';
  $handler->display->display_options['filters']['promote']['expose']['identifier'] = 'promote';
  $handler->display->display_options['filters']['promote']['expose']['multiple'] = FALSE;
  /* Filter criterion: Content: Sticky */
  $handler->display->display_options['filters']['sticky']['id'] = 'sticky';
  $handler->display->display_options['filters']['sticky']['table'] = 'node';
  $handler->display->display_options['filters']['sticky']['field'] = 'sticky';
  $handler->display->display_options['filters']['sticky']['value'] = 'All';
  $handler->display->display_options['filters']['sticky']['group'] = 1;
  $handler->display->display_options['filters']['sticky']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sticky']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['sticky']['expose']['label'] = 'Sticky';
  $handler->display->display_options['filters']['sticky']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['sticky']['expose']['operator'] = 'sticky_op';
  $handler->display->display_options['filters']['sticky']['expose']['identifier'] = 'sticky';
  $handler->display->display_options['filters']['sticky']['expose']['multiple'] = FALSE;

  /*** Display: [content_type] ***/
  foreach (node_type_get_types() as $type) {
    $type_url_str = str_replace('_', '-', $type->type);

    $handler = $view->new_display('page', $type->name, $type->type);
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Manage ' . $type->name;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Content: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'node';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      $type->type => $type->type,
    );
    $handler->display->display_options['filters']['type']['group'] = 1;
    /* Filter criterion: Content: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['group'] = 1;
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title contains';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
    $handler->display->display_options['filters']['title']['expose']['required'] = 0;
    $handler->display->display_options['filters']['title']['expose']['multiple'] = FALSE;
    /* Filter criterion: User: Name */
    $handler->display->display_options['filters']['uid']['id'] = 'uid';
    $handler->display->display_options['filters']['uid']['table'] = 'users';
    $handler->display->display_options['filters']['uid']['field'] = 'uid';
    $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
    $handler->display->display_options['filters']['uid']['value'] = '';
    $handler->display->display_options['filters']['uid']['group'] = 1;
    $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
    $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
    $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
    $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
    $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
    if (module_exists('comment')) {
      /* Filter criterion: Content: Comment status */
      $handler->display->display_options['filters']['comment']['id'] = 'comment';
      $handler->display->display_options['filters']['comment']['table'] = 'node';
      $handler->display->display_options['filters']['comment']['field'] = 'comment';
      $handler->display->display_options['filters']['comment']['group'] = 1;
      $handler->display->display_options['filters']['comment']['exposed'] = TRUE;
      $handler->display->display_options['filters']['comment']['expose']['operator_id'] = 'comment_op';
      $handler->display->display_options['filters']['comment']['expose']['label'] = 'Comments';
      $handler->display->display_options['filters']['comment']['expose']['operator'] = 'comment_op';
      $handler->display->display_options['filters']['comment']['expose']['identifier'] = 'comment';
      $handler->display->display_options['filters']['comment']['expose']['reduce'] = 0;
    }
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 'All';
    $handler->display->display_options['filters']['status']['group'] = 1;
    $handler->display->display_options['filters']['status']['exposed'] = TRUE;
    $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
    $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
    $handler->display->display_options['filters']['status']['expose']['use_operator'] = FALSE;
    $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
    $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
    $handler->display->display_options['filters']['status']['expose']['multiple'] = FALSE;
    /* Filter criterion: Content: Promoted to front page */
    $handler->display->display_options['filters']['promote']['id'] = 'promote';
    $handler->display->display_options['filters']['promote']['table'] = 'node';
    $handler->display->display_options['filters']['promote']['field'] = 'promote';
    $handler->display->display_options['filters']['promote']['value'] = 'All';
    $handler->display->display_options['filters']['promote']['group'] = 1;
    $handler->display->display_options['filters']['promote']['exposed'] = TRUE;
    $handler->display->display_options['filters']['promote']['expose']['operator_id'] = '';
    $handler->display->display_options['filters']['promote']['expose']['label'] = 'Promoted';
    $handler->display->display_options['filters']['promote']['expose']['use_operator'] = FALSE;
    $handler->display->display_options['filters']['promote']['expose']['operator'] = 'promote_op';
    $handler->display->display_options['filters']['promote']['expose']['identifier'] = 'promote';
    $handler->display->display_options['filters']['promote']['expose']['multiple'] = FALSE;
    /* Filter criterion: Content: Sticky */
    $handler->display->display_options['filters']['sticky']['id'] = 'sticky';
    $handler->display->display_options['filters']['sticky']['table'] = 'node';
    $handler->display->display_options['filters']['sticky']['field'] = 'sticky';
    $handler->display->display_options['filters']['sticky']['value'] = 'All';
    $handler->display->display_options['filters']['sticky']['group'] = 1;
    $handler->display->display_options['filters']['sticky']['exposed'] = TRUE;
    $handler->display->display_options['filters']['sticky']['expose']['operator_id'] = '';
    $handler->display->display_options['filters']['sticky']['expose']['label'] = 'Sticky';
    $handler->display->display_options['filters']['sticky']['expose']['use_operator'] = FALSE;
    $handler->display->display_options['filters']['sticky']['expose']['operator'] = 'sticky_op';
    $handler->display->display_options['filters']['sticky']['expose']['identifier'] = 'sticky';
    $handler->display->display_options['filters']['sticky']['expose']['multiple'] = FALSE;
    $handler->display->display_options['path'] = 'contentment/' . $type_url_str . '/manage';
    $handler->display->display_options['menu']['type'] = 'default tab';
    $handler->display->display_options['menu']['title'] = 'Manage';
    $handler->display->display_options['menu']['weight'] = '-5';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['tab_options']['type'] = 'tab';
    $handler->display->display_options['tab_options']['title'] = $type->name;
    $handler->display->display_options['tab_options']['weight'] = '0';
  }
  $views[$view->name] = $view;


  /**
   * Contentment: Dashboard
   */
  $view = new view;
  $view->name = 'contentment_dashboard';
  $view->description = 'Views to display on the main Contentment dashboard.';
  $view->tag = 'contentment';
  $view->base_table = 'node';
  $view->human_name = 'Contentment: Dashboard';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;

  /*** Display: Master ***/
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recent Activity';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'timestamp' => 'title',
    'type' => 'type',
    'created' => 'created',
    'name' => 'name',
    'changed' => 'changed',
    'status' => 'status',
    'promote' => 'status',
    'sticky' => 'status',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
  );
  if (module_exists('comment')) {
    $handler->display->display_options['style_options']['columns']['comment'] = 'comment';
    $handler->display->display_options['style_options']['columns']['comment_count'] = 'comment';
  }
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'timestamp' => array(
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
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'promote' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sticky' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '&nbsp;&nbsp;&nbsp;',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  if (module_exists('comment')) {
    $handler->display->display_options['style_options']['info']['comment'] = array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    );
    $handler->display->display_options['style_options']['info']['comment_count'] = array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    );
  }
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Has new content */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'history';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = '';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['link_to_node'] = 0;
  if (module_exists('comment')) {
    $handler->display->display_options['fields']['timestamp']['comments'] = 0;
  }
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 0;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
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
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
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
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['changed']['date_format'] = 'time ago';
  if (module_exists('comment')) {
    /* Field: Content: Updated date */
    $handler->display->display_options['fields']['changed']['table'] = 'node_comment_statistics';
    $handler->display->display_options['fields']['changed']['field'] = 'last_updated';
    /* Field: Content: Comment status */
    $handler->display->display_options['fields']['comment']['id'] = 'comment';
    $handler->display->display_options['fields']['comment']['table'] = 'node';
    $handler->display->display_options['fields']['comment']['field'] = 'comment';
    $handler->display->display_options['fields']['comment']['label'] = 'Comments';
    $handler->display->display_options['fields']['comment']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['external'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['comment']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['comment']['alter']['more_link'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['comment']['alter']['html'] = 0;
    $handler->display->display_options['fields']['comment']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['comment']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['comment']['hide_empty'] = 0;
    $handler->display->display_options['fields']['comment']['empty_zero'] = 0;
    $handler->display->display_options['fields']['comment']['hide_alter_empty'] = 1;
    /* Field: Content: Comment count */
    $handler->display->display_options['fields']['comment_count']['id'] = 'comment_count';
    $handler->display->display_options['fields']['comment_count']['table'] = 'node_comment_statistics';
    $handler->display->display_options['fields']['comment_count']['field'] = 'comment_count';
    $handler->display->display_options['fields']['comment_count']['label'] = '';
    $handler->display->display_options['fields']['comment_count']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['comment_count']['alter']['path'] = 'node/[nid]#comments';
    $handler->display->display_options['fields']['comment_count']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['external'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['comment_count']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['comment_count']['alter']['more_link'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['comment_count']['alter']['html'] = 0;
    $handler->display->display_options['fields']['comment_count']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['comment_count']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['comment_count']['hide_empty'] = 1;
    $handler->display->display_options['fields']['comment_count']['empty_zero'] = 0;
    $handler->display->display_options['fields']['comment_count']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['comment_count']['format_plural'] = 1;
    $handler->display->display_options['fields']['comment_count']['format_plural_singular'] = '1 comment';
    $handler->display->display_options['fields']['comment_count']['format_plural_plural'] = '@count comments';
  }
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Promoted to front page */
  $handler->display->display_options['fields']['promote']['id'] = 'promote';
  $handler->display->display_options['fields']['promote']['table'] = 'node';
  $handler->display->display_options['fields']['promote']['field'] = 'promote';
  $handler->display->display_options['fields']['promote']['label'] = '';
  $handler->display->display_options['fields']['promote']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['external'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['promote']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['promote']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['promote']['alter']['html'] = 0;
  $handler->display->display_options['fields']['promote']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['promote']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['promote']['hide_empty'] = 0;
  $handler->display->display_options['fields']['promote']['empty_zero'] = 0;
  $handler->display->display_options['fields']['promote']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['promote']['type'] = 'promoted-notpromoted';
  $handler->display->display_options['fields']['promote']['not'] = 0;
  /* Field: Content: Sticky */
  $handler->display->display_options['fields']['sticky']['id'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['table'] = 'node';
  $handler->display->display_options['fields']['sticky']['field'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['label'] = '';
  $handler->display->display_options['fields']['sticky']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['external'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['sticky']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['sticky']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['sticky']['alter']['html'] = 0;
  $handler->display->display_options['fields']['sticky']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['sticky']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['sticky']['hide_empty'] = 0;
  $handler->display->display_options['fields']['sticky']['empty_zero'] = 0;
  $handler->display->display_options['fields']['sticky']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['sticky']['type'] = 'sticky';
  $handler->display->display_options['fields']['sticky']['not'] = 0;
  /* Field: Node: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
  /* Field: Node: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_node']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delete_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['delete_node']['text'] = 'delete';

  /*** Display: Activity ***/
  $handler = $view->new_display('panel_pane', 'Activity', 'activity');
  $handler->display->display_options['display_description'] = 'Displays a list of the latest content updates.';
  $handler->display->display_options['pane_category']['name'] = 'Contentment';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $views[$view->name] = $view;

  return $views;
}

