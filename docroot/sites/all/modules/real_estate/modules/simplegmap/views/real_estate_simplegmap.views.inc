<?php

/**
 * @file
 * Real Estate Property views.
 */

/**
 * Implements hook_views_default_views().
 */
function real_estate_simplegmap_views_default_views() {

  // Show main map of property.
  $view = new view();
  $view->name = 'real_estate_property_map';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Real Estate Property Map';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Real Estate Property Map';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Photo */
  $handler->display->display_options['fields']['property_photo']['id'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['table'] = 'field_data_property_photo';
  $handler->display->display_options['fields']['property_photo']['field'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['label'] = '';
  $handler->display->display_options['fields']['property_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['property_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['property_photo']['settings'] = array(
    'image_style' => 'real_estate_property_thumbnail',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['property_photo']['delta_limit'] = '1';
  $handler->display->display_options['fields']['property_photo']['delta_offset'] = '0';
  $handler->display->display_options['fields']['property_photo']['separator'] = '';
  /* Field: Content: Price */
  $handler->display->display_options['fields']['property_price']['id'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['table'] = 'field_data_property_price';
  $handler->display->display_options['fields']['property_price']['field'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['element_type'] = 'span';
  $handler->display->display_options['fields']['property_price']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '0',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Bathrooms */
  $handler->display->display_options['fields']['property_bathrooms']['id'] = 'property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['table'] = 'field_data_property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['field'] = 'property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['element_type'] = 'span';
  /* Field: Content: Bedrooms */
  $handler->display->display_options['fields']['property_bedrooms']['id'] = 'property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['table'] = 'field_data_property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['field'] = 'property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['element_type'] = 'span';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'details >>';
  /* Field: Content: Geo Location */
  $handler->display->display_options['fields']['property_geo_location']['id'] = 'property_geo_location';
  $handler->display->display_options['fields']['property_geo_location']['table'] = 'field_data_property_geo_location';
  $handler->display->display_options['fields']['property_geo_location']['field'] = 'property_geo_location';
  $handler->display->display_options['fields']['property_geo_location']['label'] = '';
  $handler->display->display_options['fields']['property_geo_location']['exclude'] = TRUE;
  $handler->display->display_options['fields']['property_geo_location']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['property_geo_location']['click_sort_column'] = 'lat';
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
    'property' => 'property',
  );
  /* Filter criterion: Content: Geo Location (property_geo_location:lat) */
  $handler->display->display_options['filters']['property_geo_location_lat']['id'] = 'property_geo_location_lat';
  $handler->display->display_options['filters']['property_geo_location_lat']['table'] = 'field_data_property_geo_location';
  $handler->display->display_options['filters']['property_geo_location_lat']['field'] = 'property_geo_location_lat';
  $handler->display->display_options['filters']['property_geo_location_lat']['operator'] = 'not empty';
  /* Filter criterion: Content: Geo Location (property_geo_location:lng) */
  $handler->display->display_options['filters']['property_geo_location_lng']['id'] = 'property_geo_location_lng';
  $handler->display->display_options['filters']['property_geo_location_lng']['table'] = 'field_data_property_geo_location';
  $handler->display->display_options['filters']['property_geo_location_lng']['field'] = 'property_geo_location_lng';
  $handler->display->display_options['filters']['property_geo_location_lng']['operator'] = 'not empty';

  /* Display: Block - Main map */
  $handler = $view->new_display('block', 'Block - Main map', 'block_real_estate_map_main');

  $views[$view->name] = $view;

  return $views;

}
