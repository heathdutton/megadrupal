<?php
/**
 * @file
 * nm_testimonials.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function nm_testimonials_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'nm_testimonials';
  $view->description = '';
  $view->tag = 'nodemaker, testimonials';
  $view->base_table = 'node';
  $view->human_name = 'Testimonials';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Testimonials';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<a href="/node/add/nm-testimonial" title="Write your own testimonial!" class="button">Write your own testimonial!</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '<a href="/node/add/nm-testimonial" title="Write your own testimonial!" class="button">Write your own testimonial!</a>';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  /* Relationship: Content: Vote results */
  $handler->display->display_options['relationships']['votingapi_cache']['id'] = 'votingapi_cache';
  $handler->display->display_options['relationships']['votingapi_cache']['table'] = 'node';
  $handler->display->display_options['relationships']['votingapi_cache']['field'] = 'votingapi_cache';
  $handler->display->display_options['relationships']['votingapi_cache']['votingapi'] = array(
    'value_type' => '',
    'tag' => 'rate',
    'function' => '',
  );
  /* Field: Content: Rate */
  $handler->display->display_options['fields']['field_nm_rate']['id'] = 'field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['table'] = 'field_data_field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['field'] = 'field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['label'] = '';
  $handler->display->display_options['fields']['field_nm_rate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_rate']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_nm_rate']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'default',
    ),
    'expose' => 1,
    'style' => 'average',
    'text' => 'none',
  );
  /* Field: Content: Body */
  $handler->display->display_options['fields']['field_nm_body']['id'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['table'] = 'field_data_field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['field'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['label'] = '';
  $handler->display->display_options['fields']['field_nm_body']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
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
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'nm_testimonial' => 'nm_testimonial',
  );
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['path'] = 'testimonials';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Testimonials';
  $handler->display->display_options['menu']['description'] = 'Testimonials';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  
  /* Display: Block: Random 1 */
  $handler = $view->new_display('block', 'Block: Random 1', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'All Testimonials';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '<a class="button" href="/node/add/nm-testimonial" title="Write your own testimonial!">Write your own testimonial!</a>';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Rate */
  $handler->display->display_options['fields']['field_nm_rate']['id'] = 'field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['table'] = 'field_data_field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['field'] = 'field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['label'] = '';
  $handler->display->display_options['fields']['field_nm_rate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_rate']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_nm_rate']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'default',
    ),
    'expose' => 1,
    'style' => 'average',
    'text' => 'none',
  );
  /* Field: Content: Body */
  $handler->display->display_options['fields']['field_nm_body']['id'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['table'] = 'field_data_field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['field'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['label'] = '';
  $handler->display->display_options['fields']['field_nm_body']['alter']['max_length'] = '300';
  $handler->display->display_options['fields']['field_nm_body']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  
  /* Display: Block: Front page */
  $handler = $view->new_display('block', 'Block: Front page', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'All Testimonials';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Rate */
  $handler->display->display_options['fields']['field_nm_rate']['id'] = 'field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['table'] = 'field_data_field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['field'] = 'field_nm_rate';
  $handler->display->display_options['fields']['field_nm_rate']['label'] = '';
  $handler->display->display_options['fields']['field_nm_rate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_rate']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_nm_rate']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'default',
    ),
    'expose' => 1,
    'style' => 'average',
    'text' => 'none',
  );
  /* Field: Content: Body */
  $handler->display->display_options['fields']['field_nm_body']['id'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['table'] = 'field_data_field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['field'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['label'] = '';
  $handler->display->display_options['fields']['field_nm_body']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_testimonial' => 'nm_testimonial',
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = '1';

  $export['nm_testimonials'] = $view;

  return $export;
}
