<?php

/**
 * @file
 * Provide theme items for ec_charge.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_charge_component_form($variables) {
  $form = $variables['form'];
  $type = $form['#ctype'];
  $output = '';

  $head = array(t('Description'));
  if ($type != 'variable') {
    $head[] = array(
      'data' => t('Weight'),
      'class' => array($type . '-weight'),
    );
  }
  $head[] = t('Operations');
  $rows = array();

  foreach (_ec_charge_get_table_ids($form) as $key) {
    $rows[] = array(
      'data' => ec_charge_component_form_render_row($type, $form[$key]),
      'class' => array('draggable', ($form[$key]['#parent'] ? 'tabledrag-root' : 'tabledrag-leaf')),
    );
  }
  $output .= '<div id="' . $type . '-wrapper">';
  if (!empty($rows)) {
    $attributes = array('id' => $type . '-table');

    $output .= theme('table', array('header' => $head, 'rows' => $rows, 'attributes' => $attributes));
  }
  $output .= '</div>';
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_charge_admin_list_table($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('ec-charge-table', 'order', 'sibling', 'ec-charge-weight');

  $head = array(t('Charge'), t('Description'), t('Type'), t('Enabled'), t('Weight'), '');

  $rows = array();
  foreach (element_children($form) as $id) {
    $form[$id]['weight']['#attributes']['class'][] = 'ec-charge-weight';
    $rows[] = array(
      'data' => array(
        drupal_render($form[$id]['name']),
        drupal_render($form[$id]['description']),
        drupal_render($form[$id]['chg_type']),
        drupal_render($form[$id]['enabled']),
        drupal_render($form[$id]['weight']),
        drupal_render($form[$id]['operations']),
      ),
      'class' => array('draggable'),
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No charges have been created.'),
        'colspan' => 6,
        'align' => 'center',
      ),
    );
  }

  $output = theme('table', array('header' => $head, 'rows' => $rows, 'attributes' => array('id' => 'ec-charge-table')));
  $output .= drupal_render_children($form);

  return $output;
}
