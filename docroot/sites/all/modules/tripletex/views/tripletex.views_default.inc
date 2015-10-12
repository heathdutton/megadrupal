<?php

/**
 * @file
 * Default views definition. Aviailable in the views listing
 *
 */

/**
 * Returns the default view for the Tripletex module
 *
 */
function tripletex_views_default_views() {

$view = new view();
$view->name = 'tripletex_invoice_list';
$view->description = 'List of invoices and their status';
$view->tag = '';
$view->base_table = 'tripletex_invoice';
$view->human_name = '';
$view->core = 6;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Invoices';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['use_more_text'] = 'mer';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'access own tripletex content';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['submit_button'] = 'Bruk';
$handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Nullstill';
$handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortér på';
$handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Stigende';
$handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Synkende';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = 20;
$handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementer per side';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Alle -';
$handler->display->display_options['pager']['options']['tags']['first'] = '« første';
$handler->display->display_options['pager']['options']['tags']['previous'] = '‹ forrige';
$handler->display->display_options['pager']['options']['tags']['next'] = 'neste ›';
$handler->display->display_options['pager']['options']['tags']['last'] = 'siste »';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = '';
$handler->display->display_options['style_options']['columns'] = array(
  'id' => 'id',
  'nid' => 'nid',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'id' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'nid' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
/* Relasjon: Tripletex: User Id */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'tripletex_invoice';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relasjon: Tripletex: Node Id */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'tripletex_invoice';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
/* Felt: Tripletex: Invoice Id */
$handler->display->display_options['fields']['id']['id'] = 'id';
$handler->display->display_options['fields']['id']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['id']['field'] = 'id';
$handler->display->display_options['fields']['id']['label'] = 'Id';
$handler->display->display_options['fields']['id']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['id']['alter']['path'] = 'invoice/display/[id]';
/* Felt: Bruker: Navn */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
/* Felt: Tripletex: Navn */
$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
$handler->display->display_options['fields']['name_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['name_1']['field'] = 'name';
$handler->display->display_options['fields']['name_1']['label'] = 'Name';
/* Felt: Tripletex: E-post */
$handler->display->display_options['fields']['mail_1']['id'] = 'mail_1';
$handler->display->display_options['fields']['mail_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['mail_1']['field'] = 'mail';
$handler->display->display_options['fields']['mail_1']['label'] = 'E-mail';
/* Felt: Tripletex: Valutta */
$handler->display->display_options['fields']['currency_1']['id'] = 'currency_1';
$handler->display->display_options['fields']['currency_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['currency_1']['field'] = 'currency';
$handler->display->display_options['fields']['currency_1']['label'] = 'Currency';
/* Felt: Tripletex: Remaining */
$handler->display->display_options['fields']['remaining_amount_1']['id'] = 'remaining_amount_1';
$handler->display->display_options['fields']['remaining_amount_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['remaining_amount_1']['field'] = 'remaining_amount';
/* Felt: Tripletex: Dato */
$handler->display->display_options['fields']['invoice_date_1']['id'] = 'invoice_date_1';
$handler->display->display_options['fields']['invoice_date_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['invoice_date_1']['field'] = 'invoice_date';
$handler->display->display_options['fields']['invoice_date_1']['label'] = 'Date';
$handler->display->display_options['fields']['invoice_date_1']['hide_alter_empty'] = FALSE;
$handler->display->display_options['fields']['invoice_date_1']['date_format'] = 'custom';
$handler->display->display_options['fields']['invoice_date_1']['custom_date_format'] = 'd/m/Y';
/* Felt: Tripletex: Due */
$handler->display->display_options['fields']['due_date_1']['id'] = 'due_date_1';
$handler->display->display_options['fields']['due_date_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['due_date_1']['field'] = 'due_date';
$handler->display->display_options['fields']['due_date_1']['date_format'] = 'custom';
$handler->display->display_options['fields']['due_date_1']['custom_date_format'] = 'd/m/Y';
/* Felt: Tripletex: Settled */
$handler->display->display_options['fields']['settled_date_1']['id'] = 'settled_date_1';
$handler->display->display_options['fields']['settled_date_1']['table'] = 'tripletex_invoice';
$handler->display->display_options['fields']['settled_date_1']['field'] = 'settled_date';

/* Display: Invoice listing */
$handler = $view->new_display('page', 'Invoice listing', 'page_1');
$handler->display->display_options['path'] = 'admin/content/invoice';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Invoices';
$handler->display->display_options['menu']['description'] = 'Invoices attached to this user';
$handler->display->display_options['menu']['weight'] = '1';

/* Display: User invoices */
$handler = $view->new_display('page', 'User invoices', 'page_2');
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Tripletex: User Id */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'tripletex_invoice';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['exception']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
$handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
  2 => '2',
);
$handler->display->display_options['arguments']['uid']['validate']['fail'] = 'empty';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
$handler->display->display_options['path'] = 'user/%/invoices';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Invoices';
$handler->display->display_options['menu']['description'] = 'Invoices assigned to this user';
$handler->display->display_options['menu']['weight'] = '5';

/* Display: Due invoices */
$handler = $view->new_display('page', 'Due invoices', 'page_3');
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Tripletex: Due */
$handler->display->display_options['filters']['due_date']['id'] = 'due_date';
$handler->display->display_options['filters']['due_date']['table'] = 'tripletex_invoice';
$handler->display->display_options['filters']['due_date']['field'] = 'due_date';
$handler->display->display_options['filters']['due_date']['operator'] = '<';
$handler->display->display_options['filters']['due_date']['value']['value'] = '+0 day';
$handler->display->display_options['filters']['due_date']['value']['type'] = 'offset';
$handler->display->display_options['filters']['due_date']['group'] = '0';
$handler->display->display_options['filters']['due_date']['expose']['operator'] = FALSE;
/* Filter criterion: Tripletex: Remaining */
$handler->display->display_options['filters']['remaining_amount']['id'] = 'remaining_amount';
$handler->display->display_options['filters']['remaining_amount']['table'] = 'tripletex_invoice';
$handler->display->display_options['filters']['remaining_amount']['field'] = 'remaining_amount';
$handler->display->display_options['filters']['remaining_amount']['operator'] = '>';
$handler->display->display_options['filters']['remaining_amount']['value']['value'] = '0';
$handler->display->display_options['filters']['remaining_amount']['group'] = '0';
$handler->display->display_options['filters']['remaining_amount']['expose']['operator'] = FALSE;
$handler->display->display_options['path'] = 'admin/content/invoice/due';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Due invoices';
$handler->display->display_options['menu']['description'] = 'List of invoices where due date is passed and amount is not zero';
$handler->display->display_options['menu']['weight'] = '10';
$translatables['tripletex_invoice_list'] = array(
  t('Defaults'),
  t('Invoices'),
  t('mer'),
  t('Bruk'),
  t('Nullstill'),
  t('Sortér på'),
  t('Stigende'),
  t('Synkende'),
  t('Elementer per side'),
  t('- Alle -'),
  t('Offset'),
  t('« første'),
  t('‹ forrige'),
  t('neste ›'),
  t('siste »'),
  t('Invoice user'),
  t('Invoice node'),
  t('Id'),
  t('Navn'),
  t('Name'),
  t('E-mail'),
  t('Currency'),
  t('Remaining'),
  t(','),
  t(' '),
  t('kr. '),
  t('Date'),
  t('Due'),
  t('Settled'),
  t('Invoice listing'),
  t('User invoices'),
  t('All'),
  t('Due invoices'),
);

  $views[$view->name] = $view;

  return $views;

}
