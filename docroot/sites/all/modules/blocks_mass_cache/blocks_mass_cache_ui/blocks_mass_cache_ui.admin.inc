<?php
/**
 * @file
 * Blocks Mass cache admin UI.
 */

/**
 * Menu callback.
 */
function blocks_mass_cache_ui_admin_blocks($form, $form_state, $rid = NULL) {

  // Retrieve cache types to be placed on the table columns.
  $cache_types = _blocks_mass_cache_get_cache_types();
  if (is_numeric($rid)) {
    $cache_types = array($rid => $cache_types[$rid]);
  }

  // Store $cache_types in the form.
  $form['cache_types'] = array(
    '#type' => 'value',
    '#value' => $cache_types,
  );

  // Block cache configuration.
  $blocks_cache_config = _blocks_mass_cache_blocks_cache_config($cache_types);

  $options = array();
  foreach (_blocks_mass_cache_block_list() as $module_name => $block_group) {

    $form['blocks'][] = array(
      '#markup' => ucfirst($module_name),
      '#id' => $module_name,
    );

    foreach ($block_group as $block => $item) {
      $bid = _blocks_mass_cache_get_bid($item);
      $options[$bid] = '';
      $form['blocks'][$bid] = array(
        '#type' => 'item',
        '#markup' => $item['info'],
        '#description' => '',
      );
      foreach ($cache_types as $cache_id => $name) {
        // Builds arrays for checked boxes for each role.
        if (isset($blocks_cache_config[$cache_id][$bid]) && $blocks_cache_config[$cache_id][$bid] == $cache_id) {
          $status[$cache_id][$bid] = $bid;
        }
      }
    }
  }

  // Have to build checkboxes here after checkbox arrays are built.
  foreach ($cache_types as $cache_id => $name) {
    $form['checkboxes'][$cache_id] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => isset($status[$cache_id]) ? $status[$cache_id] : array(),
      '#attributes' => array('class' => array('rid-' . $cache_id)),
    );
    $form['cache_types'][$cache_id] = array('#markup' => check_plain($name), '#tree' => TRUE);
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  $form['#attached']['js'][] = drupal_get_path('module', 'blocks_mass_cache_ui') . '/blocks_mass_cache_ui.js';

  return $form;
}

/**
 * Save settings.
 */
function blocks_mass_cache_ui_admin_blocks_submit($form, &$form_state) {
  $cache_values = array();

  foreach ($form_state['values']['cache_types'] as $cache_id => $name) {
    foreach ($form_state['values'][$cache_id] as $block_id => $value) {
      if ($value != FALSE) {
        if (!isset($cache_values[$block_id])) {
          $cache_values[$block_id] = $cache_id;
        }
        else {
          // Add up cache types.
          $cache_values[$block_id] = $cache_values[$block_id] + $cache_id;
        }
      }
    }
  }

  cache_clear_all();
  variable_set('blocks_mass_cache_storage', $cache_values);
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Returns HTML for the blocks page.
 *
 * @param $variables
 *   An as  sociative array containing:
 *   - form: A render element representing the form.
 */
function theme_blocks_mass_cache_ui_admin_blocks($variables) {
  $rows = array();
  $form = $variables['form'];
  $cache_types = _blocks_mass_cache_get_cache_types();

  // Rows.
  foreach (element_children($form['blocks']) as $key) {
    $row = array();

    // Display Module name.
    if (is_numeric($key)) {
      // @todo add button to show/hide module name row.
      $row[] = array(
        'data' => '<strong>' . drupal_render($form['blocks'][$key]) . '</strong>',
        'class' => array('module'),
        'id' => 'module-' . $form['blocks'][$key]['#id'],
        'colspan' => count($form['cache_types']['#value']) + 1,
      );
    }
    else {

      // Display block row.
      $row[] = array(
        'data' => drupal_render($form['blocks'][$key]),
        'class' => array('blocks'),
        'id' => $key,
      );
      foreach (element_children($form['checkboxes']) as $cache_id) {
        $form['checkboxes'][$cache_id][$key]['#title'] = $cache_types[$cache_id] . ': ' . $form['blocks'][$key]['#markup'];
        $form['checkboxes'][$cache_id][$key]['#title_display'] = 'invisible';
        $row[] = array(
          'data' => drupal_render($form['checkboxes'][$cache_id][$key]),
          'class' => array('checkbox', $key),
        );
      }
    }
    $rows[] = $row;
  }

  // Header.
  $header[] = (t('Blocks'));
  foreach (element_children($form['cache_types']) as $cache_id) {
    $header[] = array(
      'data' => drupal_render($form['cache_types'][$cache_id]),
      'class' => array('checkbox'),
    );
  }

  $output = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('id' => 'blocks'),
      )
    ) . drupal_render_children($form);

  return $output;
}
