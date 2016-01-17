<?php
/**
 * Box deletion form.
 */
function configurable_box_reset_form($form, $form_state, $box) {
  $form['delta'] = array(
    '#type' => 'hidden',
    '#value' => $box->delta,
  );
  if (($box->export_type & EXPORT_IN_DATABASE) && ($box->export_type & EXPORT_IN_CODE)) {
    return confirm_form($form, t('Are you sure you want to reset the block %name?', array('%name' => $box->title)), 'admin/structure/block', '', t('Revert'), t('Cancel'));
  }

  drupal_not_found();
  die;
}

/**
 * Submit handler for boxes_delete_form
 */
function configurable_box_reset_form_submit($form, &$form_state) {
  boxes_box_load($form_state['values']['delta'])->reset_default();
  $form_state['redirect'] = 'admin/structure/block';
}

