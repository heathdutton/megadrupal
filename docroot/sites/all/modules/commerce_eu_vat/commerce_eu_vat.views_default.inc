<?php
/**
 * @file
 * Default views for the EU VAT Module
 */

/**
 * Impliments hook_views_default_views()
 */
function commerce_eu_vat_views_default_views() {

  $view = new view();
  $view->name = 'commerce_eu_vat_products';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_product';
  $view->human_name = 'Commerce EU VAT: Products';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Products EU VAT Rates';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_product entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Commerce Product: Node referencing products from field_product */
  $handler->display->display_options['relationships']['field_product']['id'] = 'field_product';
  $handler->display->display_options['relationships']['field_product']['table'] = 'commerce_product';
  $handler->display->display_options['relationships']['field_product']['field'] = 'field_product';
  $handler->display->display_options['relationships']['field_product']['label'] = 'Product Display';
  /* Field: Bulk operations: Commerce Product */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Edit VAT Rates',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          'bags_cases::commerce_eu_vat_rate' => 'bags_cases::commerce_eu_vat_rate',
          'drinks::commerce_eu_vat_rate' => 'drinks::commerce_eu_vat_rate',
          'hats::commerce_eu_vat_rate' => 'hats::commerce_eu_vat_rate',
          'shoes::commerce_eu_vat_rate' => 'shoes::commerce_eu_vat_rate',
          'storage_devices::commerce_eu_vat_rate' => 'storage_devices::commerce_eu_vat_rate',
          'tops::commerce_eu_vat_rate' => 'tops::commerce_eu_vat_rate',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['link_to_product'] = 0;
  /* Field: Commerce Product: EU VAT Rate */
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['id'] = 'commerce_eu_vat_rate';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['table'] = 'field_data_commerce_eu_vat_rate';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['field'] = 'commerce_eu_vat_rate';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['label'] = '';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['delta_offset'] = '0';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['multi_type'] = 'ul';
  $handler->display->display_options['fields']['commerce_eu_vat_rate']['field_api_classes'] = TRUE;
  /* Field: Commerce Product: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_product';
  $handler->display->display_options['fields']['title_1']['label'] = 'Display Title';
  /* Field: Content: Operations links (Dropbutton) */
  $handler->display->display_options['fields']['operations_dropbutton']['id'] = 'operations_dropbutton';
  $handler->display->display_options['fields']['operations_dropbutton']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['operations_dropbutton']['field'] = 'operations_dropbutton';
  $handler->display->display_options['fields']['operations_dropbutton']['relationship'] = 'field_product';
  $handler->display->display_options['fields']['operations_dropbutton']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations_dropbutton']['add_quick_edit'] = 0;
  $handler->display->display_options['fields']['operations_dropbutton']['add_destination'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/commerce/products/vat';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'VAT Rates';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;


  $views[$view->name] = $view;

  // Return the views.
  return $views;
}
