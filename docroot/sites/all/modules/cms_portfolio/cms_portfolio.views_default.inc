<?php
/**
 * @file
 * cms_portfolio.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cms_portfolio_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cms_portfolio';
  $view->description = '';
  $view->tag = 'cms';
  $view->base_table = 'node';
  $view->human_name = 'CMS Portfolio';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '16';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '3';
  $handler->display->display_options['style_options']['columns_lg'] = '3';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col12_square',
    'image_link' => 'content',
    'field_formatter_class' => '',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
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
    'portfolio' => 'portfolio',
  );

  /* Display: Portfolio block */
  $handler = $view->new_display('block', 'Portfolio block', 'portfolio_block');
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '4';
  $handler->display->display_options['style_options']['columns_lg'] = '4';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6_square',
    'image_link' => 'content',
    'field_formatter_class' => '',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;

  /* Display: Portfolio */
  $handler = $view->new_display('page', 'Portfolio', 'portfolio');
  $handler->display->display_options['path'] = 'portfolio';
  $handler->display->display_options['menu']['title'] = 'Portfolio';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio 1 Column */
  $handler = $view->new_display('page', 'Portfolio 1 Column', 'portfolio_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio 1 Column';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '0';
  $handler->display->display_options['style_options']['columns_md'] = '0';
  $handler->display->display_options['style_options']['columns_lg'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col12',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/1';
  $handler->display->display_options['menu']['title'] = 'Portfolio 1 Column';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio 2 Columns */
  $handler = $view->new_display('page', 'Portfolio 2 Columns', 'portfolio_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio 2 Columns';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '0';
  $handler->display->display_options['style_options']['columns_lg'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/2';
  $handler->display->display_options['menu']['title'] = 'Portfolio 2 Columns';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio 3 Columns */
  $handler = $view->new_display('page', 'Portfolio 3 Columns', 'portfolio_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio 3 Columns';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '4';
  $handler->display->display_options['style_options']['columns_lg'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/3';
  $handler->display->display_options['menu']['title'] = 'Portfolio 3 Columns';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio 1 Column Excerpt */
  $handler = $view->new_display('page', 'Portfolio 1 Column Excerpt', 'portfolio_1excerpt');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio 1 Column with Excerpt';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'cms-portfolio-excerpt';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '0';
  $handler->display->display_options['style_options']['columns_md'] = '0';
  $handler->display->display_options['style_options']['columns_lg'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col12',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_type'] = 'p';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'smart_trim_format';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_link' => '0',
    'trim_length' => '300',
    'trim_type' => 'chars',
    'trim_suffix' => '...',
    'more_link' => '0',
    'more_text' => 'Read more',
    'summary_handler' => 'ignore',
    'trim_options' => array(
      'text' => 'text',
    ),
    'trim_preserve_tags' => '<em><strong>',
  );
  $handler->display->display_options['path'] = 'portfolio/1excerpt';
  $handler->display->display_options['menu']['title'] = 'Portfolio 1 Column with Excerpt';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 2 */
  $handler = $view->new_display('page', 'Portfolio Masonry 2', 'portfolio_v2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 2 Columns';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '6';
  $handler->display->display_options['style_options']['columns_lg'] = '6';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/v2';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 2';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 3 */
  $handler = $view->new_display('page', 'Portfolio Masonry 3', 'portfolio_v3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 3 Columns';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '4';
  $handler->display->display_options['style_options']['columns_lg'] = '4';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/v3';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 3';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 4 */
  $handler = $view->new_display('page', 'Portfolio Masonry 4', 'portfolio_v4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 4 Columns';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '3';
  $handler->display->display_options['style_options']['columns_lg'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/v4';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 4';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 2 with Excerpt */
  $handler = $view->new_display('page', 'Portfolio Masonry 2 with Excerpt', 'portfolio_v2excerpt');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 2 Columns with Excerpt';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'cms-portfolio-excerpt';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '0';
  $handler->display->display_options['style_options']['columns_lg'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_type'] = 'p';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'smart_trim_format';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_link' => '0',
    'trim_length' => '160',
    'trim_type' => 'chars',
    'trim_suffix' => '...',
    'more_link' => '0',
    'more_text' => 'Read more',
    'summary_handler' => 'ignore',
    'trim_options' => array(
      'text' => 'text',
    ),
    'trim_preserve_tags' => '<em><strong>',
  );
  $handler->display->display_options['path'] = 'portfolio/v2excerpt';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 2 with Excerpt';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 3 Excerpt */
  $handler = $view->new_display('page', 'Portfolio Masonry 3 Excerpt', 'portfolio_v3excerpt');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 3 Columns with Excerpt';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'cms-portfolio-excerpt';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '4';
  $handler->display->display_options['style_options']['columns_lg'] = '4';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_type'] = 'p';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'smart_trim_format';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_link' => '0',
    'trim_length' => '100',
    'trim_type' => 'chars',
    'trim_suffix' => '...',
    'more_link' => '0',
    'more_text' => 'Read more',
    'summary_handler' => 'ignore',
    'trim_options' => array(
      'text' => 'text',
    ),
    'trim_preserve_tags' => '<em><strong>',
  );
  $handler->display->display_options['path'] = 'portfolio/v3excerpt';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 3 with Excerpt';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 4 Excerpt */
  $handler = $view->new_display('page', 'Portfolio Masonry 4 Excerpt', 'portfolio_v4excerpt');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 4 Columns with Excerpt';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'cms-portfolio-excerpt';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '3';
  $handler->display->display_options['style_options']['columns_lg'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_type'] = 'p';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'smart_trim_format';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_link' => '0',
    'trim_length' => '100',
    'trim_type' => 'chars',
    'trim_suffix' => '...',
    'more_link' => '0',
    'more_text' => 'Read more',
    'summary_handler' => 'ignore',
    'trim_options' => array(
      'text' => 'text',
    ),
    'trim_preserve_tags' => '<em><strong>',
  );
  $handler->display->display_options['path'] = 'portfolio/v4excerpt';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 4 with Excerpt';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio 6 Columns */
  $handler = $view->new_display('page', 'Portfolio 6 Columns', 'portfolio_6');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio 6 Columns';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '24';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '2';
  $handler->display->display_options['style_options']['columns_lg'] = '2';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/6';
  $handler->display->display_options['menu']['title'] = 'Portfolio 6 Columns';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Masonry 6 */
  $handler = $view->new_display('page', 'Portfolio Masonry 6', 'portfolio_v6');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Portfolio Masonry 6 Columns';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '24';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '2';
  $handler->display->display_options['style_options']['columns_lg'] = '2';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_class'] = 'title cms-core-util-single-line text-center';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/v6';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 6';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Photography Masonry 4 */
  $handler = $view->new_display('page', 'Photography Masonry 4', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Photography Masonry 4 Columns';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['alignment'] = 'masonry';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '12';
  $handler->display->display_options['style_options']['columns_sm'] = '6';
  $handler->display->display_options['style_options']['columns_md'] = '3';
  $handler->display->display_options['style_options']['columns_lg'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6',
    'image_link' => 'content',
    'image_hover_effects' => 'fade_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<br>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'portfolio/photography4';
  $handler->display->display_options['menu']['title'] = 'Portfolio Masonry 4';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Portfolio Block 4 x 1 */
  $handler = $view->new_display('block', 'Portfolio Block 4 x 1', 'block_4x1');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'glazed-util-no-gutter';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '6';
  $handler->display->display_options['style_options']['columns_sm'] = '3';
  $handler->display->display_options['style_options']['columns_md'] = '3';
  $handler->display->display_options['style_options']['columns_lg'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6_square',
    'image_link' => 'content',
    'field_formatter_class' => '',
    'image_hover_effects' => 'zoom_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  $handler->display->display_options['block_description'] = 'Portfolio Block 4 x 1';

  /* Display: Portfolio Block 6 x 1 */
  $handler = $view->new_display('block', 'Portfolio Block 6 x 1', 'block_6x1');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'glazed-util-no-gutter';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '6';
  $handler->display->display_options['style_options']['columns_sm'] = '4';
  $handler->display->display_options['style_options']['columns_md'] = '2';
  $handler->display->display_options['style_options']['columns_lg'] = '2';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6_square',
    'image_link' => 'content',
    'field_formatter_class' => '',
    'image_hover_effects' => 'zoom_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  $handler->display->display_options['block_description'] = 'Portfolio Block 6 x 1';

  /* Display: Portfolio Block 4 x 3 */
  $handler = $view->new_display('block', 'Portfolio Block 4 x 3', 'block_4x3');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'glazed-util-no-gutter';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '6';
  $handler->display->display_options['style_options']['columns_sm'] = '3';
  $handler->display->display_options['style_options']['columns_md'] = '3';
  $handler->display->display_options['style_options']['columns_lg'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6_square',
    'image_link' => 'content',
    'field_formatter_class' => '',
    'image_hover_effects' => 'zoom_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  $handler->display->display_options['block_description'] = 'Portfolio Block 4 x 3';

  /* Display: Portfolio Block 6 x 3 */
  $handler = $view->new_display('block', 'Portfolio Block 6 x 3', 'block_6x3');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'glazed-util-no-gutter';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
  $handler->display->display_options['style_options']['columns'] = '0';
  $handler->display->display_options['style_options']['columns_xs'] = '6';
  $handler->display->display_options['style_options']['columns_sm'] = '4';
  $handler->display->display_options['style_options']['columns_md'] = '2';
  $handler->display->display_options['style_options']['columns_lg'] = '2';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Portfolio images */
  $handler->display->display_options['fields']['field_portfolio_images']['id'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['table'] = 'field_data_field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['field'] = 'field_portfolio_images';
  $handler->display->display_options['fields']['field_portfolio_images']['label'] = '';
  $handler->display->display_options['fields']['field_portfolio_images']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_portfolio_images']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_portfolio_images']['settings'] = array(
    'image_style' => 'bootstrap3_col6_square',
    'image_link' => 'content',
    'field_formatter_class' => '',
    'image_hover_effects' => 'zoom_in',
  );
  $handler->display->display_options['fields']['field_portfolio_images']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_portfolio_images']['delta_offset'] = '0';
  $handler->display->display_options['block_description'] = 'Portfolio Block 6 x 3';
  $export['cms_portfolio'] = $view;

  return $export;
}
