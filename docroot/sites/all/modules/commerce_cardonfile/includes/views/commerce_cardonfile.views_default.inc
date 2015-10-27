<?php

/**
 * @file
 * Views for the default Commerce Card on File.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_cardonfile_views_default_views() {
  $views = array();

  // Stored cards: User
  $view = new view();
  $view->name = 'commerce_card_on_file_user_cards';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_cardonfile';
  $view->human_name = 'Commerce Card on File: User Cards';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Stored cards';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own card data';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'card_type' => 'card_type',
    'card_number' => 'card_number',
    'card_name' => 'card_name',
    'instance_default' => 'instance_default',
    'card_exp_month' => 'card_exp_month',
    'card_exp_year' => 'card_exp_year',
    'nothing' => 'nothing',
    'commerce_cardonfile_profile' => 'commerce_cardonfile_profile',
    'edit_card' => 'edit_card',
    'delete_card' => 'edit_card',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'card_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'card_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'card_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'instance_default' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'card_exp_month' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'card_exp_year' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_cardonfile_profile' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_card' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'delete_card' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Commerce Card on File: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Credit Card';
  /* Field: Commerce Card on File: Card name */
  $handler->display->display_options['fields']['card_name']['id'] = 'card_name';
  $handler->display->display_options['fields']['card_name']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['fields']['card_name']['field'] = 'card_name';
  $handler->display->display_options['fields']['card_name']['label'] = 'Name on Card';
  /* Field: Commerce Card on File: Instance default */
  $handler->display->display_options['fields']['instance_default']['id'] = 'instance_default';
  $handler->display->display_options['fields']['instance_default']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['fields']['instance_default']['field'] = 'instance_default';
  $handler->display->display_options['fields']['instance_default']['label'] = 'Default';
  $handler->display->display_options['fields']['instance_default']['not'] = 0;
  /* Field: Commerce Card on File: Card expiration */
  $handler->display->display_options['fields']['card_exp']['id'] = 'card_exp';
  $handler->display->display_options['fields']['card_exp']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['fields']['card_exp']['field'] = 'card_exp';
  $handler->display->display_options['fields']['card_exp']['label'] = 'Expires On';
  /* Field: Commerce Card on File: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['add_destination'] = 0;
  /* Sort criterion: Commerce Card on File: Instance default */
  $handler->display->display_options['sorts']['instance_default']['id'] = 'instance_default';
  $handler->display->display_options['sorts']['instance_default']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['sorts']['instance_default']['field'] = 'instance_default';
  /* Contextual filter: Commerce Card on File: User uid */
  $handler->display->display_options['arguments']['user']['id'] = 'user';
  $handler->display->display_options['arguments']['user']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['arguments']['user']['field'] = 'user';
  $handler->display->display_options['arguments']['user']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['user']['exception']['value'] = '';
  $handler->display->display_options['arguments']['user']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['user']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['user']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['user']['validate']['type'] = 'commerce_cof_owner';
  /* Filter criterion: Commerce Card on File: Card Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_cardonfile';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/cards';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Stored cards';
  $handler->display->display_options['menu']['description'] = 'Manage your stored card data.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $views[$view->name] = $view;

  return $views;
}
