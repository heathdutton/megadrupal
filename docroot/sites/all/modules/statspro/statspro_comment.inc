<?php

/**
 * @file
 *   Comment report.
 *
 */
require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro_settings.inc';

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_comment() {
  $view = views_new_view();

  $view->name = 'statspro_comment';
  $view->description = 'New comments';
  $view->tag = 'Statistics Pro';
  $view->view_php = '';
  $view->base_table = 'statspro';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'day' => array(
      'label' => 'Date',
      'date_format' => 'custom',
      'custom_date_format' => 'd. F Y',
      'exclude' => 0,
      'id' => 'day',
      'table' => 'statspro',
      'field' => 'day',
      'relationship' => 'none',
    ),
    'comment' => array(
      'label' => 'Comments',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => '.',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'comment',
      'table' => 'statspro',
      'field' => 'comment',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'display_mode' => 'graph',
      'graph_size' => '350',
    ),
    'comment_1' => array(
      'label' => 'Trend',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'comment_1',
      'table' => 'statspro',
      'field' => 'comment',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'display_mode' => 'trend',
      'graph_size' => '',
    ),
    'comment_2' => array(
      'label' => 'Amount',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => '.',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'comment_2',
      'table' => 'statspro',
      'field' => 'comment',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'display_mode' => 'text',
      'graph_size' => '',
    ),
  ));
  $handler->override_option('sorts', array(
    'day' => array(
      'order' => 'DESC',
      'id' => 'day',
      'table' => 'statspro',
      'field' => 'day',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access statistics pro reports',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Statistics Pro');
  $handler->override_option('header', 'New comments.');
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'day' => 'day',
      'comment' => 'comment',
      'comment_1' => 'comment_1',
      'comment_2' => 'comment_2',
    ),
    'info' => array(
      'day' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'comment' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'comment_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'comment_2' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'day',
  ));

  $handler = $view->new_display('page', 'New comments table', 'page_1');
  $handler->override_option('path', 'admin/reports/statspro/comment');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Comments',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  if (module_exists('views_charts')) {
    $handler = $view->new_display('attachment', 'New comments chart', 'attachment_1');
    $handler->override_option('fields', array(
      'day' => array(
        'label' => 'Date',
        'date_format' => 'custom',
        'custom_date_format' => 'd. F Y',
        'exclude' => 0,
        'id' => 'day',
        'table' => 'statspro',
        'field' => 'day',
        'relationship' => 'none',
      ),
      'comment' => array(
        'label' => 'Comments',
        'set_precision' => FALSE,
        'precision' => 0,
        'decimal' => '.',
        'separator' => '.',
        'prefix' => '',
        'suffix' => '',
        'exclude' => 0,
        'id' => 'comment',
        'table' => 'statspro',
        'field' => 'comment',
        'override' => array(
          'button' => 'Override',
        ),
        'relationship' => 'none',
        'display_mode' => 'graph',
        'graph_size' => '350',
      ),
    ));
    $handler->override_option('style_plugin', 'charts');
    $handler->override_option('style_options', array(
      'views_charts_series_fields' => array(
        'comment' => 'comment',
      ),
      'views_charts_x_labels' => 'day',
      'width' => '700',
      'height' => '200',
      'engine' => statspro_get_current_charting_option(),
      'type' => array(
        'amcharts' => 'line',
        'bluff' => 'line',
        'open-flash' => 'line',
      ),
      'wmode' => 'window',
      'y_min' => '0',
      'y_legend' => '',
    ));
    $handler->override_option('attachment_position', 'after');
    $handler->override_option('inherit_arguments', TRUE);
    $handler->override_option('inherit_exposed_filters', FALSE);
    $handler->override_option('inherit_pager', FALSE);
    $handler->override_option('render_pager', TRUE);
    $handler->override_option('displays', array(
      'page_1' => 'page_1',
      'default' => 0,
    ));
  }

  return $view->preview('page_1', array());
}
