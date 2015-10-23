<?PHP

/**
 * @file
 * Handles the admin page for Speed_form_blocks module.
 */

/**
 * Does the display of a basic page to explain how to use this module.
 */
function logic_block_admin() {

  $form['speed_form_blocks_main'] = array(
    '#markup' => '<h3>' . t('Logic Block management') . '</h3><h4>' .
    t('This module allows administrators to display blocks dependent on the content of a block') . '</h4><p>'
    . t('To create a block visit') . l(t(" Block Creation"), "admin/structure/logic_block/create")
    . '</p><p>' . t('To edit / duplicate / delete blocks you have created visit')
    . l(t(" Manage"), "admin/structure/logic_block/manage"),
  );

  return $form;

}
