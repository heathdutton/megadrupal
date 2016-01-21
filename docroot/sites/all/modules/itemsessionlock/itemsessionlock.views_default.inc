<?php

/**
 * @file
 * Defines administrative views for locks.
 */

/**
 * Implements hook_views_default_views().
 * @todo Missing translatables.
 */
function itemsessionlock_views_default_views() {
  $views = array();
  // Main global admin view.
  $view = new view();
  $view->name = 'itemsessionlock_locks';
  $view->description = t('Administative view to manage Locks using Item session lock module');
  $view->tag = 'default';
  $view->base_table = 'itemsessionlock';
  $view->human_name = t('Items Session Locks');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = t('Items Session Locks');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'break any session lock';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Item session locks: Lock owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'itemsessionlock';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Item session locks: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'itemsessionlock';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Item session locks: Lock item type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'itemsessionlock';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = t('Owner');
  /* Field: Item session locks: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'itemsessionlock';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Field: Item session locks: Break link */
  $handler->display->display_options['fields']['break_link']['id'] = 'break_link';
  $handler->display->display_options['fields']['break_link']['table'] = 'itemsessionlock';
  $handler->display->display_options['fields']['break_link']['field'] = 'break_link';
  $handler->display->display_options['fields']['break_link']['label'] = 'Break lock';
  /* Sort criterion: Item session locks: Timestamp */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'itemsessionlock';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  /* Filter criterion: Item session locks: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'itemsessionlock';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['operator'] = 'contains';
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  $handler->display->display_options['filters']['module']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Item session locks: Lock item type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'itemsessionlock';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'contains';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Lock item type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Item session locks: Timestamp */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'itemsessionlock';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['operator'] = '<=';
  $handler->display->display_options['filters']['timestamp']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = t('Older than');
  $handler->display->display_options['filters']['timestamp']['expose']['description'] = t('An offset from the current time such as "-3 days" or "-2 weeks"');
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/people/itemsessionlock';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Items Session Locks';
  $handler->display->display_options['menu']['name'] = 'management';
  $views[$view->name] = $view;

  // Views created on behalf implementing modules.
  foreach (module_implements('itemsessionlock_info') as $module) {
    $function = $module . '_itemsessionlock_info';
    $info = $function();
    foreach ($info as $type => $implement) {
      if (!isset($implement['view']) || $implement['view']) {
        $view = new view();
        $view->name = $module . '_' . $type . '_itemsessionlock';
        $view->description = t('Administative view to manage @module @type locks', array('@module' => $module, '@type' => $implement['label']));
        $view->tag = 'default';
        $view->base_table = 'itemsessionlock';
        $view->human_name = t('@module @type locks', array('@module' => ucfirst($module), '@type' => $implement['label']));
        $view->core = 7;
        $view->api_version = '3.0';
        $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

        /* Display: Master */
        $handler = $view->new_display('default', 'Master', 'default');
        $handler->display->display_options['title'] = 'Items Session Locks';
        $handler->display->display_options['use_more_always'] = FALSE;
        $handler->display->display_options['access']['type'] = 'perm';
        $handler->display->display_options['access']['perm'] = 'break any ' . $module . ' ' . $type . ' locks';
        $handler->display->display_options['cache']['type'] = 'none';
        $handler->display->display_options['query']['type'] = 'views_query';
        $handler->display->display_options['exposed_form']['type'] = 'basic';
        $handler->display->display_options['pager']['type'] = 'full';
        $handler->display->display_options['pager']['options']['items_per_page'] = '50';
        $handler->display->display_options['style_plugin'] = 'table';
        /* Relationship: Item session locks: Lock owner */
        $handler->display->display_options['relationships']['uid']['id'] = 'uid';
        $handler->display->display_options['relationships']['uid']['table'] = 'itemsessionlock';
        $handler->display->display_options['relationships']['uid']['field'] = 'uid';
        /* Field: User: Name */
        $handler->display->display_options['fields']['name']['id'] = 'name';
        $handler->display->display_options['fields']['name']['table'] = 'users';
        $handler->display->display_options['fields']['name']['field'] = 'name';
        $handler->display->display_options['fields']['name']['relationship'] = 'uid';
        $handler->display->display_options['fields']['name']['label'] = 'Owner';
        /* Field: Item session locks: Timestamp */
        $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
        $handler->display->display_options['fields']['timestamp']['table'] = 'itemsessionlock';
        $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
        $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
        $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
        /* Sort criterion: Item session locks: Timestamp */
        $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
        $handler->display->display_options['sorts']['timestamp']['table'] = 'itemsessionlock';
        $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
        /* Field: Item session locks: Break link */
        $handler->display->display_options['fields']['break_link']['id'] = 'break_link';
        $handler->display->display_options['fields']['break_link']['table'] = 'itemsessionlock';
        $handler->display->display_options['fields']['break_link']['field'] = 'break_link';
        $handler->display->display_options['fields']['break_link']['label'] = 'Break lock';

        /* Filter criterion: Item session locks: Module */
        $handler->display->display_options['filters']['module']['id'] = 'module';
        $handler->display->display_options['filters']['module']['table'] = 'itemsessionlock';
        $handler->display->display_options['filters']['module']['field'] = 'module';
        $handler->display->display_options['filters']['module']['value'] = $module;
        $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
        $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
        $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
        $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
        $handler->display->display_options['filters']['module']['expose']['remember_roles'] = array(
          2 => '2',
          1 => 0,
        );
        /* Filter criterion: Item session locks: Lock item type */
        $handler->display->display_options['filters']['type']['id'] = 'type';
        $handler->display->display_options['filters']['type']['table'] = 'itemsessionlock';
        $handler->display->display_options['filters']['type']['field'] = 'type';
        $handler->display->display_options['filters']['type']['value'] = $type;
        $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
        $handler->display->display_options['filters']['type']['expose']['label'] = 'Lock item type';
        $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
        $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
        $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
          2 => '2',
          1 => 0,
        );
        /* Filter criterion: User: Name */
        $handler->display->display_options['filters']['uid']['id'] = 'uid';
        $handler->display->display_options['filters']['uid']['table'] = 'users';
        $handler->display->display_options['filters']['uid']['field'] = 'uid';
        $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
        $handler->display->display_options['filters']['uid']['value'] = '';
        $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
        $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
        $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
        $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
        $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
        $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
          2 => '2',
          1 => 0,
        );
        /* Filter criterion: Item session locks: Timestamp */
        $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
        $handler->display->display_options['filters']['timestamp']['table'] = 'itemsessionlock';
        $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
        $handler->display->display_options['filters']['timestamp']['operator'] = '<=';
        $handler->display->display_options['filters']['timestamp']['value']['type'] = 'offset';
        $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
        $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
        $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Older than';
        $handler->display->display_options['filters']['timestamp']['expose']['description'] = 'An offset from the current time such as "-3 days" or "-2 weeks" ';
        $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
        $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
        $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
          2 => '2',
          1 => 0,
        );

        /* Display: Page */
        $handler = $view->new_display('page', 'Page', 'page');
        $handler->display->display_options['path'] = 'admin/people/' . $module . '-' . $type . '-locks';
        $handler->display->display_options['menu']['type'] = 'normal';
        $handler->display->display_options['menu']['title'] = t('@module @type locks', array('@module' => ucfirst($module), '@type' => $implement['label']));
        $handler->display->display_options['menu']['name'] = 'management';
        $views[$view->name] = $view;
      }
    }
  }
  return $views;
}
