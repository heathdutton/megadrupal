<?php
/**
 * @file
 * AJAX readmore settings form.
 */

/**
 * AJAX readmore settings form.
 */
function read_more_ajax_settings() {
  $form = array();
  $form['read_more_ajax_node_types'] = array(
    '#title' => t('Content types'),
    '#type' => 'checkboxes',
    '#description' => t('Select node types you want to activate ajax readmore on. If you select nothing, AJAX will be enabled everywhere.'),
    '#default_value' => variable_get('read_more_ajax_node_types', array()),
    '#options' => node_type_get_names(),
  );
  $form['read_more_ajax_move_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled on scroll up.'),
    '#default_value' => variable_get('read_more_ajax_move_on', TRUE),
  );
  $form['read_more_ajax_read_less'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled read less'),
    '#default_value' => variable_get('read_more_ajax_read_less', TRUE),
  );
  $form['read_more_ajax_move_scroll_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed scroll up'),
    '#default_value' => variable_get('read_more_ajax_move_scroll_speed', 350),
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 10,
    '#states' => array(
      'visible' => array(
        'input[name="read_more_ajax_move_on"]' => array('checked' => TRUE),
      ),
    ),
  );
  return system_settings_form($form);
}
