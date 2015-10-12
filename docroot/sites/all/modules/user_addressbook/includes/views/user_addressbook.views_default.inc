<?php

/**
 * Views for the default customer UI.
 */

/**
 * Implements hook_views_default_views().
 */
function user_addressbook_views_default_views() {
  $views = array();

  // User addressbook.
  $view = new view();
  $view->name = 'user_addressbook';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'user_address';
  $view->human_name = 'User addressbook';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Addressbook';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own user_address';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '3';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Addresses number */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['ui_name'] = 'Addresses number';
  $handler->display->display_options['header']['result']['content'] = '@total addresses saved in your addressbook.';
  /* Relationship: User address: Address owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'user_address';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: User address: Rendered User address */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_user_address';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'user_addressbook';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: User address: Set default address */
  $handler->display->display_options['fields']['user_address_set_default']['id'] = 'user_address_set_default';
  $handler->display->display_options['fields']['user_address_set_default']['table'] = 'user_address';
  $handler->display->display_options['fields']['user_address_set_default']['field'] = 'user_address_set_default';
  /* Field: User address: Edit link */
  $handler->display->display_options['fields']['edit_user_address']['id'] = 'edit_user_address';
  $handler->display->display_options['fields']['edit_user_address']['table'] = 'user_address';
  $handler->display->display_options['fields']['edit_user_address']['field'] = 'edit_user_address';
  $handler->display->display_options['fields']['edit_user_address']['label'] = '';
  $handler->display->display_options['fields']['edit_user_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_user_address']['text'] = 'Edit address';
  /* Field: User address: Delete link */
  $handler->display->display_options['fields']['delete_user_address']['id'] = 'delete_user_address';
  $handler->display->display_options['fields']['delete_user_address']['table'] = 'user_address';
  $handler->display->display_options['fields']['delete_user_address']['field'] = 'delete_user_address';
  $handler->display->display_options['fields']['delete_user_address']['label'] = '';
  $handler->display->display_options['fields']['delete_user_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_user_address']['text'] = 'Remove';
  /* Contextual filter: User address: Owner uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'user_address';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Default addresses header */
  $handler = $view->new_display('attachment', 'Default addresses header', 'attachment_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Default addresses';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User Addressbook Defaults: User address default type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'user_address_defaults';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['element_type'] = 'h3';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: User address: Rendered User address */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_user_address';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'user_addressbook';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User Addressbook Defaults: User address default type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'user_address_defaults';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'not empty';
  $handler->display->display_options['filters']['type']['value'] = array(
    'all' => 'all',
  );
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
  );
  $views[$view->name] = $view;

  return $views;
}
