<?php

/**
* @file
* Implements hook_views_default_views()
*/

function ftp_put_views_default_views() {
  // Begin copy and paste of output from the Export tab of a view.

$view = new view;
$view->name = 'ftp_put';
$view->description = 'View for FTP Put module stats and errors';
$view->tag = 'default';
$view->base_table = 'ftp_put_stats';
$view->human_name = 'ftp_put';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'ftp_put_stats';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: ftp put Stats: ID */
$handler->display->display_options['fields']['fid']['id'] = 'fid';
$handler->display->display_options['fields']['fid']['table'] = 'ftp_put_stats';
$handler->display->display_options['fields']['fid']['field'] = 'fid';
/* Field: ftp put Stats: Timestamp */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'ftp_put_stats';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
$handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
/* Field: ftp put Stats: User ID */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'ftp_put_stats';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['uid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['uid']['alter']['external'] = 0;
$handler->display->display_options['fields']['uid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['uid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['uid']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['uid']['alter']['html'] = 0;
$handler->display->display_options['fields']['uid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['uid']['hide_empty'] = 0;
$handler->display->display_options['fields']['uid']['empty_zero'] = 0;
$handler->display->display_options['fields']['uid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['uid']['format_plural'] = 0;
/* Field: ftp put Stats: FTP server IP */
$handler->display->display_options['fields']['ftp_ip']['id'] = 'ftp_ip';
$handler->display->display_options['fields']['ftp_ip']['table'] = 'ftp_put_stats';
$handler->display->display_options['fields']['ftp_ip']['field'] = 'ftp_ip';
$handler->display->display_options['fields']['ftp_ip']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['external'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['ftp_ip']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['ftp_ip']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['trim'] = 0;
$handler->display->display_options['fields']['ftp_ip']['alter']['html'] = 0;
$handler->display->display_options['fields']['ftp_ip']['element_label_colon'] = 1;
$handler->display->display_options['fields']['ftp_ip']['element_default_classes'] = 1;
$handler->display->display_options['fields']['ftp_ip']['hide_empty'] = 0;
$handler->display->display_options['fields']['ftp_ip']['empty_zero'] = 0;
$handler->display->display_options['fields']['ftp_ip']['hide_alter_empty'] = 1;
/* Field: ftp put Stats: Files hashes */
$handler->display->display_options['fields']['hash']['id'] = 'hash';
$handler->display->display_options['fields']['hash']['table'] = 'ftp_put_stats';
$handler->display->display_options['fields']['hash']['field'] = 'hash';
$handler->display->display_options['fields']['hash']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['hash']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['hash']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['hash']['alter']['external'] = 0;
$handler->display->display_options['fields']['hash']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['hash']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['hash']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['hash']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['hash']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['hash']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['hash']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['hash']['alter']['trim'] = 0;
$handler->display->display_options['fields']['hash']['alter']['html'] = 0;
$handler->display->display_options['fields']['hash']['element_label_colon'] = 1;
$handler->display->display_options['fields']['hash']['element_default_classes'] = 1;
$handler->display->display_options['fields']['hash']['hide_empty'] = 0;
$handler->display->display_options['fields']['hash']['empty_zero'] = 0;
$handler->display->display_options['fields']['hash']['hide_alter_empty'] = 1;
/* Field: ftp put Stats: Errors */
$handler->display->display_options['fields']['error']['id'] = 'error';
$handler->display->display_options['fields']['error']['table'] = 'ftp_put_stats';
$handler->display->display_options['fields']['error']['field'] = 'error';
$handler->display->display_options['fields']['error']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['error']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['error']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['error']['alter']['external'] = 0;
$handler->display->display_options['fields']['error']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['error']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['error']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['error']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['error']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['error']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['error']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['error']['alter']['trim'] = 0;
$handler->display->display_options['fields']['error']['alter']['html'] = 0;
$handler->display->display_options['fields']['error']['element_label_colon'] = 1;
$handler->display->display_options['fields']['error']['element_default_classes'] = 1;
$handler->display->display_options['fields']['error']['hide_empty'] = 0;
$handler->display->display_options['fields']['error']['empty_zero'] = 0;
$handler->display->display_options['fields']['error']['hide_alter_empty'] = 1;
/* Sort criterion: Broken/missing handler */
$handler->display->display_options['sorts']['ftp_ip']['id'] = 'ftp_ip';
$handler->display->display_options['sorts']['ftp_ip']['table'] = 'ftp_put_stats';
$handler->display->display_options['sorts']['ftp_ip']['field'] = 'ftp_ip';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'ftp-put-stats';


  // End copy and paste of Export tab output.

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}
