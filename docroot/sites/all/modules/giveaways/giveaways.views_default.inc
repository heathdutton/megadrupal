<?php
/**
 * @file
 * giveaways.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function giveaways_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'giveaway_keys';
  $view->description = 'Default view for data table Keys to be given away';
  $view->tag = 'data table';
  $view->base_table = 'giveaway_keys';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Default */
  $handler = $view->new_display('default', 'Default', 'default');
  $handler->display->display_options['title'] = 'Keys to be given away';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit any giveaway content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'gkid' => 'gkid',
    'gid' => 'gid',
    'game_key' => 'game_key',
    'name' => 'name',
    'ip_address' => 'ip_address',
    'claimed_on' => 'claimed_on',
    'edit_link' => 'edit_link',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'gkid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'gid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'game_key' => array(
      'sortable' => 1,
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
    'ip_address' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'claimed_on' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'area';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['content'] = 'There is no data in this table.';
  $handler->display->display_options['empty']['text']['format'] = '1';
  /* Relationship: Keys to be given away: The user who has claimed it. */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'giveaway_keys';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  /* Field: Keys to be given away: Key Id */
  $handler->display->display_options['fields']['gkid']['id'] = 'gkid';
  $handler->display->display_options['fields']['gkid']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['gkid']['field'] = 'gkid';
  $handler->display->display_options['fields']['gkid']['label'] = 'gkid';
  $handler->display->display_options['fields']['gkid']['exclude'] = TRUE;
  /* Field: Keys to be given away: Giveaway Id */
  $handler->display->display_options['fields']['gid']['id'] = 'gid';
  $handler->display->display_options['fields']['gid']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['gid']['field'] = 'gid';
  $handler->display->display_options['fields']['gid']['exclude'] = TRUE;
  /* Field: Keys to be given away: Game Key */
  $handler->display->display_options['fields']['game_key']['id'] = 'game_key';
  $handler->display->display_options['fields']['game_key']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['game_key']['field'] = 'game_key';
  $handler->display->display_options['fields']['game_key']['label'] = 'Key';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Keys to be given away: The IP to which it has been given. */
  $handler->display->display_options['fields']['ip_address']['id'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['ip_address']['field'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address']['label'] = 'IP Address';
  /* Field: Keys to be given away: Timestamp when it was claimed. */
  $handler->display->display_options['fields']['claimed_on']['id'] = 'claimed_on';
  $handler->display->display_options['fields']['claimed_on']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['claimed_on']['field'] = 'claimed_on';
  $handler->display->display_options['fields']['claimed_on']['label'] = 'Claimed On';
  $handler->display->display_options['fields']['claimed_on']['date_format'] = 'long';
  $handler->display->display_options['fields']['claimed_on']['second_date_format'] = 'long';
  /* Field: Keys to be given away: Edit link */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'giveaway_keys';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['label'] = '';
  $handler->display->display_options['fields']['edit_link']['element_label_colon'] = FALSE;
  /* Contextual filter: Keys to be given away: Giveaway Id */
  $handler->display->display_options['arguments']['gid']['id'] = 'gid';
  $handler->display->display_options['arguments']['gid']['table'] = 'giveaway_keys';
  $handler->display->display_options['arguments']['gid']['field'] = 'gid';
  $handler->display->display_options['arguments']['gid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['gid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['gid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['gid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['gid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['gid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['gid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['gid']['validate_options']['types'] = array(
    'giveaway' => 'giveaway',
  );
  /* Filter criterion: Keys to be given away: Game Key */
  $handler->display->display_options['filters']['game_key']['id'] = 'game_key';
  $handler->display->display_options['filters']['game_key']['table'] = 'giveaway_keys';
  $handler->display->display_options['filters']['game_key']['field'] = 'game_key';
  $handler->display->display_options['filters']['game_key']['operator'] = 'contains';
  $handler->display->display_options['filters']['game_key']['exposed'] = TRUE;
  $handler->display->display_options['filters']['game_key']['expose']['operator_id'] = 'game_key_op';
  $handler->display->display_options['filters']['game_key']['expose']['label'] = 'Key';
  $handler->display->display_options['filters']['game_key']['expose']['operator'] = 'game_key_op';
  $handler->display->display_options['filters']['game_key']['expose']['identifier'] = 'game_key';
  $handler->display->display_options['filters']['game_key']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'uid';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Username';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'node/%/keys';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Keys';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['giveaway_keys'] = $view;

  return $export;
}
