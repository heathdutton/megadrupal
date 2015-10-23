<?php
/**
 * @file
 * Theme functions for time_tracker.
 *
 */

/**
 * Theme the widget menu table as a sortable list of menu items.
 *
 * @ingroup themeable
 * @see widget_menu_settings_form()
 */
function theme_widget_menu_entry_table($form) {
  $form = reset($form);
  // For some reason form comes in an empty array...
  if(!empty($form)){
    $rows = array();
    // Take all the form elements and format theme for theme_table
    foreach (element_children($form) as $key) {
      if (isset($form[$key]['title'])) {
        // Add the table drag functionality
        drupal_add_tabledrag('widget-menu-table', 'order', 'sibling', 'menu-weight');
        // Add class to group weight fields for drag and drop.
        $form[$key]['weight']['#attributes']['class'] = array('menu-weight');
        // array to store row data
        $rows[] = array(
          'data' => array(
            drupal_render($form[$key]['title']),
            drupal_render($form[$key]['path']),
            drupal_render($form[$key]['fa_icon']),
            drupal_render($form[$key]['class']),
            drupal_render($form[$key]['weight']),
            drupal_render($form[$key]['status']),
            drupal_render($form[$key]['delete']),
          ),
          'class' => array('draggable'),
        );
      }
    }
    // The table headers
    $header = array(
      t('Title'),
      t('Path'),
      t('Icon'),
      t('Class'),
      t('weight'),
      t('Enabled'),
      t('Delete'),
    );

    $table = array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => array('widget-menu-table'),
        'max-width' => '100%',
      ),
    );
    // Theme it as a table
    $output = theme('table', $table);

    // Render the form
    $output .= drupal_render_children($form);
  }
  else {
    $output = drupal_render_children($form);
  }

  // Return the themed activities table
  return $output;
}
