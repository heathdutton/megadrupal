<?php
// @codingStandardsIgnoreFile
/**
 * @file
 * Contains FieldFieldLibraryBundleUI.
 */

/**
 * Defines a ctools export ui handler for field library bundles.
 */
class FieldFieldLibraryBundleUI extends ctools_export_ui {

  /**
   * {@inheritdoc}
   */
  function build_operations($item) {
    $ops = parent::build_operations($item);
    $ops['manage_fields'] = array(
      'title' => t('Manage fields'),
      'href' => _field_ui_bundle_admin_path('field_library', $item->name) . '/fields',
    );
    $ops['manage_display'] = array(
      'title' => t('Manage display'),
      'href' => _field_ui_bundle_admin_path('field_library', $item->name) . '/display',
    );
    return $ops;
  }

  /**
   * {@inheritdoc}
   */
  function edit_save_form($form_state) {
    parent::edit_save_form($form_state);
    field_attach_create_bundle('field_library', $form_state['item']->name);
  }

  function delete_form_submit(&$form_state) {
    parent::delete_form_submit($form_state);
    field_attach_delete_bundle('field_library', $form_state['item']->name);
  }

}
