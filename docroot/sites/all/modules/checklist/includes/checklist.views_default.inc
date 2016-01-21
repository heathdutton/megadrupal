<?php
/**
 * @file
 * Contains default views for checklist module.
 */

/**
 * Implements hook_views_default_views().
 */
function checklist_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'checklist_items_completed';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'checklist_items';
  $view->human_name = 'Recently completed checklist items';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'checked_on' => 'checked_on',
    'title_1' => 'title_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'checked_on' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Checklist: Checked by */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'checklist_user_checks';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 1;
  /* Relationship: Checklist: Checklist */
  $handler->display->display_options['relationships']['clid']['id'] = 'clid';
  $handler->display->display_options['relationships']['clid']['table'] = 'checklist_items';
  $handler->display->display_options['relationships']['clid']['field'] = 'clid';
  $handler->display->display_options['relationships']['clid']['required'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Checklist: Checklist title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'checklist_lists';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'clid';
  $handler->display->display_options['fields']['title']['label'] = 'Checklist';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  /* Field: Checklist: Item title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'checklist_items';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Item';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title_1']['empty_zero'] = 0;
  /* Field: Checklist: Checked On */
  $handler->display->display_options['fields']['checked_on']['id'] = 'checked_on';
  $handler->display->display_options['fields']['checked_on']['table'] = 'checklist_user_checks';
  $handler->display->display_options['fields']['checked_on']['field'] = 'checked_on';
  $handler->display->display_options['fields']['checked_on']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['external'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['checked_on']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['checked_on']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['checked_on']['alter']['html'] = 0;
  $handler->display->display_options['fields']['checked_on']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['checked_on']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['checked_on']['hide_empty'] = 0;
  $handler->display->display_options['fields']['checked_on']['empty_zero'] = 0;
  $handler->display->display_options['fields']['checked_on']['date_format'] = 'long';
  /* Sort criterion: Checklist: Checked On */
  $handler->display->display_options['sorts']['checked_on']['id'] = 'checked_on';
  $handler->display->display_options['sorts']['checked_on']['table'] = 'checklist_user_checks';
  $handler->display->display_options['sorts']['checked_on']['field'] = 'checked_on';
  $handler->display->display_options['sorts']['checked_on']['order'] = 'DESC';
  $translatables['checklist_items_completed'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Checked by'),
    t('Checklist'),
    t('User'),
    t('Item'),
    t('Checked On'),
  );
  $views[$view->name] = $view;

  return $views;
}
