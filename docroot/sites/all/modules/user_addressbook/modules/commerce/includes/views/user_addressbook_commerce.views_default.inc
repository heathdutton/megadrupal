<?php

/**
 * Views for the default customer UI.
 */

/**
 * Implements hook_views_default_views().
 */
function user_addressbook_commerce_views_default_views() {
  $views = array();

  // User addressbook for checkout.
  $view = new view();
  $view->name = 'user_addressbook_checkout';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'user_address';
  $view->human_name = 'User addressbook checkout';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Addressbook';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own user_address';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: User addresses: Address owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'user_address';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: User addresses: Rendered User address */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_user_address';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'user_addressbook_checkout';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: User addresses: Set default address */
  $handler->display->display_options['fields']['user_address_set_default']['id'] = 'user_address_set_default';
  $handler->display->display_options['fields']['user_address_set_default']['table'] = 'user_address';
  $handler->display->display_options['fields']['user_address_set_default']['field'] = 'user_address_set_default';
  $handler->display->display_options['fields']['user_address_set_default']['label'] = '';
  $handler->display->display_options['fields']['user_address_set_default']['element_label_colon'] = FALSE;
  /* Field: User addresses: Checkout select user address button */
  $handler->display->display_options['fields']['user_address_commerce_checkout_select_user_address']['id'] = 'user_address_commerce_checkout_select_user_address';
  $handler->display->display_options['fields']['user_address_commerce_checkout_select_user_address']['table'] = 'user_address';
  $handler->display->display_options['fields']['user_address_commerce_checkout_select_user_address']['field'] = 'user_address_commerce_checkout_select_user_address';
  $handler->display->display_options['fields']['user_address_commerce_checkout_select_user_address']['label'] = '';
  $handler->display->display_options['fields']['user_address_commerce_checkout_select_user_address']['element_label_colon'] = FALSE;
  /* Contextual filter: User addresses: Owner uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'user_address';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  $views[$view->name] = $view;

  return $views;
}
