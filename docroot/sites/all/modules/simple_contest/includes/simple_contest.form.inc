<?php
// $Id: simple_contest.form.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $
/**
 * @file
 *
 * simple_contest.form.inc
 */



/**
 * Implementation of hook_form_alter().
 */
function _simple_contest_form_alter(&$form, $form_state, $form_id) {

  if (!empty($form['#node']->type) && 
    $form_id == $form['#node']->type .'_node_form' && 
    simple_contest_is_simple_contest($form['#node']->type)) {

    $node = $form['#node'];

    $form['simple_contest'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contest'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $options = simple_contest_status_array();

    $form['simple_contest']['status'] = array(
      '#type' => 'radios',
      '#title' => t('Contest status'),
      '#options' => $options,
      '#default_value' => isset($node->simple_contest['status'])?$node->simple_contest['status']:SIMPLE_CONTEST_STATUS_ACTIVE,
    );
    $form['simple_contest'] += array(
      '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'simple_contest') : 1,
      '#tree' => TRUE,
    );
  }
}
