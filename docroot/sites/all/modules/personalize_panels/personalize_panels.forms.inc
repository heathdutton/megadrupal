<?php
/**
 *  @file
 *  Form callbacks for the Personalize Panels module.
 */

/**
 * Deletion of a Personalized Panel.
 */
function personalize_panels_variation_set_delete($form, $form_state, $ppanel) {
  $form['osid'] = array('#type' => 'hidden', '#value' => $ppanel->osid);
  $form['title'] = array('#type' => 'hidden', '#value' => $ppanel->label);
  $form['page_did'] = array('#type' => 'hidden', '#value' => $ppanel->data['page_did']);
  return confirm_form($form, t('Are you sure you want to delete the personalized panel %title?', array('%title' => $ppanel->label)), 'admin/structure/personalize/variations/personalize-panels', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for Personalized Panel deletion.
 */
function personalize_panels_variation_set_delete_submit($form, &$form_state) {
  $display = panels_load_display($form_state['values']['page_did']);
  foreach ($display->content as $pid => $pane) {
    if (isset($pane->configuration['osid']) && $pane->configuration['osid'] == $form_state['values']['osid']) {
      unset($display->content[$pid]);
      break;
    }
  }
  panels_save_display($display);
  personalize_option_set_delete($form_state['values']['osid']);
  drupal_set_message(t('The personalize panel %name has been removed.', array('%name' => $form_state['values']['title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/personalize/variations/personalize-panels';
}
