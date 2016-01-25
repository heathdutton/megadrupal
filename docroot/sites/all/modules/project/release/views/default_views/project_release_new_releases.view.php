<?php

if (module_exists('sampler')) {
$view = new view;
$view->name = 'project_release_new_releases';
$view->description = 'Sparkline showing count of new releases over time for a given project';
$view->tag = 'Metrics';
$view->view_php = '';
$view->base_table = 'sampler_project_release_new_releases';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'timestamp' => array(
    'label' => 'Timestamp',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'date_format' => 'small',
    'custom_date_format' => '',
    'exclude' => 0,
    'id' => 'timestamp',
    'table' => 'sampler_project_release_new_releases',
    'field' => 'timestamp',
    'relationship' => 'none',
  ),
  'value_0' => array(
    'label' => 'New release count',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'set_precision' => FALSE,
    'precision' => 0,
    'decimal' => '.',
    'separator' => ',',
    'prefix' => '',
    'suffix' => '',
    'exclude' => 0,
    'id' => 'value_0',
    'table' => 'sampler_project_release_new_releases',
    'field' => 'value_0',
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'timestamp' => array(
    'order' => 'ASC',
    'granularity' => 'second',
    'id' => 'timestamp',
    'table' => 'sampler_project_release_new_releases',
    'field' => 'timestamp',
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'object_id' => array(
    'default_action' => 'not found',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => 'New releases for %1',
    'breadcrumb' => '',
    'default_argument_type' => 'node',
    'default_argument' => '',
    'validate_type' => 'project_nid',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'object_id',
    'table' => 'sampler_project_release_new_releases',
    'field' => 'object_id',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'timestamp' => array(
    'operator' => 'between',
    'value' => array(
      'type' => 'offset',
      'value' => '',
      'min' => '-1 year',
      'max' => 'today',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'timestamp',
    'table' => 'sampler_project_release_new_releases',
    'field' => 'timestamp',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('items_per_page', 0);
$handler->override_option('style_plugin', 'views_sparkline');
$handler->override_option('style_options', array(
  'grouping' => '',
  'x_field' => 'timestamp',
  'y_field' => 'value_0',
  'width' => '300px',
  'height' => '30px',
  'color' => '#336699',
));
} // module_exists('sampler')
