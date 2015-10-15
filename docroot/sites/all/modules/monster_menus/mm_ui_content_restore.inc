<?php

/**
 * @file
 * User interface routines for restoring MM content from a recycle bin
 */

function mm_ui_content_restore_confirm($form, &$form_state, $item, $parent, $x) {
  $form['mmtid']     = array('#type' => 'value', '#value' => $item->mmtid);
  $form['mode']      = array('#type' => 'value', '#value' => 'move');
  $form['move_mode'] = array('#type' => 'value', '#value' => 'page');
  $form['dest']      = array('#type' => 'value', '#value' => array($parent => ''));
  $form['name']      = array('#type' => 'value', '#value' => $item->name);
  $form['alias']     = array('#type' => 'value', '#value' => $item->alias);
  $msg = t('Are you sure you want to restore this @thing as a @subthing of !name?', $x);
  $msg_short = t('Are you sure you want to restore this @thing?', $x);

  return confirm_form($form, $msg_short, mm_content_get_mmtid_url($item->mmtid), $msg,
      t('Restore'), t('Cancel'));
}

function mm_ui_content_restore_confirm_validate($form, &$form_state) {
  module_load_include('inc', 'monster_menus', 'mm_ui_content_copymove');
  mm_ui_content_copymove_validate($form, $form_state, TRUE);
}

function mm_ui_content_restore_confirm_submit($form, &$form_state) {
  $error = mm_content_move_from_bin($src_mmtid = $form_state['values']['mmtid']);

  if (is_string($error)) {
    form_set_error('', t($error));
  }
  else {
    drupal_set_message(t('The @thing has been restored.', mm_ui_strings(FALSE)));
    $form_state['redirect'] = mm_content_get_mmtid_url($src_mmtid);
  }
}
