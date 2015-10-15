<?php
/**
 * @file
 * enterprise_rotator.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function enterprise_rotator_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'oe_rotating_banner';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Rotating banner';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'rotating-banner';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'slideshow';
  $handler->display->display_options['style_options']['slideshow_type'] = 'views_slideshow_cycle';
  $handler->display->display_options['style_options']['slideshow_skin'] = 'default';
  $handler->display->display_options['style_options']['skin_info'] = array(
    'class' => 'default',
    'name' => 'Default',
    'module' => 'views_slideshow',
    'path' => '',
    'stylesheets' => array(),
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'title' => 0,
    'field_oe_slide_text' => 0,
    'field_oe_slide_image' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'title' => 0,
    'field_oe_slide_text' => 0,
    'field_oe_slide_image' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['transition_advanced'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['timeout'] = '3000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['speed'] = '700';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['delay'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['start_paused'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide_days'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['items_per_slide'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartype'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartypenobg'] = 0;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'banner-title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Banner Text */
  $handler->display->display_options['fields']['field_oe_slide_text']['id'] = 'field_oe_slide_text';
  $handler->display->display_options['fields']['field_oe_slide_text']['table'] = 'field_data_field_oe_slide_text';
  $handler->display->display_options['fields']['field_oe_slide_text']['field'] = 'field_oe_slide_text';
  $handler->display->display_options['fields']['field_oe_slide_text']['label'] = '';
  $handler->display->display_options['fields']['field_oe_slide_text']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['max_length'] = '500';
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_oe_slide_text']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_oe_slide_text']['element_wrapper_class'] = 'banner-text';
  $handler->display->display_options['fields']['field_oe_slide_text']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_text']['field_api_classes'] = 0;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['field_oe_slide_link']['id'] = 'field_oe_slide_link';
  $handler->display->display_options['fields']['field_oe_slide_link']['table'] = 'field_data_field_oe_slide_link';
  $handler->display->display_options['fields']['field_oe_slide_link']['field'] = 'field_oe_slide_link';
  $handler->display->display_options['fields']['field_oe_slide_link']['label'] = '';
  $handler->display->display_options['fields']['field_oe_slide_link']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_oe_slide_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_link']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_oe_slide_link']['type'] = 'plain';
  $handler->display->display_options['fields']['field_oe_slide_link']['field_api_classes'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<h2 class="banner-title">[title]</h2>
<div class="banner-body">[field_oe_slide_text]</div>';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['nothing']['element_wrapper_class'] = 'banner-text';
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_oe_slide_image']['id'] = 'field_oe_slide_image';
  $handler->display->display_options['fields']['field_oe_slide_image']['table'] = 'field_data_field_oe_slide_image';
  $handler->display->display_options['fields']['field_oe_slide_image']['field'] = 'field_oe_slide_image';
  $handler->display->display_options['fields']['field_oe_slide_image']['label'] = '';
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['path'] = '[field_oe_slide_link]';
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['absolute'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_oe_slide_image']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_oe_slide_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_oe_slide_image']['settings'] = array(
    'image_style' => 'oe_slide',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_oe_slide_image']['field_api_classes'] = 0;
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
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'oe_slide' => 'oe_slide',
  );

  /* Display: Rotating Banner */
  $handler = $view->new_display('block', 'Rotating Banner', 'block');
  $export['oe_rotating_banner'] = $view;

  return $export;
}
