<?php
/**
 * @file
 * impress.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function impress_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'impress';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Impress';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'full';
  $handler->display->display_options['row_options']['links'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = '[edit]';
  /* Field: Content: data-rotate */
  $handler->display->display_options['fields']['field_impress_rotate']['id'] = 'field_impress_rotate';
  $handler->display->display_options['fields']['field_impress_rotate']['table'] = 'field_data_field_impress_rotate';
  $handler->display->display_options['fields']['field_impress_rotate']['field'] = 'field_impress_rotate';
  /* Field: Content: data-rotate-x */
  $handler->display->display_options['fields']['field_impress_rotate_x']['id'] = 'field_impress_rotate_x';
  $handler->display->display_options['fields']['field_impress_rotate_x']['table'] = 'field_data_field_impress_rotate_x';
  $handler->display->display_options['fields']['field_impress_rotate_x']['field'] = 'field_impress_rotate_x';
  /* Field: Content: data-rotate-y */
  $handler->display->display_options['fields']['field_impress_rotate_y']['id'] = 'field_impress_rotate_y';
  $handler->display->display_options['fields']['field_impress_rotate_y']['table'] = 'field_data_field_impress_rotate_y';
  $handler->display->display_options['fields']['field_impress_rotate_y']['field'] = 'field_impress_rotate_y';
  /* Field: Content: data-x */
  $handler->display->display_options['fields']['field_impress_x']['id'] = 'field_impress_x';
  $handler->display->display_options['fields']['field_impress_x']['table'] = 'field_data_field_impress_x';
  $handler->display->display_options['fields']['field_impress_x']['field'] = 'field_impress_x';
  /* Field: Content: data-y */
  $handler->display->display_options['fields']['field_impress_y']['id'] = 'field_impress_y';
  $handler->display->display_options['fields']['field_impress_y']['table'] = 'field_data_field_impress_y';
  $handler->display->display_options['fields']['field_impress_y']['field'] = 'field_impress_y';
  /* Field: Content: data-z */
  $handler->display->display_options['fields']['field_impress_z']['id'] = 'field_impress_z';
  $handler->display->display_options['fields']['field_impress_z']['table'] = 'field_data_field_impress_z';
  $handler->display->display_options['fields']['field_impress_z']['field'] = 'field_impress_z';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node_1']['id'] = 'edit_node_1';
  $handler->display->display_options['fields']['edit_node_1']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node_1']['field'] = 'edit_node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  /* Sort criterion: Weight: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'weight_weights';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
    'impress_slide' => 'impress_slide',
  );

  /* Display: Impress3D */
  $handler = $view->new_display('page', 'Impress3D', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'impress';

  /* Display: Slides Editor */
  $handler = $view->new_display('page', 'Slides Editor', 'page_2');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = FALSE;
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Unfiltered text */
  $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['content'] = '<a href=\'/node/add/impress-slide?destination=slides-editor\'>[Add new]</a>';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: data-scale */
  $handler->display->display_options['fields']['field_impress_scale']['id'] = 'field_impress_scale';
  $handler->display->display_options['fields']['field_impress_scale']['table'] = 'field_data_field_impress_scale';
  $handler->display->display_options['fields']['field_impress_scale']['field'] = 'field_impress_scale';
  $handler->display->display_options['fields']['field_impress_scale']['label'] = 'Scale';
  $handler->display->display_options['fields']['field_impress_scale']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_scale']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '2',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: data-x */
  $handler->display->display_options['fields']['field_impress_x']['id'] = 'field_impress_x';
  $handler->display->display_options['fields']['field_impress_x']['table'] = 'field_data_field_impress_x';
  $handler->display->display_options['fields']['field_impress_x']['field'] = 'field_impress_x';
  $handler->display->display_options['fields']['field_impress_x']['label'] = 'X Position';
  $handler->display->display_options['fields']['field_impress_x']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_impress_x']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_x']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '0',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: data-y */
  $handler->display->display_options['fields']['field_impress_y']['id'] = 'field_impress_y';
  $handler->display->display_options['fields']['field_impress_y']['table'] = 'field_data_field_impress_y';
  $handler->display->display_options['fields']['field_impress_y']['field'] = 'field_impress_y';
  $handler->display->display_options['fields']['field_impress_y']['label'] = 'Y Position';
  $handler->display->display_options['fields']['field_impress_y']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_impress_y']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_y']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '0',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: data-z */
  $handler->display->display_options['fields']['field_impress_z']['id'] = 'field_impress_z';
  $handler->display->display_options['fields']['field_impress_z']['table'] = 'field_data_field_impress_z';
  $handler->display->display_options['fields']['field_impress_z']['field'] = 'field_impress_z';
  $handler->display->display_options['fields']['field_impress_z']['label'] = 'Z Position';
  $handler->display->display_options['fields']['field_impress_z']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_z']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '0',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: data-rotate */
  $handler->display->display_options['fields']['field_impress_rotate']['id'] = 'field_impress_rotate';
  $handler->display->display_options['fields']['field_impress_rotate']['table'] = 'field_data_field_impress_rotate';
  $handler->display->display_options['fields']['field_impress_rotate']['field'] = 'field_impress_rotate';
  $handler->display->display_options['fields']['field_impress_rotate']['label'] = 'Rotate';
  $handler->display->display_options['fields']['field_impress_rotate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_impress_rotate']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_rotate']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '0',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: data-rotate-x */
  $handler->display->display_options['fields']['field_impress_rotate_x']['id'] = 'field_impress_rotate_x';
  $handler->display->display_options['fields']['field_impress_rotate_x']['table'] = 'field_data_field_impress_rotate_x';
  $handler->display->display_options['fields']['field_impress_rotate_x']['field'] = 'field_impress_rotate_x';
  $handler->display->display_options['fields']['field_impress_rotate_x']['label'] = 'X Rotate';
  $handler->display->display_options['fields']['field_impress_rotate_x']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_impress_rotate_x']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_rotate_x']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '0',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: data-rotate-y */
  $handler->display->display_options['fields']['field_impress_rotate_y']['id'] = 'field_impress_rotate_y';
  $handler->display->display_options['fields']['field_impress_rotate_y']['table'] = 'field_data_field_impress_rotate_y';
  $handler->display->display_options['fields']['field_impress_rotate_y']['field'] = 'field_impress_rotate_y';
  $handler->display->display_options['fields']['field_impress_rotate_y']['label'] = 'Y Rotate';
  $handler->display->display_options['fields']['field_impress_rotate_y']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_impress_rotate_y']['type'] = 'editable';
  $handler->display->display_options['fields']['field_impress_rotate_y']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '0',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = '[edit]';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['text'] = '[delete]';
  /* Field: Weight: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'weight_weights';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['path'] = 'slides-editor';

  /* Display: Slides */
  $handler = $view->new_display('page', 'Slides', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'slides';

  /* Display: Slides (paged) */
  $handler = $view->new_display('page', 'Slides (paged)', 'page_3');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'slides-paged';
  $export['impress'] = $view;

  return $export;
}
