<?php

/**
 * @file
 * Real Estate Property views.
 */

/**
 * Implements hook_views_default_views().
 */
function real_estate_property_views_default_views() {

  // Show list of property.
  $view = new view();
  $view->name = 'real_estate_property_list';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Real Estate Property List';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Real Estate Property List';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    1 => '1',
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'property_photo' => 'property_photo',
    'property_address' => 'property_address',
    'property_availability' => 'property_price',
    'property_purpose' => 'property_price',
    'property_price' => 'property_price',
    'property_bedrooms' => 'property_bedrooms',
    'property_bathrooms' => 'property_bathrooms',
    'property_yearbuilt' => 'property_yearbuilt',
    'created' => 'property_address',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_photo' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_address' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_availability' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_purpose' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_price' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_bedrooms' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_bathrooms' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'property_yearbuilt' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There is no results for that query. Please check your spelling or enter a valid ZIP code.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Photo */
  $handler->display->display_options['fields']['property_photo']['id'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['table'] = 'field_data_property_photo';
  $handler->display->display_options['fields']['property_photo']['field'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['label'] = '';
  $handler->display->display_options['fields']['property_photo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['property_photo']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['property_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['property_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['property_photo']['settings'] = array(
    'image_style' => 'real_estate_property_thumbnail',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['property_photo']['delta_limit'] = '1';
  $handler->display->display_options['fields']['property_photo']['delta_offset'] = '0';
  /* Field: Content: Address */
  $handler->display->display_options['fields']['property_address']['id'] = 'property_address';
  $handler->display->display_options['fields']['property_address']['table'] = 'field_data_property_address';
  $handler->display->display_options['fields']['property_address']['field'] = 'property_address';
  $handler->display->display_options['fields']['property_address']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['property_address']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['property_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['property_address']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Content: Availability */
  $handler->display->display_options['fields']['property_availability']['id'] = 'property_availability';
  $handler->display->display_options['fields']['property_availability']['table'] = 'field_data_property_availability';
  $handler->display->display_options['fields']['property_availability']['field'] = 'property_availability';
  $handler->display->display_options['fields']['property_availability']['label'] = '';
  $handler->display->display_options['fields']['property_availability']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_availability']['element_label_colon'] = FALSE;
  /* Field: Content: Purpose */
  $handler->display->display_options['fields']['property_purpose']['id'] = 'property_purpose';
  $handler->display->display_options['fields']['property_purpose']['table'] = 'field_data_property_purpose';
  $handler->display->display_options['fields']['property_purpose']['field'] = 'property_purpose';
  $handler->display->display_options['fields']['property_purpose']['label'] = '';
  $handler->display->display_options['fields']['property_purpose']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_purpose']['element_label_colon'] = FALSE;
  /* Field: Content: Price */
  $handler->display->display_options['fields']['property_price']['id'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['table'] = 'field_data_property_price';
  $handler->display->display_options['fields']['property_price']['field'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Bedrooms */
  $handler->display->display_options['fields']['property_bedrooms']['id'] = 'property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['table'] = 'field_data_property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['field'] = 'property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['label'] = 'Beds';
  /* Field: Content: Bathrooms */
  $handler->display->display_options['fields']['property_bathrooms']['id'] = 'property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['table'] = 'field_data_property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['field'] = 'property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['label'] = 'Baths';
  /* Field: Content: Year Built */
  $handler->display->display_options['fields']['property_yearbuilt']['id'] = 'property_yearbuilt';
  $handler->display->display_options['fields']['property_yearbuilt']['table'] = 'field_data_property_yearbuilt';
  $handler->display->display_options['fields']['property_yearbuilt']['field'] = 'property_yearbuilt';
  $handler->display->display_options['fields']['property_yearbuilt']['label'] = 'Built';
  $handler->display->display_options['fields']['property_yearbuilt']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'added';
  $handler->display->display_options['fields']['created']['element_type'] = 'div';
  $handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Address - Postal code */
  $handler->display->display_options['filters']['property_address_postal_code']['id'] = 'property_address_postal_code';
  $handler->display->display_options['filters']['property_address_postal_code']['table'] = 'field_data_property_address';
  $handler->display->display_options['filters']['property_address_postal_code']['field'] = 'property_address_postal_code';
  $handler->display->display_options['filters']['property_address_postal_code']['group'] = 1;
  $handler->display->display_options['filters']['property_address_postal_code']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_address_postal_code']['expose']['operator_id'] = 'property_address_postal_code_op';
  $handler->display->display_options['filters']['property_address_postal_code']['expose']['label'] = 'Postal code';
  $handler->display->display_options['filters']['property_address_postal_code']['expose']['operator'] = 'property_address_postal_code_op';
  $handler->display->display_options['filters']['property_address_postal_code']['expose']['identifier'] = 'property_address_postal_code';
  $handler->display->display_options['filters']['property_address_postal_code']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Address - Country */
  $handler->display->display_options['filters']['property_address_country']['id'] = 'property_address_country';
  $handler->display->display_options['filters']['property_address_country']['table'] = 'field_data_property_address';
  $handler->display->display_options['filters']['property_address_country']['field'] = 'property_address_country';
  $handler->display->display_options['filters']['property_address_country']['group'] = 1;
  $handler->display->display_options['filters']['property_address_country']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_address_country']['expose']['operator_id'] = 'property_address_country_op';
  $handler->display->display_options['filters']['property_address_country']['expose']['label'] = 'Country';
  $handler->display->display_options['filters']['property_address_country']['expose']['operator'] = 'property_address_country_op';
  $handler->display->display_options['filters']['property_address_country']['expose']['identifier'] = 'property_address_country';
  $handler->display->display_options['filters']['property_address_country']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Address - Administrative area (i.e. State / Province) */
  $handler->display->display_options['filters']['property_address_administrative_area']['id'] = 'property_address_administrative_area';
  $handler->display->display_options['filters']['property_address_administrative_area']['table'] = 'field_data_property_address';
  $handler->display->display_options['filters']['property_address_administrative_area']['field'] = 'property_address_administrative_area';
  $handler->display->display_options['filters']['property_address_administrative_area']['operator'] = 'contains';
  $handler->display->display_options['filters']['property_address_administrative_area']['group'] = 1;
  $handler->display->display_options['filters']['property_address_administrative_area']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_address_administrative_area']['expose']['operator_id'] = 'property_address_administrative_area_op';
  $handler->display->display_options['filters']['property_address_administrative_area']['expose']['label'] = 'Administrative area';
  $handler->display->display_options['filters']['property_address_administrative_area']['expose']['operator'] = 'property_address_administrative_area_op';
  $handler->display->display_options['filters']['property_address_administrative_area']['expose']['identifier'] = 'property_address_administrative_area';
  $handler->display->display_options['filters']['property_address_administrative_area']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Price (property_price) */
  $handler->display->display_options['filters']['property_price_value']['id'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['table'] = 'field_data_property_price';
  $handler->display->display_options['filters']['property_price_value']['field'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['operator'] = 'between';
  $handler->display->display_options['filters']['property_price_value']['group'] = 1;
  $handler->display->display_options['filters']['property_price_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_price_value']['expose']['operator_id'] = 'property_price_value_op';
  $handler->display->display_options['filters']['property_price_value']['expose']['label'] = 'Price';
  $handler->display->display_options['filters']['property_price_value']['expose']['operator'] = 'property_price_value_op';
  $handler->display->display_options['filters']['property_price_value']['expose']['identifier'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['property_price_value']['group_info']['label'] = 'Price (property_price)';
  $handler->display->display_options['filters']['property_price_value']['group_info']['identifier'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['group_info']['group_items'] = array(
    1 => array(
      'title' => '',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'property' => 'property',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Page - Property List */
  $handler = $view->new_display('page', 'Page - Property List', 'property_list');
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There is no results for that query. Please check your spelling or try enter other values.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Photo */
  $handler->display->display_options['fields']['property_photo']['id'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['table'] = 'field_data_property_photo';
  $handler->display->display_options['fields']['property_photo']['field'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['label'] = '';
  $handler->display->display_options['fields']['property_photo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['property_photo']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['property_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['property_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['property_photo']['settings'] = array(
    'image_style' => 'real_estate_property_thumbnail',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['property_photo']['delta_limit'] = '1';
  $handler->display->display_options['fields']['property_photo']['delta_offset'] = '0';
  /* Field: Content: Address */
  $handler->display->display_options['fields']['property_address']['id'] = 'property_address';
  $handler->display->display_options['fields']['property_address']['table'] = 'field_data_property_address';
  $handler->display->display_options['fields']['property_address']['field'] = 'property_address';
  $handler->display->display_options['fields']['property_address']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['property_address']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['property_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['property_address']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Content: Availability */
  $handler->display->display_options['fields']['property_availability']['id'] = 'property_availability';
  $handler->display->display_options['fields']['property_availability']['table'] = 'field_data_property_availability';
  $handler->display->display_options['fields']['property_availability']['field'] = 'property_availability';
  $handler->display->display_options['fields']['property_availability']['label'] = '';
  $handler->display->display_options['fields']['property_availability']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_availability']['element_label_colon'] = FALSE;
  /* Field: Content: Purpose */
  $handler->display->display_options['fields']['property_purpose']['id'] = 'property_purpose';
  $handler->display->display_options['fields']['property_purpose']['table'] = 'field_data_property_purpose';
  $handler->display->display_options['fields']['property_purpose']['field'] = 'property_purpose';
  $handler->display->display_options['fields']['property_purpose']['label'] = '';
  $handler->display->display_options['fields']['property_purpose']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_purpose']['element_label_colon'] = FALSE;
  /* Field: Content: Price */
  $handler->display->display_options['fields']['property_price']['id'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['table'] = 'field_data_property_price';
  $handler->display->display_options['fields']['property_price']['field'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_price']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Bedrooms */
  $handler->display->display_options['fields']['property_bedrooms']['id'] = 'property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['table'] = 'field_data_property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['field'] = 'property_bedrooms';
  $handler->display->display_options['fields']['property_bedrooms']['label'] = 'Beds';
  /* Field: Content: Bathrooms */
  $handler->display->display_options['fields']['property_bathrooms']['id'] = 'property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['table'] = 'field_data_property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['field'] = 'property_bathrooms';
  $handler->display->display_options['fields']['property_bathrooms']['label'] = 'Baths';
  /* Field: Content: Year Built */
  $handler->display->display_options['fields']['property_yearbuilt']['id'] = 'property_yearbuilt';
  $handler->display->display_options['fields']['property_yearbuilt']['table'] = 'field_data_property_yearbuilt';
  $handler->display->display_options['fields']['property_yearbuilt']['field'] = 'property_yearbuilt';
  $handler->display->display_options['fields']['property_yearbuilt']['label'] = 'Built';
  $handler->display->display_options['fields']['property_yearbuilt']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'added';
  $handler->display->display_options['fields']['created']['element_type'] = 'div';
  $handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Purpose (property_purpose) */
  $handler->display->display_options['filters']['property_purpose_value']['id'] = 'property_purpose_value';
  $handler->display->display_options['filters']['property_purpose_value']['table'] = 'field_data_property_purpose';
  $handler->display->display_options['filters']['property_purpose_value']['field'] = 'property_purpose_value';
  $handler->display->display_options['filters']['property_purpose_value']['group'] = 1;
  $handler->display->display_options['filters']['property_purpose_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_purpose_value']['expose']['operator_id'] = 'property_purpose_value_op';
  $handler->display->display_options['filters']['property_purpose_value']['expose']['label'] = 'Purpose';
  $handler->display->display_options['filters']['property_purpose_value']['expose']['operator'] = 'property_purpose_value_op';
  $handler->display->display_options['filters']['property_purpose_value']['expose']['identifier'] = 'property_purpose_value';
  $handler->display->display_options['filters']['property_purpose_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => '1',
    8 => '8',
    3 => '3',
  );
  /* Filter criterion: Content: Type (property_type) */
  $handler->display->display_options['filters']['property_type_value']['id'] = 'property_type_value';
  $handler->display->display_options['filters']['property_type_value']['table'] = 'field_data_property_type';
  $handler->display->display_options['filters']['property_type_value']['field'] = 'property_type_value';
  $handler->display->display_options['filters']['property_type_value']['group'] = 1;
  $handler->display->display_options['filters']['property_type_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_type_value']['expose']['operator_id'] = 'property_type_value_op';
  $handler->display->display_options['filters']['property_type_value']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['property_type_value']['expose']['operator'] = 'property_type_value_op';
  $handler->display->display_options['filters']['property_type_value']['expose']['identifier'] = 'property_type_value';
  $handler->display->display_options['filters']['property_type_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => '1',
    8 => '8',
    3 => '3',
  );
  /* Filter criterion: Content: Address - Locality (i.e. City) */
  $handler->display->display_options['filters']['property_address_locality']['id'] = 'property_address_locality';
  $handler->display->display_options['filters']['property_address_locality']['table'] = 'field_data_property_address';
  $handler->display->display_options['filters']['property_address_locality']['field'] = 'property_address_locality';
  $handler->display->display_options['filters']['property_address_locality']['operator'] = 'contains';
  $handler->display->display_options['filters']['property_address_locality']['group'] = 1;
  $handler->display->display_options['filters']['property_address_locality']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_address_locality']['expose']['operator_id'] = 'property_address_locality_op';
  $handler->display->display_options['filters']['property_address_locality']['expose']['label'] = 'City';
  $handler->display->display_options['filters']['property_address_locality']['expose']['operator'] = 'property_address_locality_op';
  $handler->display->display_options['filters']['property_address_locality']['expose']['identifier'] = 'property_address_locality';
  $handler->display->display_options['filters']['property_address_locality']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Address - Thoroughfare (i.e. Street address) */
  $handler->display->display_options['filters']['property_address_thoroughfare']['id'] = 'property_address_thoroughfare';
  $handler->display->display_options['filters']['property_address_thoroughfare']['table'] = 'field_data_property_address';
  $handler->display->display_options['filters']['property_address_thoroughfare']['field'] = 'property_address_thoroughfare';
  $handler->display->display_options['filters']['property_address_thoroughfare']['operator'] = 'contains';
  $handler->display->display_options['filters']['property_address_thoroughfare']['group'] = 1;
  $handler->display->display_options['filters']['property_address_thoroughfare']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_address_thoroughfare']['expose']['operator_id'] = 'property_address_thoroughfare_op';
  $handler->display->display_options['filters']['property_address_thoroughfare']['expose']['label'] = 'Street address';
  $handler->display->display_options['filters']['property_address_thoroughfare']['expose']['operator'] = 'property_address_thoroughfare_op';
  $handler->display->display_options['filters']['property_address_thoroughfare']['expose']['identifier'] = 'property_address_thoroughfare';
  $handler->display->display_options['filters']['property_address_thoroughfare']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['property_address_thoroughfare']['group_info']['label'] = 'Address - Thoroughfare (i.e. Street address)';
  $handler->display->display_options['filters']['property_address_thoroughfare']['group_info']['identifier'] = 'property_address_thoroughfare';
  $handler->display->display_options['filters']['property_address_thoroughfare']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['property_address_thoroughfare']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: Content: Price (property_price) */
  $handler->display->display_options['filters']['property_price_value']['id'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['table'] = 'field_data_property_price';
  $handler->display->display_options['filters']['property_price_value']['field'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['operator'] = 'between';
  $handler->display->display_options['filters']['property_price_value']['group'] = 1;
  $handler->display->display_options['filters']['property_price_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['property_price_value']['expose']['operator_id'] = 'property_price_value_op';
  $handler->display->display_options['filters']['property_price_value']['expose']['label'] = 'Price';
  $handler->display->display_options['filters']['property_price_value']['expose']['operator'] = 'property_price_value_op';
  $handler->display->display_options['filters']['property_price_value']['expose']['identifier'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => '1',
    8 => '8',
    3 => '3',
  );
  $handler->display->display_options['filters']['property_price_value']['group_info']['label'] = 'Price (property_price)';
  $handler->display->display_options['filters']['property_price_value']['group_info']['identifier'] = 'property_price_value';
  $handler->display->display_options['filters']['property_price_value']['group_info']['group_items'] = array(
    1 => array(
      'title' => '',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'property' => 'property',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['path'] = 'real-estate-property-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Real Estate';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Block - Last Added */
  $handler = $view->new_display('block', 'Block - Last Added', 'block_last_added');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Photo */
  $handler->display->display_options['fields']['property_photo']['id'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['table'] = 'field_data_property_photo';
  $handler->display->display_options['fields']['property_photo']['field'] = 'property_photo';
  $handler->display->display_options['fields']['property_photo']['label'] = '';
  $handler->display->display_options['fields']['property_photo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['property_photo']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['property_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['property_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['property_photo']['settings'] = array(
    'image_style' => 'real_estate_property_thumbnail',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['property_photo']['delta_limit'] = '1';
  $handler->display->display_options['fields']['property_photo']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Availability */
  $handler->display->display_options['fields']['property_availability']['id'] = 'property_availability';
  $handler->display->display_options['fields']['property_availability']['table'] = 'field_data_property_availability';
  $handler->display->display_options['fields']['property_availability']['field'] = 'property_availability';
  $handler->display->display_options['fields']['property_availability']['label'] = '';
  $handler->display->display_options['fields']['property_availability']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_availability']['element_label_colon'] = FALSE;
  /* Field: Content: Purpose */
  $handler->display->display_options['fields']['property_purpose']['id'] = 'property_purpose';
  $handler->display->display_options['fields']['property_purpose']['table'] = 'field_data_property_purpose';
  $handler->display->display_options['fields']['property_purpose']['field'] = 'property_purpose';
  $handler->display->display_options['fields']['property_purpose']['label'] = '';
  $handler->display->display_options['fields']['property_purpose']['element_type'] = 'div';
  $handler->display->display_options['fields']['property_purpose']['element_label_colon'] = FALSE;
  /* Field: Content: Price */
  $handler->display->display_options['fields']['property_price']['id'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['table'] = 'field_data_property_price';
  $handler->display->display_options['fields']['property_price']['field'] = 'property_price';
  $handler->display->display_options['fields']['property_price']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'property' => 'property',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  $views[$view->name] = $view;

  return $views;

}
