<?php

/**
 * @file
 * Default views integration to add Views export features to the existing Views.
 */

/**
 * Implements hook_views_default_views().
 */
function regcode_ie_views_default_views_alter(&$views) {
  if (isset($views['regcode'])) {
    $view = $views['regcode'];

    /* Display: Data export */
    $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_csv');
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
    $handler->display->display_options['path'] = 'admin/config/people/regcode/export';
    $handler->display->display_options['displays'] = array(
      'page_admin' => 'page_admin',
      'default' => 0,
    );
  }
}
