<?php
/**
 * @file
 * Hooks of odir_acl module.
 */

/**
 * This hook defines additional form elements for an odir_acl block.
 * See also: hook_odir_acl_block_save
 *
 * Used primarly by odir_field.
 */
function hook_odir_acl_block_configure($delta) {
  switch ($delta) {
    case 0:
      $options = array();
      $rows = odir_field_load_fields('odir_field');
      if ($rows) {
        foreach ($rows as $r) {
          $options[$r->field_name] = $r->field_name;
        }
        $form['odir_block_fieldname'] = array(
          '#type' => 'select',
          '#title' => t('Associate a field'),
          '#size' => 1,
          '#options' => $options,
          '#default_value' => variable_get('odir_block_fieldname', $rows[0]->field_name),
        );
      }
      break;
  }
}

/**
 * This hook implemts the hook_block_save part of an odir_acl block.
 * See also: hook_odir_acl_block_save
 *
 * Used primarly by odir_field.
 */
function hook_odir_acl_block_save($delta, $edit) {
  switch ($delta) {
    case 0:
      variable_set('odir_block_fieldname', $edit['odir_block_fieldname']);
      break;
  }
}
