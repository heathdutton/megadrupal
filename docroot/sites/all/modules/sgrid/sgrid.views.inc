<?php
/**
 * @file
 * sgrid.views.inc
 * Views include file for Sortable Grid module
 *
 */
function sgrid_views_plugins() {
    $path = drupal_get_path('module', 'sgrid');
    return array(
        'style'  => array(
            'sgrid' => array(
              'title' => t('Sortable Grid'),
              'help' => t('Displays rows as a sortable HTML grid'),
              'handler' => 'views_plugin_style_sgrid',
              'theme' => 'views_view_sgrid',
              'uses row plugin' => TRUE,
              'uses row class' => TRUE,
              'uses options' => TRUE,
              'type' => 'normal',
              'help topic' => 'style-list',
            )
        ),
        'row' => array(
            'sgrid_style' => array(
                'title' => t('Sgrid Style'),
                'help' => t('(Includes the nid in any row for sort functions to work.)'),
                'handler' => 'views_plugin_row_sgrid_style',
                'theme' => 'views_view_row_sgrid_style',
                'uses fields' => TRUE,
                'uses options' => TRUE,
                'type' => 'normal',
          )
        ),
  );
}
/**
 * Implements hook_views_data().
 */
function sgrid_views_data() {
  $data = array();
  $data['sgrid']['table']['group'] = t('Sortable Grid');
  $data['sgrid']['rank'] = array(
    'title' => 'Rank',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
  ),
  'sort' => array(
    'handler' => 'views_handler_sort',
  ),
  'relationship' => array(
        'base' => 'node',
        'field' => 'nid',
        'handler' => 'views_handler_relationship',
        'label' => t('Ranked node'),
      ),
  'help' => 'This field is used to sort nodes based on user defined order.',
  );
  $data['sgrid']['table']['join'] = array(
    'node' => array(
        'left_field' => 'nid',
        'field' => 'nid',
    ),
  );
  return $data;
}

function sgrid_views_pre_render(&$view) {
       // If the style of the current display is ours, we need our save button
  if ($view->style_plugin->definition['handler'] == 'views_plugin_style_sgrid') {
    if (user_access('sort Sortable Grid Views')) {
      $view->attachment_after .= "<button id='sgrid-button-save' value='" . t('Save') . "' />Save</button><button id='sgrid-button-reset' value='Reset Order' /> " . t('Reset Order') . "</button><div id='sgrid-mess'></div>";
    }
    // We also need to add our sortable class to the view
    $view->display_handler->set_option('css_class', 'sortable');
    }
}
