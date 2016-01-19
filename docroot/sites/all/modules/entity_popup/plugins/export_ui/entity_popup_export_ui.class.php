<?php
/**
 * @file
 * Contains entity_popup_export_ui.
 */

/**
 * Export UI class.
 */
class entity_popup_export_ui extends ctools_export_ui {

  public function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    $entity_info = entity_get_info();
    $entity_types = array('' => t('- Select -'));
    foreach ($entity_info as $entity_type => $info) {
      $entity_types[$entity_type] = $info['label'];
    }
    $form['top row']['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#options' => $entity_types,
      '#default_value' => '',
    );
  }

  public function list_filter($form_state, $item) {
    $result = parent::list_filter($form_state, $item);
    if ($form_state['values']['entity_type'] && $item->entity_type != $form_state['values']['entity_type']) {
      return TRUE;
    }
    return $result;
  }
}