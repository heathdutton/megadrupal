<?php

/**
 * @file
 * Views default file for GA Push (browser) module.
 */

/**
 * Implements hook_views_default_views().
 */
function ga_push_browser_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'ga_push_browser_event';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ga_push_browser_event';
  $view->human_name = 'Ga push browser events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Ga push browser events';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'más';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'admin ga push';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reiniciar';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Ordenar por';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Ascendente';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementos por página';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Todos -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Desplazamiento';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no GA Push browser events defined.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Ga Push Browser entity: Ga push browser entity ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: Ga Push Browser entity: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Nombre';
  /* Field: Ga Push Browser entity: Selector */
  $handler->display->display_options['fields']['selector']['id'] = 'selector';
  $handler->display->display_options['fields']['selector']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['selector']['field'] = 'selector';
  /* Field: Ga Push Browser entity: Bind */
  $handler->display->display_options['fields']['bind']['id'] = 'bind';
  $handler->display->display_options['fields']['bind']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['bind']['field'] = 'bind';
  /* Field: Ga Push Browser entity: Ga_category */
  $handler->display->display_options['fields']['ga_category']['id'] = 'ga_category';
  $handler->display->display_options['fields']['ga_category']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['ga_category']['field'] = 'ga_category';
  /* Field: Ga Push Browser entity: Ga_action */
  $handler->display->display_options['fields']['ga_action']['id'] = 'ga_action';
  $handler->display->display_options['fields']['ga_action']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['ga_action']['field'] = 'ga_action';
  /* Field: Ga Push Browser entity: Ga_label */
  $handler->display->display_options['fields']['ga_label']['id'] = 'ga_label';
  $handler->display->display_options['fields']['ga_label']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['ga_label']['field'] = 'ga_label';
  /* Field: Ga Push Browser entity: Ga_value */
  $handler->display->display_options['fields']['ga_value']['id'] = 'ga_value';
  $handler->display->display_options['fields']['ga_value']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['ga_value']['field'] = 'ga_value';
  /* Field: Ga Push Browser entity: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'ga_push_browser_event';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Estado';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'admin/config/system/ga-push/browser/[id]/edit';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'admin/config/system/ga-push/browser/[id]/delete';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/config/system/ga-push/browser';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'GA Push Browser';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['context'] = 0;

  $views[$view->name] = $view;

  return $views;
}
