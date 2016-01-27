<?php
/**
 * @file
 * bendy.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function bendy_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'bendy_featured_content_rotating_banner';
  $view->description = 'Displays a block of rotating featured content and images on the homepage.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Bendy: Featured content rotating banner';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
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
    'field_bendy_url' => 0,
    'field_bendy_image' => 0,
    'title' => 0,
    'field_bendy_subtitle' => 0,
    'nothing' => 0,
    'counter' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['enable'] = 1;
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['enable'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'counter' => 'counter',
    'field_bendy_url' => 0,
    'field_bendy_image' => 0,
    'title' => 0,
    'field_bendy_subtitle' => 0,
    'nothing' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['timeout'] = '5000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['speed'] = '700';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['delay'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['pause'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['action_advanced'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['start_paused'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide_days'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['pause_when_hidden'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['pause_when_hidden_type'] = 'vertical';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['amount_allowed_visible'] = '25%';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['fixed_height'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['items_per_slide'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartype'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartypenobg'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['advanced_options'] = '{"slideResize":"0","containerResize":"0"}';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no featured items.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Content: URL */
  $handler->display->display_options['fields']['field_bendy_url']['id'] = 'field_bendy_url';
  $handler->display->display_options['fields']['field_bendy_url']['table'] = 'field_data_field_bendy_url';
  $handler->display->display_options['fields']['field_bendy_url']['field'] = 'field_bendy_url';
  $handler->display->display_options['fields']['field_bendy_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_bendy_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_bendy_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_bendy_url']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_bendy_url']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_bendy_url']['field_api_classes'] = 0;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_bendy_image']['id'] = 'field_bendy_image';
  $handler->display->display_options['fields']['field_bendy_image']['table'] = 'field_data_field_bendy_image';
  $handler->display->display_options['fields']['field_bendy_image']['field'] = 'field_bendy_image';
  $handler->display->display_options['fields']['field_bendy_image']['label'] = '';
  $handler->display->display_options['fields']['field_bendy_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['path'] = '[field_bendy_url]';
  $handler->display->display_options['fields']['field_bendy_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_bendy_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_bendy_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_bendy_image']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_bendy_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_bendy_image']['type'] = 'cs_adaptive_image';
  $handler->display->display_options['fields']['field_bendy_image']['settings'] = array(
    'relationship' => 'none',
    'ui_name' => '',
    'custom_label' => 0,
    'label' => '',
    'element_label_colon' => 0,
    'exclude' => 0,
    'element_type_enable' => 0,
    'element_type' => '',
    'element_class_enable' => 0,
    'element_class' => '',
    'element_label_type_enable' => 0,
    'element_label_type' => '',
    'element_label_class_enable' => 0,
    'element_label_class' => '',
    'element_wrapper_type_enable' => 0,
    'element_wrapper_type' => '',
    'element_wrapper_class_enable' => 0,
    'element_wrapper_class' => '',
    'element_default_classes' => 1,
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'absolute' => 0,
      'replace_spaces' => 0,
      'external' => 0,
      'path_case' => 'none',
      'link_class' => '',
      'alt' => '',
      'rel' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'more_link' => 0,
      'more_link_text' => '',
      'more_link_path' => '',
      'html' => 0,
      'strip_tags' => 0,
      'preserve_tags' => '',
      'trim_whitespace' => 0,
      'nl2br' => 0,
    ),
    'empty' => '',
    'empty_zero' => 0,
    'hide_empty' => 0,
    'hide_alter_empty' => 1,
    'click_sort_column' => 'fid',
    'type' => 'cs_adaptive_image',
    'field_api_classes' => 0,
    'image_link' => '',
    'styles' => array(
      'breakpoint_1' => '480',
      'style_1' => 'bendy_mobile_small',
      'breakpoint_2' => '739',
      'style_2' => 'bendy_mobile',
      'breakpoint_3' => '979',
      'style_3' => 'bendy_narrow',
      'breakpoint_4' => '1219',
      'style_4' => 'bendy_normal',
      'breakpoint_5' => '1500',
      'style_5' => 'bendy_wide',
      'max_style' => 'bendy_wide',
      'fallback_style' => 'bendy_wide',
    ),
  );
  $handler->display->display_options['fields']['field_bendy_image']['field_api_classes'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_bendy_url]';
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['max_length'] = '80';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Subtitle */
  $handler->display->display_options['fields']['field_bendy_subtitle']['id'] = 'field_bendy_subtitle';
  $handler->display->display_options['fields']['field_bendy_subtitle']['table'] = 'field_data_field_bendy_subtitle';
  $handler->display->display_options['fields']['field_bendy_subtitle']['field'] = 'field_bendy_subtitle';
  $handler->display->display_options['fields']['field_bendy_subtitle']['label'] = '';
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['max_length'] = '60';
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['strip_tags'] = 1;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['field_bendy_subtitle']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_bendy_subtitle']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_bendy_subtitle']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_bendy_subtitle']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_bendy_subtitle']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_bendy_subtitle']['field_api_classes'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Read more';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = '[field_bendy_url]';
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['exclude'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['external'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['counter']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['counter']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['html'] = 0;
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['counter']['hide_empty'] = 0;
  $handler->display->display_options['fields']['counter']['empty_zero'] = 0;
  $handler->display->display_options['fields']['counter']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
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
    'bendy_featured_content' => 'bendy_featured_content',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $export['bendy_featured_content_rotating_banner'] = $view;

  return $export;
}
