<?php
/**
 * @file
 * clientcert.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function clientcert_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'clientcert_admin';
  $view->description = 'List of all clientcert items.';
  $view->tag = 'default';
  $view->base_table = 'clientcert';
  $view->human_name = 'Clientcert admin';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'List of all Clientcert items';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view any clientcert item';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'ccid' => 'ccid',
    'uid' => 'uid',
    'status' => 'status',
    'status_login' => 'status_login',
    'verify' => 'verify',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'ccid';
  $handler->display->display_options['style_options']['info'] = array(
    'ccid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status_login' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'verify' => array(
      'sortable' => 1,
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
  );
  $handler->display->display_options['style_options']['override'] = FALSE;
  /* Field: Clientcert item: Clientcert item ID */
  $handler->display->display_options['fields']['ccid']['id'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['ccid']['field'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['label'] = 'ID';
  $handler->display->display_options['fields']['ccid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['ccid']['alter']['path'] = 'clientcert/item/[ccid]';
  /* Field: Clientcert item: User ID */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['path'] = 'user/[uid]';
  /* Field: Clientcert item: Active */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Clientcert item: Login active */
  $handler->display->display_options['fields']['status_login']['id'] = 'status_login';
  $handler->display->display_options['fields']['status_login']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_login']['field'] = 'status_login';
  $handler->display->display_options['fields']['status_login']['not'] = 0;
  /* Field: Clientcert item: Verify report */
  $handler->display->display_options['fields']['verify']['id'] = 'verify';
  $handler->display->display_options['fields']['verify']['table'] = 'clientcert';
  $handler->display->display_options['fields']['verify']['field'] = 'verify';
  /* Field: Clientcert item: Clientcert item created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'clientcert';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'privatemsg_current_day';
  /* Sort criterion: Clientcert item: Clientcert item created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'clientcert';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/config/people/clientcert/list';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'List of all clientcert items.';
  $handler->display->display_options['menu']['weight'] = '9';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['clientcert_admin'] = array(
    t('Master'),
    t('List of all Clientcert items'),
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
    t('ID'),
    t('.'),
    t(','),
    t('User ID'),
    t('Active'),
    t('Login active'),
    t('Verify report'),
    t('Clientcert item created'),
    t('Page'),
  );
  $export['clientcert_admin'] = $view;

  $view = new view();
  $view->name = 'clientcert_items';
  $view->description = 'Shows the clientcert items of a user.';
  $view->tag = 'default';
  $view->base_table = 'clientcert';
  $view->human_name = 'Clientcert items';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Clientcert values';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own clientcert item';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Clientcert item: Clientcert item ID */
  $handler->display->display_options['fields']['ccid']['id'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['ccid']['field'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['ccid']['alter']['path'] = 'clientcert/item/[ccid]';
  $handler->display->display_options['fields']['ccid']['element_wrapper_type'] = 'h3';
  /* Field: Clientcert item: Clientcert item title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'clientcert';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Clientcert item: Serial */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'clientcert';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['label'] = 'Serial number';
  /* Field: Clientcert item: Validity start */
  $handler->display->display_options['fields']['validity_start']['id'] = 'validity_start';
  $handler->display->display_options['fields']['validity_start']['table'] = 'clientcert';
  $handler->display->display_options['fields']['validity_start']['field'] = 'validity_start';
  $handler->display->display_options['fields']['validity_start']['date_format'] = 'short';
  /* Field: Clientcert item: Validity end */
  $handler->display->display_options['fields']['validity_end']['id'] = 'validity_end';
  $handler->display->display_options['fields']['validity_end']['table'] = 'clientcert';
  $handler->display->display_options['fields']['validity_end']['field'] = 'validity_end';
  $handler->display->display_options['fields']['validity_end']['date_format'] = 'short';
  /* Contextual filter: Clientcert item: User ID */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'clientcert';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: My clientcert items  */
  $handler = $view->new_display('page', 'My clientcert items ', 'page');
  $handler->display->display_options['path'] = 'clientcert/my-items';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'My clientcert items';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Clientcert items of user */
  $handler = $view->new_display('page', 'Clientcert items of user', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Clientcert items of user';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['path'] = 'user/%/clientcert';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Clientcert items';
  $handler->display->display_options['menu']['weight'] = '9';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Clientcert item: Clientcert item ID */
  $handler->display->display_options['fields']['ccid']['id'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['ccid']['field'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['alter']['path'] = 'clientcert/item/[ccid]';
  $handler->display->display_options['fields']['ccid']['element_wrapper_type'] = 'h3';
  /* Field: Clientcert item: Clientcert item created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'clientcert';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Clientcert item changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'clientcert';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Clientcert item title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'clientcert';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Clientcert item: User ID */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Clientcert item: Client Certificate (PEM) */
  $handler->display->display_options['fields']['cert']['id'] = 'cert';
  $handler->display->display_options['fields']['cert']['table'] = 'clientcert';
  $handler->display->display_options['fields']['cert']['field'] = 'cert';
  /* Field: Clientcert item: Certificate hash */
  $handler->display->display_options['fields']['cert_hash']['id'] = 'cert_hash';
  $handler->display->display_options['fields']['cert_hash']['table'] = 'clientcert';
  $handler->display->display_options['fields']['cert_hash']['field'] = 'cert_hash';
  /* Field: Clientcert item: Verify report */
  $handler->display->display_options['fields']['verify']['id'] = 'verify';
  $handler->display->display_options['fields']['verify']['table'] = 'clientcert';
  $handler->display->display_options['fields']['verify']['field'] = 'verify';
  /* Field: Clientcert item: Serial */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'clientcert';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['label'] = 'Serial number';
  /* Field: Clientcert item: DN subject */
  $handler->display->display_options['fields']['dn_subject']['id'] = 'dn_subject';
  $handler->display->display_options['fields']['dn_subject']['table'] = 'clientcert';
  $handler->display->display_options['fields']['dn_subject']['field'] = 'dn_subject';
  /* Field: Clientcert item: DN issuer */
  $handler->display->display_options['fields']['dn_issuer']['id'] = 'dn_issuer';
  $handler->display->display_options['fields']['dn_issuer']['table'] = 'clientcert';
  $handler->display->display_options['fields']['dn_issuer']['field'] = 'dn_issuer';
  /* Field: Clientcert item: Validity start */
  $handler->display->display_options['fields']['validity_start']['id'] = 'validity_start';
  $handler->display->display_options['fields']['validity_start']['table'] = 'clientcert';
  $handler->display->display_options['fields']['validity_start']['field'] = 'validity_start';
  $handler->display->display_options['fields']['validity_start']['date_format'] = 'short';
  $handler->display->display_options['fields']['validity_start']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Validity end */
  $handler->display->display_options['fields']['validity_end']['id'] = 'validity_end';
  $handler->display->display_options['fields']['validity_end']['table'] = 'clientcert';
  $handler->display->display_options['fields']['validity_end']['field'] = 'validity_end';
  $handler->display->display_options['fields']['validity_end']['date_format'] = 'short';
  $handler->display->display_options['fields']['validity_end']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Active */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Clientcert item: Login active */
  $handler->display->display_options['fields']['status_login']['id'] = 'status_login';
  $handler->display->display_options['fields']['status_login']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_login']['field'] = 'status_login';
  $handler->display->display_options['fields']['status_login']['not'] = 0;
  /* Field: Clientcert item: Encryption active */
  $handler->display->display_options['fields']['status_encrypt']['id'] = 'status_encrypt';
  $handler->display->display_options['fields']['status_encrypt']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_encrypt']['field'] = 'status_encrypt';
  $handler->display->display_options['fields']['status_encrypt']['not'] = 0;
  /* Field: Clientcert item: Public */
  $handler->display->display_options['fields']['status_public']['id'] = 'status_public';
  $handler->display->display_options['fields']['status_public']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_public']['field'] = 'status_public';
  $handler->display->display_options['fields']['status_public']['not'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Clientcert item: Clientcert item ID */
  $handler->display->display_options['arguments']['ccid']['id'] = 'ccid';
  $handler->display->display_options['arguments']['ccid']['table'] = 'clientcert';
  $handler->display->display_options['arguments']['ccid']['field'] = 'ccid';
  $handler->display->display_options['arguments']['ccid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['ccid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['ccid']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['ccid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['ccid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['ccid']['summary_options']['items_per_page'] = '25';
  $translatables['clientcert_items'] = array(
    t('Master'),
    t('Clientcert values'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Clientcert item ID'),
    t('.'),
    t(','),
    t('Clientcert item title'),
    t('Serial number'),
    t('Validity start'),
    t('Validity end'),
    t('All'),
    t('My clientcert items '),
    t('Clientcert items of user'),
    t('Block'),
    t('Clientcert item created'),
    t('Clientcert item changed'),
    t('User ID'),
    t('Client Certificate (PEM)'),
    t('Certificate hash'),
    t('Verify report'),
    t('DN subject'),
    t('DN issuer'),
    t('Active'),
    t('Login active'),
    t('Encryption active'),
    t('Public'),
  );

  $export['clientcert_items'] = $view;



  $view = new view();
  $view->name = 'clientcert_item_view';
  $view->description = 'Overrides the normal clientcert item page view.';
  $view->tag = 'default';
  $view->base_table = 'clientcert';
  $view->human_name = 'Clientcert item view';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Clientcert item page';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'clientcert';
  $handler->display->display_options['access']['perm'] = 'view';
  $handler->display->display_options['access']['clientcert_id_arg'] = '2';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'ccid' => 'ccid',
    'created' => 'created',
    'changed' => 'changed',
    'title' => 'title',
    'uid' => 'uid',
    'status_login' => 'status_login',
    'status_encrypt' => 'status_encrypt',
    'status_public' => 'status_public',
  );
  $handler->display->display_options['row_options']['separator'] = ' | ';
  /* Field: Clientcert item: Clientcert item ID */
  $handler->display->display_options['fields']['ccid']['id'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['ccid']['field'] = 'ccid';
  $handler->display->display_options['fields']['ccid']['alter']['path'] = 'clientcert/item/[ccid]';
  $handler->display->display_options['fields']['ccid']['element_label_type'] = 'strong';
  /* Field: Clientcert item: Clientcert item created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'clientcert';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Clientcert item changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'clientcert';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Clientcert item title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'clientcert';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  /* Field: Clientcert item: User ID */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'clientcert';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['path'] = 'user/[uid]';
  /* Field: Clientcert item: Client Certificate (PEM) */
  $handler->display->display_options['fields']['cert']['id'] = 'cert';
  $handler->display->display_options['fields']['cert']['table'] = 'clientcert';
  $handler->display->display_options['fields']['cert']['field'] = 'cert';
  $handler->display->display_options['fields']['cert']['element_type'] = 'div';
  $handler->display->display_options['fields']['cert']['element_class'] = 'clientcert-pem';
  $handler->display->display_options['fields']['cert']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['cert']['element_wrapper_type'] = 'div';
  /* Field: Clientcert item: Certificate hash */
  $handler->display->display_options['fields']['cert_hash']['id'] = 'cert_hash';
  $handler->display->display_options['fields']['cert_hash']['table'] = 'clientcert';
  $handler->display->display_options['fields']['cert_hash']['field'] = 'cert_hash';
  $handler->display->display_options['fields']['cert_hash']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['cert_hash']['element_wrapper_type'] = 'p';
  /* Field: Clientcert item: Verify report */
  $handler->display->display_options['fields']['verify']['id'] = 'verify';
  $handler->display->display_options['fields']['verify']['table'] = 'clientcert';
  $handler->display->display_options['fields']['verify']['field'] = 'verify';
  $handler->display->display_options['fields']['verify']['element_label_type'] = 'strong';
  /* Field: Clientcert item: Serial */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'clientcert';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['label'] = 'Serial number';
  $handler->display->display_options['fields']['serial']['element_label_type'] = 'strong';
  /* Field: Clientcert item: DN subject */
  $handler->display->display_options['fields']['dn_subject']['id'] = 'dn_subject';
  $handler->display->display_options['fields']['dn_subject']['table'] = 'clientcert';
  $handler->display->display_options['fields']['dn_subject']['field'] = 'dn_subject';
  $handler->display->display_options['fields']['dn_subject']['element_label_type'] = 'strong';
  /* Field: Clientcert item: DN issuer */
  $handler->display->display_options['fields']['dn_issuer']['id'] = 'dn_issuer';
  $handler->display->display_options['fields']['dn_issuer']['table'] = 'clientcert';
  $handler->display->display_options['fields']['dn_issuer']['field'] = 'dn_issuer';
  $handler->display->display_options['fields']['dn_issuer']['element_label_type'] = 'strong';
  /* Field: Clientcert item: Validity start */
  $handler->display->display_options['fields']['validity_start']['id'] = 'validity_start';
  $handler->display->display_options['fields']['validity_start']['table'] = 'clientcert';
  $handler->display->display_options['fields']['validity_start']['field'] = 'validity_start';
  $handler->display->display_options['fields']['validity_start']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['validity_start']['date_format'] = 'short';
  $handler->display->display_options['fields']['validity_start']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Validity end */
  $handler->display->display_options['fields']['validity_end']['id'] = 'validity_end';
  $handler->display->display_options['fields']['validity_end']['table'] = 'clientcert';
  $handler->display->display_options['fields']['validity_end']['field'] = 'validity_end';
  $handler->display->display_options['fields']['validity_end']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['validity_end']['date_format'] = 'short';
  $handler->display->display_options['fields']['validity_end']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Clientcert item: Active */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Active/verified';
  $handler->display->display_options['fields']['status']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Clientcert item: Login active */
  $handler->display->display_options['fields']['status_login']['id'] = 'status_login';
  $handler->display->display_options['fields']['status_login']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_login']['field'] = 'status_login';
  $handler->display->display_options['fields']['status_login']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['status_login']['not'] = 0;
  /* Field: Clientcert item: Encryption active */
  $handler->display->display_options['fields']['status_encrypt']['id'] = 'status_encrypt';
  $handler->display->display_options['fields']['status_encrypt']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_encrypt']['field'] = 'status_encrypt';
  $handler->display->display_options['fields']['status_encrypt']['not'] = 0;
  /* Field: Clientcert item: Public */
  $handler->display->display_options['fields']['status_public']['id'] = 'status_public';
  $handler->display->display_options['fields']['status_public']['table'] = 'clientcert';
  $handler->display->display_options['fields']['status_public']['field'] = 'status_public';
  $handler->display->display_options['fields']['status_public']['not'] = 0;
  /* Contextual filter: Clientcert item: Clientcert item ID */
  $handler->display->display_options['arguments']['ccid']['id'] = 'ccid';
  $handler->display->display_options['arguments']['ccid']['table'] = 'clientcert';
  $handler->display->display_options['arguments']['ccid']['field'] = 'ccid';
  $handler->display->display_options['arguments']['ccid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['ccid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['ccid']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['ccid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['ccid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['ccid']['summary_options']['items_per_page'] = '25';

  /* Display: Clientcert item page */
  $handler = $view->new_display('page', 'Clientcert item page', 'page');
  $handler->display->display_options['path'] = 'clientcert/item/%';
  $handler->display->display_options['menu']['title'] = 'My clientcert items';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['clientcert_item_view'] = array(
    t('Master'),
    t('Clientcert item page'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Clientcert item ID'),
    t('.'),
    t(','),
    t('created'),
    t('changed'),
    t('title'),
    t('User ID'),
    t('Client Certificate (PEM)'),
    t('Certificate hash'),
    t('Verify report'),
    t('Serial number'),
    t('DN subject'),
    t('DN issuer'),
    t('Validity start'),
    t('Validity end'),
    t('Active/verified'),
    t('Login active'),
    t('Encryption active'),
    t('Public'),
    t('All'),
  );




  $export['clientcert_item_view'] = $view;

  return $export;
}