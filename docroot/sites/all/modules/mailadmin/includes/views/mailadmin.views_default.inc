<?php

/**
 * Views for the mailadmin module.
 */

/**
 * Implements hook_views_default_views().
 */
function mailadmin_views_default_views() {
  $views = array();

  // Mailbox table view.
  $view = new view;
  $view->name = 'mailadmin_mailbox_table';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'mailadmin_mailboxes';
  $view->human_name = 'Mailadmin mailboxes';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '1';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mailbox_id' => 'mailbox_id',
    'domain_name' => 'domain_name',
    'local_part' => 'local_part',
    'local_part_1' => 'local_part_1',
    'nothing' => 'local_part_1',
    'nothing_1' => 'nothing_1',
  );
  $handler->display->display_options['style_options']['default'] = 'local_part_1';
  $handler->display->display_options['style_options']['info'] = array(
    'mailbox_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'domain_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'local_part' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'local_part_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No mailboxes created';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: E-mail mailbox: E-mail mailbox ID */
  $handler->display->display_options['fields']['mailbox_id']['id'] = 'mailbox_id';
  $handler->display->display_options['fields']['mailbox_id']['table'] = 'mailadmin_mailboxes';
  $handler->display->display_options['fields']['mailbox_id']['field'] = 'mailbox_id';
  $handler->display->display_options['fields']['mailbox_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['mailbox_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['mailbox_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['mailbox_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['mailbox_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['mailbox_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['mailbox_id']['separator'] = '';
  $handler->display->display_options['fields']['mailbox_id']['format_plural'] = 0;
  /* Field: E-mail mailbox: Domain_name */
  $handler->display->display_options['fields']['domain_name']['id'] = 'domain_name';
  $handler->display->display_options['fields']['domain_name']['table'] = 'mailadmin_mailboxes';
  $handler->display->display_options['fields']['domain_name']['field'] = 'domain_name';
  $handler->display->display_options['fields']['domain_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['domain_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['domain_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['domain_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['domain_name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['domain_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['domain_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['domain_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['domain_name']['hide_alter_empty'] = 0;
  /* Field: E-mail mailbox: Local_part */
  $handler->display->display_options['fields']['local_part']['id'] = 'local_part';
  $handler->display->display_options['fields']['local_part']['table'] = 'mailadmin_mailboxes';
  $handler->display->display_options['fields']['local_part']['field'] = 'local_part';
  $handler->display->display_options['fields']['local_part']['exclude'] = TRUE;
  $handler->display->display_options['fields']['local_part']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['text'] = '<span>';
  $handler->display->display_options['fields']['local_part']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['external'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['max_length'] = '0';
  $handler->display->display_options['fields']['local_part']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['html'] = 0;
  $handler->display->display_options['fields']['local_part']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['local_part']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['local_part']['hide_empty'] = 0;
  $handler->display->display_options['fields']['local_part']['empty_zero'] = 0;
  $handler->display->display_options['fields']['local_part']['hide_alter_empty'] = 0;
  /* Field: E-mail mailbox: Local_part */
  $handler->display->display_options['fields']['local_part_1']['id'] = 'local_part_1';
  $handler->display->display_options['fields']['local_part_1']['table'] = 'mailadmin_mailboxes';
  $handler->display->display_options['fields']['local_part_1']['field'] = 'local_part';
  $handler->display->display_options['fields']['local_part_1']['label'] = 'Mailbox';
  $handler->display->display_options['fields']['local_part_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['local_part_1']['alter']['text'] = '<span></span>';
  $handler->display->display_options['fields']['local_part_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['local_part_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['local_part_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['local_part_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['local_part_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['local_part_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['local_part_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['local_part_1']['hide_alter_empty'] = 0;
  /* Field: Mailbox */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Mailbox';
  $handler->display->display_options['fields']['nothing']['label'] = 'Mailbox';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[local_part]@[domain_name]';
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
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Field: Operations */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Operations';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Operation';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'mailadmin/mailbox/[mailbox_id]';
  $handler->display->display_options['fields']['nothing_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = 0;
  /* Contextual filter: E-mail mailbox: Domain_name */
  $handler->display->display_options['arguments']['domain_name']['id'] = 'domain_name';
  $handler->display->display_options['arguments']['domain_name']['table'] = 'mailadmin_mailboxes';
  $handler->display->display_options['arguments']['domain_name']['field'] = 'domain_name';
  $handler->display->display_options['arguments']['domain_name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['domain_name']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['domain_name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['domain_name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['domain_name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['domain_name']['glossary'] = 0;
  $handler->display->display_options['arguments']['domain_name']['limit'] = '0';
  $handler->display->display_options['arguments']['domain_name']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['domain_name']['break_phrase'] = 0;

  $views[$view->name] = $view;

  // Mail forward table view.
  $view = new view;
  $view->name = 'mailadmin_forward_table';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'mailadmin_forwards';
  $view->human_name = 'Mailadmin Forward';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'forward_id' => 'forward_id',
    'local_part' => 'local_part',
    'domain_name' => 'domain_name',
    'local_part_1' => 'local_part_1',
    'destination' => 'destination',
    'nothing' => 'local_part_1',
    'nothing_1' => 'nothing_1',
  );
  $handler->display->display_options['style_options']['default'] = 'local_part_1';
  $handler->display->display_options['style_options']['info'] = array(
    'forward_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'local_part' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'domain_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'local_part_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'destination' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No forward-only addresses';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: E-mail forward: E-mail forward ID */
  $handler->display->display_options['fields']['forward_id']['id'] = 'forward_id';
  $handler->display->display_options['fields']['forward_id']['table'] = 'mailadmin_forwards';
  $handler->display->display_options['fields']['forward_id']['field'] = 'forward_id';
  $handler->display->display_options['fields']['forward_id']['label'] = '';
  $handler->display->display_options['fields']['forward_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['forward_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['forward_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['forward_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['forward_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['forward_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['forward_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['forward_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['forward_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['forward_id']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['forward_id']['separator'] = '';
  $handler->display->display_options['fields']['forward_id']['format_plural'] = 0;
  /* Field: E-mail forward: Local_part */
  $handler->display->display_options['fields']['local_part']['id'] = 'local_part';
  $handler->display->display_options['fields']['local_part']['table'] = 'mailadmin_forwards';
  $handler->display->display_options['fields']['local_part']['field'] = 'local_part';
  $handler->display->display_options['fields']['local_part']['exclude'] = TRUE;
  $handler->display->display_options['fields']['local_part']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['external'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['local_part']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['local_part']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['local_part']['alter']['html'] = 0;
  $handler->display->display_options['fields']['local_part']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['local_part']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['local_part']['hide_empty'] = 0;
  $handler->display->display_options['fields']['local_part']['empty_zero'] = 0;
  $handler->display->display_options['fields']['local_part']['hide_alter_empty'] = 0;
  /* Field: E-mail forward: Domain_name */
  $handler->display->display_options['fields']['domain_name']['id'] = 'domain_name';
  $handler->display->display_options['fields']['domain_name']['table'] = 'mailadmin_forwards';
  $handler->display->display_options['fields']['domain_name']['field'] = 'domain_name';
  $handler->display->display_options['fields']['domain_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['domain_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['domain_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['domain_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['domain_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['domain_name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['domain_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['domain_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['domain_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['domain_name']['hide_alter_empty'] = 0;
  /* Field: E-mail forward: Local_part */
  $handler->display->display_options['fields']['local_part_1']['id'] = 'local_part_1';
  $handler->display->display_options['fields']['local_part_1']['table'] = 'mailadmin_forwards';
  $handler->display->display_options['fields']['local_part_1']['field'] = 'local_part';
  $handler->display->display_options['fields']['local_part_1']['label'] = 'Forward';
  $handler->display->display_options['fields']['local_part_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['local_part_1']['alter']['text'] = '<span></span>';
  $handler->display->display_options['fields']['local_part_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['local_part_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['local_part_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['local_part_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['local_part_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['local_part_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['local_part_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['local_part_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['local_part_1']['hide_alter_empty'] = 0;
  /* Field: E-mail forward: Destination */
  $handler->display->display_options['fields']['destination']['id'] = 'destination';
  $handler->display->display_options['fields']['destination']['table'] = 'mailadmin_forwards';
  $handler->display->display_options['fields']['destination']['field'] = 'destination';
  $handler->display->display_options['fields']['destination']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['external'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['destination']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['destination']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['destination']['alter']['html'] = 0;
  $handler->display->display_options['fields']['destination']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['destination']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['destination']['hide_empty'] = 0;
  $handler->display->display_options['fields']['destination']['empty_zero'] = 0;
  $handler->display->display_options['fields']['destination']['hide_alter_empty'] = 0;
  /* Field: Forward */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Forward';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[local_part]@[domain_name]';
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
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Field: Edit link */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Edit link';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Operation';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'mailadmin/forward/[forward_id]';
  $handler->display->display_options['fields']['nothing_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = 0;
  /* Contextual filter: E-mail forward: Domain_name */
  $handler->display->display_options['arguments']['domain_name']['id'] = 'domain_name';
  $handler->display->display_options['arguments']['domain_name']['table'] = 'mailadmin_forwards';
  $handler->display->display_options['arguments']['domain_name']['field'] = 'domain_name';
  $handler->display->display_options['arguments']['domain_name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['domain_name']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['domain_name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['domain_name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['domain_name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['domain_name']['glossary'] = 0;
  $handler->display->display_options['arguments']['domain_name']['limit'] = '0';
  $handler->display->display_options['arguments']['domain_name']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['domain_name']['break_phrase'] = 0;

  $views[$view->name] = $view;

  // Domain alias table view.
  $view = new view;
  $view->name = 'mailadmin_alias_table';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'mailadmin_domain_aliases';
  $view->human_name = 'Mailadmin Alias';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'alias_id' => 'alias_id',
    'source_domain_name' => 'source_domain_name',
    'target_domain_name' => 'target_domain_name',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'source_domain_name';
  $handler->display->display_options['style_options']['info'] = array(
    'alias_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'source_domain_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'target_domain_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No domain aliases have been created yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: E-mail domain alias: E-mail domain alias ID */
  $handler->display->display_options['fields']['alias_id']['id'] = 'alias_id';
  $handler->display->display_options['fields']['alias_id']['table'] = 'mailadmin_domain_aliases';
  $handler->display->display_options['fields']['alias_id']['field'] = 'alias_id';
  $handler->display->display_options['fields']['alias_id']['label'] = '';
  $handler->display->display_options['fields']['alias_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['alias_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['alias_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['alias_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['alias_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['alias_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['alias_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['alias_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['alias_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['alias_id']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['alias_id']['separator'] = '';
  $handler->display->display_options['fields']['alias_id']['format_plural'] = 0;
  /* Field: E-mail domain alias: Source_domain_name */
  $handler->display->display_options['fields']['source_domain_name']['id'] = 'source_domain_name';
  $handler->display->display_options['fields']['source_domain_name']['table'] = 'mailadmin_domain_aliases';
  $handler->display->display_options['fields']['source_domain_name']['field'] = 'source_domain_name';
  $handler->display->display_options['fields']['source_domain_name']['label'] = 'Source';
  $handler->display->display_options['fields']['source_domain_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['source_domain_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['source_domain_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['source_domain_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['source_domain_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['source_domain_name']['hide_alter_empty'] = 0;
  /* Field: E-mail domain alias: Target_domain_name */
  $handler->display->display_options['fields']['target_domain_name']['id'] = 'target_domain_name';
  $handler->display->display_options['fields']['target_domain_name']['table'] = 'mailadmin_domain_aliases';
  $handler->display->display_options['fields']['target_domain_name']['field'] = 'target_domain_name';
  $handler->display->display_options['fields']['target_domain_name']['label'] = 'Target';
  $handler->display->display_options['fields']['target_domain_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['target_domain_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['target_domain_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['target_domain_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['target_domain_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['target_domain_name']['hide_alter_empty'] = 0;
  /* Field: Edit link */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Edit link';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'mailadmin/alias/[alias_id]';
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
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;

  $views[$view->name] = $view;

  return $views;
}
