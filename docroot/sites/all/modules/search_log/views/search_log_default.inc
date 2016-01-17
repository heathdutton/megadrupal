<?php
/**
 * @file
 * Default View for Search log module.
 */

$view = new view;
$view->name = 'search_log';
$view->description = 'List of of top search terms by day, module, result and language.';
$view->tag = '';
$view->base_table = 'search_log';
$view->human_name = 'Search log';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Top searches';
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'search content';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['inline'] = array(
  'q' => 'q',
  'counter' => 'counter',
);
$handler->display->display_options['row_options']['hide_empty'] = 0;
/* Field: Search log: Module */
$handler->display->display_options['fields']['module']['id'] = 'module';
$handler->display->display_options['fields']['module']['table'] = 'search_log';
$handler->display->display_options['fields']['module']['field'] = 'module';
$handler->display->display_options['fields']['module']['label'] = '';
$handler->display->display_options['fields']['module']['exclude'] = TRUE;
$handler->display->display_options['fields']['module']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['module']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['module']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['module']['alter']['external'] = 0;
$handler->display->display_options['fields']['module']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['module']['alter']['trim'] = 0;
$handler->display->display_options['fields']['module']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['module']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['module']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['module']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['module']['alter']['html'] = 0;
$handler->display->display_options['fields']['module']['element_label_colon'] = 0;
$handler->display->display_options['fields']['module']['element_default_classes'] = 1;
$handler->display->display_options['fields']['module']['hide_empty'] = 0;
$handler->display->display_options['fields']['module']['empty_zero'] = 0;
/* Field: Search log: Keys */
$handler->display->display_options['fields']['q']['id'] = 'q';
$handler->display->display_options['fields']['q']['table'] = 'search_log';
$handler->display->display_options['fields']['q']['field'] = 'q';
$handler->display->display_options['fields']['q']['label'] = '';
$handler->display->display_options['fields']['q']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['q']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['q']['alter']['path'] = 'search/[module]/[q]';
$handler->display->display_options['fields']['q']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['q']['alter']['external'] = 0;
$handler->display->display_options['fields']['q']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['q']['alter']['trim'] = 0;
$handler->display->display_options['fields']['q']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['q']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['q']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['q']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['q']['alter']['html'] = 0;
$handler->display->display_options['fields']['q']['element_label_colon'] = 0;
$handler->display->display_options['fields']['q']['element_default_classes'] = 1;
$handler->display->display_options['fields']['q']['hide_empty'] = 1;
$handler->display->display_options['fields']['q']['empty_zero'] = 0;
/* Field: Search log: Counter */
$handler->display->display_options['fields']['counter']['id'] = 'counter';
$handler->display->display_options['fields']['counter']['table'] = 'search_log';
$handler->display->display_options['fields']['counter']['field'] = 'counter';
$handler->display->display_options['fields']['counter']['group_type'] = 'sum';
$handler->display->display_options['fields']['counter']['label'] = '';
$handler->display->display_options['fields']['counter']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['counter']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['counter']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['counter']['alter']['external'] = 0;
$handler->display->display_options['fields']['counter']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['counter']['alter']['trim'] = 0;
$handler->display->display_options['fields']['counter']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['counter']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['counter']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['counter']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['counter']['alter']['html'] = 0;
$handler->display->display_options['fields']['counter']['element_label_colon'] = 0;
$handler->display->display_options['fields']['counter']['element_default_classes'] = 1;
$handler->display->display_options['fields']['counter']['hide_empty'] = 0;
$handler->display->display_options['fields']['counter']['empty_zero'] = 0;
$handler->display->display_options['fields']['counter']['format_plural'] = 0;
$handler->display->display_options['fields']['counter']['prefix'] = ' (';
$handler->display->display_options['fields']['counter']['suffix'] = ')';
/* Sort criterion: Search log: Counter */
$handler->display->display_options['sorts']['counter']['id'] = 'counter';
$handler->display->display_options['sorts']['counter']['table'] = 'search_log';
$handler->display->display_options['sorts']['counter']['field'] = 'counter';
$handler->display->display_options['sorts']['counter']['group_type'] = 'sum';
$handler->display->display_options['sorts']['counter']['order'] = 'DESC';
/* Filter criterion: Search log: Result */
$handler->display->display_options['filters']['result']['id'] = 'result';
$handler->display->display_options['filters']['result']['table'] = 'search_log';
$handler->display->display_options['filters']['result']['field'] = 'result';
$handler->display->display_options['filters']['result']['operator'] = 'not in';
$handler->display->display_options['filters']['result']['value'] = array(
  -1 => '-1',
);
/* Filter criterion: Search log: Day */
$handler->display->display_options['filters']['day']['id'] = 'day';
$handler->display->display_options['filters']['day']['table'] = 'search_log';
$handler->display->display_options['filters']['day']['field'] = 'day';
$handler->display->display_options['filters']['day']['operator'] = '>=';
$handler->display->display_options['filters']['day']['value']['value'] = '-7 days';
$handler->display->display_options['filters']['day']['value']['type'] = 'offset';
/* Filter criterion: Search log: Module */
$handler->display->display_options['filters']['module']['id'] = 'module';
$handler->display->display_options['filters']['module']['table'] = 'search_log';
$handler->display->display_options['filters']['module']['field'] = 'module';
$handler->display->display_options['filters']['module']['value'] = array(
  'node' => 'node',
);

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block');
$handler->display->display_options['block_description'] = 'Top searches (Search log)';
$translatables['search_log'] = array(
  t('Master'),
  t('Top searches'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('search/[module]/[q]'),
  t('.'),
  t(','),
  t(' ('),
  t(')'),
  t('Block'),
  t('Top searches (Search log)'),
);
