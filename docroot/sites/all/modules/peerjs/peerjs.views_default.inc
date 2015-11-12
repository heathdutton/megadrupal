<?php
/**
 * @file
 * Default views for the PeerJS Chat module.
 */

/**
 * Implements hook_views_default_views().
 */
function peerjs_views_default_views() {
  $views = array();

  $views['peerjs_chat'] = peerjs_chat_view();

  return $views;
}

/**
 * Provides the peerjs_chat view.
 * @see peerjs_views_default_views()
 */
function peerjs_chat_view() {
  $view = new view();
  $view->name = 'peerjs_chat';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'peerjs_chat';
  $view->human_name = 'PeerJS Chat';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer peerjs';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'cid' => 'cid',
    'label' => 'label',
    'name' => 'name',
    'created' => 'created',
    'host' => 'host',
    'port' => 'port',
    'path2app' => 'path2app',
    'nothing' => 'nothing',
    'nothing_1' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'cid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'host' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'port' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'path2app' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Chat Room: Author uid */
  $handler->display->display_options['relationships']['author']['id'] = 'author';
  $handler->display->display_options['relationships']['author']['table'] = 'peerjs_chat';
  $handler->display->display_options['relationships']['author']['field'] = 'author';
  /* Field: Chat Room: Chat ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'peerjs_chat';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  /* Field: Chat Room: Chat Room Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'peerjs_chat';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Title';
  $handler->display->display_options['fields']['label']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['label']['alter']['path'] = 'peerjs_chat/[cid]';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'author';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  /* Field: Chat Room: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'peerjs_chat';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Chat Room: Host */
  $handler->display->display_options['fields']['host']['id'] = 'host';
  $handler->display->display_options['fields']['host']['table'] = 'peerjs_chat';
  $handler->display->display_options['fields']['host']['field'] = 'host';
  /* Field: Chat Room: Port */
  $handler->display->display_options['fields']['port']['id'] = 'port';
  $handler->display->display_options['fields']['port']['table'] = 'peerjs_chat';
  $handler->display->display_options['fields']['port']['field'] = 'port';
  /* Field: Chat Room: Path to Application */
  $handler->display->display_options['fields']['path2app']['id'] = 'path2app';
  $handler->display->display_options['fields']['path2app']['table'] = 'peerjs_chat';
  $handler->display->display_options['fields']['path2app']['field'] = 'path2app';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'peerjs_chat/[cid]/edit?destination=admin/content/peerjs_chat';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'peerjs_chat/[cid]/delete?destination=admin/content/peerjs_chat';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'peerjs_chat_page');
  $handler->display->display_options['path'] = 'admin/content/peerjs_chat';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Chat Rooms';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  return $view;
}
