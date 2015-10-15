<?php
/**
 * @file
 * This alters the menu settings on the node add/Edit page.
 */

/**
 * Implements hook_form_ID_alter().
 *
 * Removes the ('disabled' links on the node edit form.)
 */
function i18n_menu_overview_form_alter(&$form, $form_state, $form_id) {
  if ($form['#node_edit_form'] && strpos($form_id, '_node_form') !== FALSE) {
    foreach ($form['menu']['link']['parent']['#options'] as $index => $option) {
      if (strpos($option, 'disabled') !== FALSE) {
        unset($form['menu']['link']['parent']['#options'][$index]);
      }
    }
  }
}
