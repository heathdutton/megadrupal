<?php
/**
 * @file
 * drupalgive.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function drupalgive_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'drupalgive';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'drupalgive';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Drupal Give';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  $handler->display->display_options['header']['area']['content'] = 'Our organization is not only using Drupal, but also contributes back to the community and to the project by writing modules and themes, participating in forums, lecturing in conferences, organizing Drupal events and more. 

This page is a tribute to the contributors. It displays the highlights of our give-backs to the community. It is our part of the <a href="http://drupal.org/drupalgive">/drupalgive</a> initiative.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_contributor_target_id']['id'] = 'field_contributor_target_id';
  $handler->display->display_options['relationships']['field_contributor_target_id']['table'] = 'field_data_field_contributor';
  $handler->display->display_options['relationships']['field_contributor_target_id']['field'] = 'field_contributor_target_id';
  $handler->display->display_options['relationships']['field_contributor_target_id']['required'] = 0;
  /* Field: User: Member Picture */
  $handler->display->display_options['fields']['field_memberpic']['id'] = 'field_memberpic';
  $handler->display->display_options['fields']['field_memberpic']['table'] = 'field_data_field_memberpic';
  $handler->display->display_options['fields']['field_memberpic']['field'] = 'field_memberpic';
  $handler->display->display_options['fields']['field_memberpic']['relationship'] = 'field_contributor_target_id';
  $handler->display->display_options['fields']['field_memberpic']['label'] = '';
  $handler->display->display_options['fields']['field_memberpic']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_memberpic']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_memberpic']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_memberpic']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_memberpic']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_memberpic']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_memberpic']['settings'] = array(
    'image_style' => 'drupal_give_35-50',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_memberpic']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_memberpic']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_memberpic']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_memberpic']['separator'] = '';
  $handler->display->display_options['fields']['field_memberpic']['field_api_classes'] = 0;
  /* Field: Content: Contributor role */
  $handler->display->display_options['fields']['field_contributor_role']['id'] = 'field_contributor_role';
  $handler->display->display_options['fields']['field_contributor_role']['table'] = 'field_data_field_contributor_role';
  $handler->display->display_options['fields']['field_contributor_role']['field'] = 'field_contributor_role';
  $handler->display->display_options['fields']['field_contributor_role']['label'] = '';
  $handler->display->display_options['fields']['field_contributor_role']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_contributor_role']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_contributor_role']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_contributor_role']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_contributor_role']['field_api_classes'] = 0;
  /* Field: Content: Contributor */
  $handler->display->display_options['fields']['field_contributor']['id'] = 'field_contributor';
  $handler->display->display_options['fields']['field_contributor']['table'] = 'field_data_field_contributor';
  $handler->display->display_options['fields']['field_contributor']['field'] = 'field_contributor';
  $handler->display->display_options['fields']['field_contributor']['label'] = '';
  $handler->display->display_options['fields']['field_contributor']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_contributor']['alter']['text'] = '<strong>[field_contributor]</strong><br /> ([field_contributor_role])';
  $handler->display->display_options['fields']['field_contributor']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_contributor']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_contributor']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_contributor']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_contributor']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_contributor']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_contributor']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_contributor']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_contributor']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_contributor']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_contributor']['field_api_classes'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
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
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['body']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['body']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '175';
  $handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['body']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['body']['alter']['html'] = 0;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['body']['field_api_classes'] = 0;
  /* Field: Content: Link to project */
  $handler->display->display_options['fields']['field_contrib_link']['id'] = 'field_contrib_link';
  $handler->display->display_options['fields']['field_contrib_link']['table'] = 'field_data_field_contrib_link';
  $handler->display->display_options['fields']['field_contrib_link']['field'] = 'field_contrib_link';
  $handler->display->display_options['fields']['field_contrib_link']['label'] = '';
  $handler->display->display_options['fields']['field_contrib_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_contrib_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_contrib_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_contrib_link']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_contrib_link']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_contrib_link']['field_api_classes'] = 0;
  /* Field: Node: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
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
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 1;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['granularity'] = 'month';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'drupalgive_contribution' => 'drupalgive_contribution',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'drupalgive';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Drupalgive';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'drupalgive/rss';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $handler->display->display_options['sitename_title'] = 0;
  $translatables['drupalgive'] = array(
    t('Master'),
    t('Drupal Give'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Our organization is not only using Drupal, but also contributes back to the community and to the project by writing modules and themes, participating in forums, lecturing in conferences, organizing Drupal events and more. 

This page is a tribute to the contributors. It displays the highlights of our give-backs to the community. It is our part of the <a href="http://drupal.org/drupalgive">/drupalgive</a> initiative.'),
    t('User entity referenced from field_contributor'),
    t('<strong>[field_contributor]</strong><br /> ([field_contributor_role])'),
    t('Page'),
    t('Feed'),
    t('Linnovate Drupalgive'),
  );
  $export['drupalgive'] = $view;

  return $export;
}
