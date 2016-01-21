<?php

/**
 * @file
 * Theming functions for Multi-Step Registration module.
 */

/**
 * Create table from register steps list form.
 *
 * @param array $variables
 *   Array that contains list form.
 *
 * @return string
 *   Rendered output.
 */
function theme_step_list_form($variables) {
  $header = array(t('title'), t('Machine name'), t('Weight'), t('Actions'));
  $table_id = 'step_steps_table';

  // Get form that should be rendered.
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['step_steps']) as $step) {

    $rows[] = array(
      'data' => array(
        drupal_render($form['step_steps'][$step]['title']),
        drupal_render($form['step_steps'][$step]['id']),
        drupal_render($form['step_steps'][$step]['weight']),
        drupal_render($form['step_steps'][$step]['actions']),
      ),
      'class' => $step == STEP_REGISTER ? array('step-register') : array('draggable'),
      'colspan' => $step == '__new' ? 2 : 1,
    );
  }

  // Render table with registration steps.
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'step-weight');

  return $output . drupal_render_children($form);
}

/**
 * Base theme for displaying the registration step description on top of each
 * wizard step form.
 *
 * @param array $variables
 *   Array that contains step values:
 *   - id: The step ID.
 *   - title: The step title.
 *   - description: The un-formatted description text as is stored.
 *   - format: The format to be applied.
 *
 * @return string
 *   Rendered output.
 */
function theme_step_description($variables) {

  $output = '<div class="step-description">';
  $output .= check_markup($variables['value'], $variables['format']);
  $output .= '</div>';

  return $output;
}
