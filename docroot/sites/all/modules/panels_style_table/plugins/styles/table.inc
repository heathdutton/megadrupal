<?php


/**
 * @file
 * Definition of the 'Table' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Table'),
  'description' => t('Show panel panes in a table.'),
  'render region' => 'panels_style_table_render_region',
  'settings form' => 'panels_style_table_settings_form',
);

/**
 * Merges passed settings with their defaults and returns the resulting array.
 */
function _panels_style_table_default_settings($settings) {
  $settings = (array)$settings;
  return array_merge(array(
    'columns'       => '1',
    'show_title'    => TRUE,
    'add_colon'     => TRUE,
    'show_empty'    => TRUE,
    'fill_in_table' => TRUE,
  ), !empty($settings['content']) ? $settings['content'] : array());
}

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_panels_style_table_render_region($vars) {
  global $language;

  $display = $vars['display'];
  $region_id = $vars['region_id'];
  $owner_id = $vars['owner_id'];
  $panes = array();

  $settings = _panels_style_table_default_settings($vars['settings']);

  $table = array(
    '#theme' => 'table',
    '#rows'  => array(),
    '#attributes' => array(
      'class' => array('panels-style-table', 'panels-style-table-columns-' . $settings['columns']),
    ),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'panels_style_table') . '/css/panels-style-table.css',
      ),
    ),
  );

  if ($settings['show_title']) {
    $table['#attributes']['class'][] = 'panels-style-table-show-title';
  }

  // Ensure that all the panels are represented, even if they don't have any data.
  if ($settings['show_empty'] && isset($display->panels[$region_id])) {
    foreach ($display->panels[$region_id] as $pane_id) {
      if (isset($vars['panes'][$pane_id])) {
        $panes[$pane_id] = $vars['panes'][$pane_id];
      }
      else {
        $panes[$pane_id] = '';
      }
    }
  }
  else {
    $panes = $vars['panes'];
  }

  // add filler panes so that we always fill in the table
  if ($settings['fill_in_table']) {
    $filler_count = count($panes) % $settings['columns'];
    for ($index = 0; $index < $filler_count; $index++) {
      $panes['filler-pane-' . $index] = '';
    }
  }

  $index = 0;
  $row = array();
  foreach ($panes as $pane_id => $item) {
    $row[] = array(
      'data' => $item,
      'class' => array('panels-style-table-content'),
    );

    if ($settings['show_title']) {
      $title = panels_style_table_pane_titles($display->did, $pane_id);
      $title = $title ? $title : '';

      if ($settings['add_colon'] && !empty($title)) {
        if ($language->direction == LANGUAGE_RTL) {
          $title = ':' . $title;
        }
        else {
          $title = $title . ':';
        }
      }

      $title_cell = array(
        'data' => $title,
        'class' => array('panels-style-table-title'),
      );

      if ($language->direction == LANGUAGE_RTL) {
        $row[] = $title_cell;
      }
      else {
        array_splice($row, count($row) - 1, 0, array($title_cell));
      }
    }

    $index++;
    if ($index >= $settings['columns']) {
      $index = 0;
      $table['#rows'][] = $row;
      $row = array();
    }
  }

  // put any left over cells on the table
  if (!empty($row)) {
    $table['#rows'][] = $row;
  }

  return drupal_render($table);
}

/**
 * Form constructor for the region settings form.
 *
 * @param array $style_settings
 *   The style settings.
 *
 * @ingroup forms
 */
function panels_style_table_settings_form($style_settings) {
  $defaults = _panels_style_table_default_settings($style_settings);

  $form = array();

  $form['content']['columns'] = array(
    '#title' => t('Number of columns'),
    '#description' => t('The number of columns to place before starting a new row.'),
    '#type' => 'textfield',
    '#default_value' => $defaults['columns'],
  );

  $form['content']['show_title'] = array(
    '#title' => t('Show pane titles'),
    '#description' => t('If enabled, pane titles will be shown next to the columns with there data. The title columns don\'t count against the <em>Number of columns</em> given above.'),
    '#type' => 'checkbox',
    '#default_value' => $defaults['show_title'],
  );

  $form['content']['add_colon'] = array(
    '#title' => t('Add colon to title'),
    '#description' => t('If enabled, a colon will be appended to the title.'),
    '#type' => 'checkbox',
    '#default_value' => $defaults['add_colon'],
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[content][show_title]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['content']['show_empty'] = array(
    '#title' => t('Show empty panes'),
    '#description' => t('If enabled, all panes will be displayed even if they are empty. This allows you to keep the layout of the table the same always.'),
    '#type' => 'checkbox',
    '#default_value' => $defaults['show_empty'],
  );

  $form['content']['fill_in_table'] = array(
    '#title' => t('Fill in table if not full'),
    '#description' => t('If enabled, filler table cells will be added in the last row if it isn\'t filled up.'),
    '#type' => 'checkbox',
    '#default_value' => $defaults['fill_in_table'],
  );

  return $form;
}

