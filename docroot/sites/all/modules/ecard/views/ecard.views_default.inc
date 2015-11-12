<?php

/**
 * Implements hook_views_default_views().
 */
function ecard_views_default_views() {
  /**
   * Ecards report
   */
  $view = new view();
  $view->name = 'ecards_report';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'ecard';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Ecards sent';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access site reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'sent_time_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'hash' => 'hash',
    'name_from' => 'name_from',
    'sent_time' => 'sent_time',
    'mail_from' => 'mail_from',
    'name_to' => 'name_to',
    'mail_to' => 'mail_to',
    'nothing' => 'nothing',
    'sent_time_1' => 'sent_time_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'hash' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_from' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sent_time' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail_from' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_to' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail_to' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sent_time_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['label'] = 'Summary count';
  $handler->display->display_options['header']['result']['content'] = '<h3 class="results-count">Displaying @start - @end of @total</h3>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'area';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['content'] = 'No ecards found.';
  $handler->display->display_options['empty']['text']['format'] = '1';
  /* Field: Ecard: Hash */
  $handler->display->display_options['fields']['hash']['id'] = 'hash';
  $handler->display->display_options['fields']['hash']['table'] = 'ecard';
  $handler->display->display_options['fields']['hash']['field'] = 'hash';
  $handler->display->display_options['fields']['hash']['label'] = 'Ecard id';
  /* Field: Ecard: Name From */
  $handler->display->display_options['fields']['name_from']['id'] = 'name_from';
  $handler->display->display_options['fields']['name_from']['table'] = 'ecard';
  $handler->display->display_options['fields']['name_from']['field'] = 'name_from';
  $handler->display->display_options['fields']['name_from']['label'] = 'Sender';
  /* Field: Ecard: Sent time */
  $handler->display->display_options['fields']['sent_time']['id'] = 'sent_time';
  $handler->display->display_options['fields']['sent_time']['table'] = 'ecard';
  $handler->display->display_options['fields']['sent_time']['field'] = 'sent_time';
  $handler->display->display_options['fields']['sent_time']['date_format'] = 'custom';
  $handler->display->display_options['fields']['sent_time']['custom_date_format'] = 'd/m/Y H:i:s';
  /* Field: Ecard: Mail From */
  $handler->display->display_options['fields']['mail_from']['id'] = 'mail_from';
  $handler->display->display_options['fields']['mail_from']['table'] = 'ecard';
  $handler->display->display_options['fields']['mail_from']['field'] = 'mail_from';
  $handler->display->display_options['fields']['mail_from']['label'] = 'Sender\'s email';
  /* Field: Ecard: Name to */
  $handler->display->display_options['fields']['name_to']['id'] = 'name_to';
  $handler->display->display_options['fields']['name_to']['table'] = 'ecard';
  $handler->display->display_options['fields']['name_to']['field'] = 'name_to';
  $handler->display->display_options['fields']['name_to']['label'] = 'Recipient\'s name';
  /* Field: Ecard: Mait to */
  $handler->display->display_options['fields']['mail_to']['id'] = 'mail_to';
  $handler->display->display_options['fields']['mail_to']['table'] = 'ecard';
  $handler->display->display_options['fields']['mail_to']['field'] = 'mail_to';
  $handler->display->display_options['fields']['mail_to']['label'] = 'Recipient\'s email';
  /* Field: Resend link */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Resend link';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'resend';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'ecard/resend/[hash]';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Ecard: Sent time */
  $handler->display->display_options['fields']['sent_time_1']['id'] = 'sent_time_1';
  $handler->display->display_options['fields']['sent_time_1']['table'] = 'ecard';
  $handler->display->display_options['fields']['sent_time_1']['field'] = 'sent_time';
  $handler->display->display_options['fields']['sent_time_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['sent_time_1']['date_format'] = 'short';
  /* Sort criterion: Ecard: Sent time */
  $handler->display->display_options['sorts']['sent_time']['id'] = 'sent_time';
  $handler->display->display_options['sorts']['sent_time']['table'] = 'ecard';
  $handler->display->display_options['sorts']['sent_time']['field'] = 'sent_time';
  $handler->display->display_options['sorts']['sent_time']['order'] = 'DESC';
  /* Filter criterion: Ecard: Sent */
  $handler->display->display_options['filters']['sent']['id'] = 'sent';
  $handler->display->display_options['filters']['sent']['table'] = 'ecard';
  $handler->display->display_options['filters']['sent']['field'] = 'sent';
  $handler->display->display_options['filters']['sent']['value'] = '1';
  $handler->display->display_options['filters']['sent']['group'] = 1;
  $handler->display->display_options['filters']['sent']['expose']['operator'] = FALSE;
  /* Filter criterion: Ecard: Sent time */
  $handler->display->display_options['filters']['sent_time']['id'] = 'sent_time';
  $handler->display->display_options['filters']['sent_time']['table'] = 'ecard';
  $handler->display->display_options['filters']['sent_time']['field'] = 'sent_time';
  $handler->display->display_options['filters']['sent_time']['operator'] = 'between';
  $handler->display->display_options['filters']['sent_time']['group'] = 1;
  $handler->display->display_options['filters']['sent_time']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sent_time']['expose']['operator_id'] = 'sent_time_op';
  $handler->display->display_options['filters']['sent_time']['expose']['label'] = 'Sent time (between)';
  $handler->display->display_options['filters']['sent_time']['expose']['description'] = 'format: yyyy-mm-dd';
  $handler->display->display_options['filters']['sent_time']['expose']['operator'] = 'sent_time_op';
  $handler->display->display_options['filters']['sent_time']['expose']['identifier'] = 'sent_time';
  $handler->display->display_options['filters']['sent_time']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Hash */
  $handler->display->display_options['filters']['hash']['id'] = 'hash';
  $handler->display->display_options['filters']['hash']['table'] = 'ecard';
  $handler->display->display_options['filters']['hash']['field'] = 'hash';
  $handler->display->display_options['filters']['hash']['operator'] = 'starts';
  $handler->display->display_options['filters']['hash']['group'] = 1;
  $handler->display->display_options['filters']['hash']['exposed'] = TRUE;
  $handler->display->display_options['filters']['hash']['expose']['operator_id'] = 'hash_op';
  $handler->display->display_options['filters']['hash']['expose']['label'] = 'Ecard Id';
  $handler->display->display_options['filters']['hash']['expose']['operator'] = 'hash_op';
  $handler->display->display_options['filters']['hash']['expose']['identifier'] = 'hash';
  $handler->display->display_options['filters']['hash']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Mail From */
  $handler->display->display_options['filters']['mail_from']['id'] = 'mail_from';
  $handler->display->display_options['filters']['mail_from']['table'] = 'ecard';
  $handler->display->display_options['filters']['mail_from']['field'] = 'mail_from';
  $handler->display->display_options['filters']['mail_from']['group'] = 1;
  $handler->display->display_options['filters']['mail_from']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail_from']['expose']['operator_id'] = 'mail_from_op';
  $handler->display->display_options['filters']['mail_from']['expose']['label'] = 'Sender\'s email';
  $handler->display->display_options['filters']['mail_from']['expose']['operator'] = 'mail_from_op';
  $handler->display->display_options['filters']['mail_from']['expose']['identifier'] = 'mail_from';
  $handler->display->display_options['filters']['mail_from']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Mait to */
  $handler->display->display_options['filters']['mail_to']['id'] = 'mail_to';
  $handler->display->display_options['filters']['mail_to']['table'] = 'ecard';
  $handler->display->display_options['filters']['mail_to']['field'] = 'mail_to';
  $handler->display->display_options['filters']['mail_to']['group'] = 1;
  $handler->display->display_options['filters']['mail_to']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail_to']['expose']['operator_id'] = 'mail_to_op';
  $handler->display->display_options['filters']['mail_to']['expose']['label'] = 'Recipient\'s email';
  $handler->display->display_options['filters']['mail_to']['expose']['operator'] = 'mail_to_op';
  $handler->display->display_options['filters']['mail_to']['expose']['identifier'] = 'mail_to';
  $handler->display->display_options['filters']['mail_to']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Name From */
  $handler->display->display_options['filters']['name_from']['id'] = 'name_from';
  $handler->display->display_options['filters']['name_from']['table'] = 'ecard';
  $handler->display->display_options['filters']['name_from']['field'] = 'name_from';
  $handler->display->display_options['filters']['name_from']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_from']['group'] = 1;
  $handler->display->display_options['filters']['name_from']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_from']['expose']['operator_id'] = 'name_from_op';
  $handler->display->display_options['filters']['name_from']['expose']['label'] = 'Sender';
  $handler->display->display_options['filters']['name_from']['expose']['operator'] = 'name_from_op';
  $handler->display->display_options['filters']['name_from']['expose']['identifier'] = 'name_from';
  $handler->display->display_options['filters']['name_from']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Name to */
  $handler->display->display_options['filters']['name_to']['id'] = 'name_to';
  $handler->display->display_options['filters']['name_to']['table'] = 'ecard';
  $handler->display->display_options['filters']['name_to']['field'] = 'name_to';
  $handler->display->display_options['filters']['name_to']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_to']['group'] = 1;
  $handler->display->display_options['filters']['name_to']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_to']['expose']['operator_id'] = 'name_to_op';
  $handler->display->display_options['filters']['name_to']['expose']['label'] = 'Recipient\'s name';
  $handler->display->display_options['filters']['name_to']['expose']['operator'] = 'name_to_op';
  $handler->display->display_options['filters']['name_to']['expose']['identifier'] = 'name_to';
  $handler->display->display_options['filters']['name_to']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page - Sent */
  $handler = $view->new_display('page', 'Page - Sent', 'ecards_sent_page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/ecards/sent';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Sent';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Ecards';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Page - Scheduled */
  $handler = $view->new_display('page', 'Page - Scheduled', 'ecards_scheduled_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Ecards scheduled';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'scheduled_time',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'hash' => 'hash',
    'name_from' => 'name_from',
    'mail_from' => 'mail_from',
    'name_to' => 'name_to',
    'mail_to' => 'mail_to',
    'nothing' => 'nothing',
    'scheduled_time' => 'scheduled_time',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'hash' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_from' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail_from' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_to' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail_to' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'scheduled_time' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Ecard: Hash */
  $handler->display->display_options['fields']['hash']['id'] = 'hash';
  $handler->display->display_options['fields']['hash']['table'] = 'ecard';
  $handler->display->display_options['fields']['hash']['field'] = 'hash';
  $handler->display->display_options['fields']['hash']['label'] = 'Ecard id';
  /* Field: Ecard: Name From */
  $handler->display->display_options['fields']['name_from']['id'] = 'name_from';
  $handler->display->display_options['fields']['name_from']['table'] = 'ecard';
  $handler->display->display_options['fields']['name_from']['field'] = 'name_from';
  $handler->display->display_options['fields']['name_from']['label'] = 'Sender';
  /* Field: Ecard: Mail From */
  $handler->display->display_options['fields']['mail_from']['id'] = 'mail_from';
  $handler->display->display_options['fields']['mail_from']['table'] = 'ecard';
  $handler->display->display_options['fields']['mail_from']['field'] = 'mail_from';
  $handler->display->display_options['fields']['mail_from']['label'] = 'Sender\'s email';
  /* Field: Ecard: Name to */
  $handler->display->display_options['fields']['name_to']['id'] = 'name_to';
  $handler->display->display_options['fields']['name_to']['table'] = 'ecard';
  $handler->display->display_options['fields']['name_to']['field'] = 'name_to';
  $handler->display->display_options['fields']['name_to']['label'] = 'Recipient\'s name';
  /* Field: Ecard: Mait to */
  $handler->display->display_options['fields']['mail_to']['id'] = 'mail_to';
  $handler->display->display_options['fields']['mail_to']['table'] = 'ecard';
  $handler->display->display_options['fields']['mail_to']['field'] = 'mail_to';
  $handler->display->display_options['fields']['mail_to']['label'] = 'Recipient\'s email';
  /* Field: Send link */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Send link';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'send';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'ecard/resend/[hash]';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Ecard: Scheduled time */
  $handler->display->display_options['fields']['scheduled_time']['id'] = 'scheduled_time';
  $handler->display->display_options['fields']['scheduled_time']['table'] = 'ecard';
  $handler->display->display_options['fields']['scheduled_time']['field'] = 'scheduled_time';
  $handler->display->display_options['fields']['scheduled_time']['exclude'] = TRUE;
  $handler->display->display_options['fields']['scheduled_time']['date_format'] = 'short';
  $handler->display->display_options['fields']['scheduled_time']['custom_date_format'] = 'u';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Ecard: Scheduled time */
  $handler->display->display_options['sorts']['scheduled_time']['id'] = 'scheduled_time';
  $handler->display->display_options['sorts']['scheduled_time']['table'] = 'ecard';
  $handler->display->display_options['sorts']['scheduled_time']['field'] = 'scheduled_time';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Ecard: Sent */
  $handler->display->display_options['filters']['sent']['id'] = 'sent';
  $handler->display->display_options['filters']['sent']['table'] = 'ecard';
  $handler->display->display_options['filters']['sent']['field'] = 'sent';
  $handler->display->display_options['filters']['sent']['value'] = '0';
  $handler->display->display_options['filters']['sent']['group'] = 1;
  $handler->display->display_options['filters']['sent']['expose']['operator'] = FALSE;
  /* Filter criterion: Ecard: Scheduled time */
  $handler->display->display_options['filters']['scheduled_time']['id'] = 'scheduled_time';
  $handler->display->display_options['filters']['scheduled_time']['table'] = 'ecard';
  $handler->display->display_options['filters']['scheduled_time']['field'] = 'scheduled_time';
  $handler->display->display_options['filters']['scheduled_time']['operator'] = '>';
  $handler->display->display_options['filters']['scheduled_time']['value']['value'] = '-30 years';
  $handler->display->display_options['filters']['scheduled_time']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['scheduled_time']['group'] = 1;
  /* Filter criterion: Ecard: Scheduled time */
  $handler->display->display_options['filters']['scheduled_time_1']['id'] = 'scheduled_time_1';
  $handler->display->display_options['filters']['scheduled_time_1']['table'] = 'ecard';
  $handler->display->display_options['filters']['scheduled_time_1']['field'] = 'scheduled_time';
  $handler->display->display_options['filters']['scheduled_time_1']['operator'] = 'between';
  $handler->display->display_options['filters']['scheduled_time_1']['group'] = 1;
  $handler->display->display_options['filters']['scheduled_time_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['scheduled_time_1']['expose']['operator_id'] = 'scheduled_time_1_op';
  $handler->display->display_options['filters']['scheduled_time_1']['expose']['label'] = 'Scheduled time';
  $handler->display->display_options['filters']['scheduled_time_1']['expose']['description'] = 'format: yyyy-mm-dd';
  $handler->display->display_options['filters']['scheduled_time_1']['expose']['operator'] = 'scheduled_time_1_op';
  $handler->display->display_options['filters']['scheduled_time_1']['expose']['identifier'] = 'scheduled_time_1';
  $handler->display->display_options['filters']['scheduled_time_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Hash */
  $handler->display->display_options['filters']['hash']['id'] = 'hash';
  $handler->display->display_options['filters']['hash']['table'] = 'ecard';
  $handler->display->display_options['filters']['hash']['field'] = 'hash';
  $handler->display->display_options['filters']['hash']['operator'] = 'starts';
  $handler->display->display_options['filters']['hash']['group'] = 1;
  $handler->display->display_options['filters']['hash']['exposed'] = TRUE;
  $handler->display->display_options['filters']['hash']['expose']['operator_id'] = 'hash_op';
  $handler->display->display_options['filters']['hash']['expose']['label'] = 'Ecard Id';
  $handler->display->display_options['filters']['hash']['expose']['operator'] = 'hash_op';
  $handler->display->display_options['filters']['hash']['expose']['identifier'] = 'hash';
  $handler->display->display_options['filters']['hash']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Mail From */
  $handler->display->display_options['filters']['mail_from']['id'] = 'mail_from';
  $handler->display->display_options['filters']['mail_from']['table'] = 'ecard';
  $handler->display->display_options['filters']['mail_from']['field'] = 'mail_from';
  $handler->display->display_options['filters']['mail_from']['group'] = 1;
  $handler->display->display_options['filters']['mail_from']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail_from']['expose']['operator_id'] = 'mail_from_op';
  $handler->display->display_options['filters']['mail_from']['expose']['label'] = 'Sender\'s email';
  $handler->display->display_options['filters']['mail_from']['expose']['operator'] = 'mail_from_op';
  $handler->display->display_options['filters']['mail_from']['expose']['identifier'] = 'mail_from';
  $handler->display->display_options['filters']['mail_from']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Mait to */
  $handler->display->display_options['filters']['mail_to']['id'] = 'mail_to';
  $handler->display->display_options['filters']['mail_to']['table'] = 'ecard';
  $handler->display->display_options['filters']['mail_to']['field'] = 'mail_to';
  $handler->display->display_options['filters']['mail_to']['group'] = 1;
  $handler->display->display_options['filters']['mail_to']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail_to']['expose']['operator_id'] = 'mail_to_op';
  $handler->display->display_options['filters']['mail_to']['expose']['label'] = 'Recipient\'s email';
  $handler->display->display_options['filters']['mail_to']['expose']['operator'] = 'mail_to_op';
  $handler->display->display_options['filters']['mail_to']['expose']['identifier'] = 'mail_to';
  $handler->display->display_options['filters']['mail_to']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Name From */
  $handler->display->display_options['filters']['name_from']['id'] = 'name_from';
  $handler->display->display_options['filters']['name_from']['table'] = 'ecard';
  $handler->display->display_options['filters']['name_from']['field'] = 'name_from';
  $handler->display->display_options['filters']['name_from']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_from']['group'] = 1;
  $handler->display->display_options['filters']['name_from']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_from']['expose']['operator_id'] = 'name_from_op';
  $handler->display->display_options['filters']['name_from']['expose']['label'] = 'Sender';
  $handler->display->display_options['filters']['name_from']['expose']['operator'] = 'name_from_op';
  $handler->display->display_options['filters']['name_from']['expose']['identifier'] = 'name_from';
  $handler->display->display_options['filters']['name_from']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Ecard: Name to */
  $handler->display->display_options['filters']['name_to']['id'] = 'name_to';
  $handler->display->display_options['filters']['name_to']['table'] = 'ecard';
  $handler->display->display_options['filters']['name_to']['field'] = 'name_to';
  $handler->display->display_options['filters']['name_to']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_to']['group'] = 1;
  $handler->display->display_options['filters']['name_to']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_to']['expose']['operator_id'] = 'name_to_op';
  $handler->display->display_options['filters']['name_to']['expose']['label'] = 'Recipient\'s name';
  $handler->display->display_options['filters']['name_to']['expose']['operator'] = 'name_to_op';
  $handler->display->display_options['filters']['name_to']['expose']['identifier'] = 'name_to';
  $handler->display->display_options['filters']['name_to']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'admin/reports/ecards/scheduled';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Scheduled';
  $handler->display->display_options['menu']['weight'] = '3';

  $views[$view->name] = $view;

  return $views;
}

