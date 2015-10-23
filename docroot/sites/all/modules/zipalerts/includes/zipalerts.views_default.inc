<?php

/**
 * @file
 * This module will import a views for the ZipAlters Implementation.
 */

/**
 * Implements hook_views_default_views().
 */
function zipalerts_views_default_views() {
  $view = new view();
  $view->name = 'zip_alerts_integration';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Zip Alerts Integration';
  $view->core = 7;
  $view->api_version = '3.0';
  /* Edit this to true to make a default view disabled initially */
  $view->disabled = FALSE;

  /* Display: Master */

  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Nid */

  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'referencenumber';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */

  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Nid */

  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['label'] = 'url';
  $handler->display->display_options['fields']['nid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid_1']['alter']['text'] = '<url>"www.YourWebsite.com/node/[nid]</url>';
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  /* Field: Content: Body */

  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'description';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_plain';
  /* Field: Content: Location */

  $handler->display->display_options['fields']['field_job_location']['id'] = 'field_job_location';
  $handler->display->display_options['fields']['field_job_location']['table'] = 'field_data_field_job_location';
  $handler->display->display_options['fields']['field_job_location']['field'] = 'field_job_location';
  $handler->display->display_options['fields']['field_job_location']['label'] = 'city';
  /* Field: Content: Organization */

  $handler->display->display_options['fields']['field_job_organization']['id'] = 'field_job_organization';
  $handler->display->display_options['fields']['field_job_organization']['table'] = 'field_data_field_job_organization';
  $handler->display->display_options['fields']['field_job_organization']['field'] = 'field_job_organization';
  $handler->display->display_options['fields']['field_job_organization']['label'] = 'company';
  $handler->display->display_options['fields']['field_job_organization']['type'] = 'text_plain';
  /* Field: Content: Region */

  $handler->display->display_options['fields']['field_job_region']['id'] = 'field_job_region';
  $handler->display->display_options['fields']['field_job_region']['table'] = 'field_data_field_job_region';
  $handler->display->display_options['fields']['field_job_region']['field'] = 'field_job_region';
  $handler->display->display_options['fields']['field_job_region']['label'] = 'state';
  $handler->display->display_options['fields']['field_job_region']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_job_region']['delta_offset'] = '0';
  /* Field: Global: Custom text */

  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'country';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'United States';
  /* Field: Content: Post date */

  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Global: Custom text */

  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'site';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Example Job Board';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */

  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */

  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Data export */

  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xml';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['transform'] = 1;
  $handler->display->display_options['style_options']['root_node'] = 'source';
  $handler->display->display_options['style_options']['item_node'] = 'job';
  $handler->display->display_options['path'] = 'zipalters/jobs.xml';
  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.
  // At the end, return array of default views.
  return $views;
}
