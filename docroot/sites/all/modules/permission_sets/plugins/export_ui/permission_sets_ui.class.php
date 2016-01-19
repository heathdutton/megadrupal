<?php

/**
 * @file
 * A custom Ctools Export UI class for Permission Sets.
 */

/**
 * Customizations of the Permission Sets UI.
 */
class permission_sets_ui extends ctools_export_ui {

  /**
   * Build a row based on the item.
   */
  function list_build_row($item, &$form_state, $operations) {
    parent::list_build_row($item, $form_state, $operations);
    $this->rows[$item->machinename]['data'][0]['data'] = check_plain($item->name);
  }

}
