<?PHP

/**
 * @file
 * Handles the admin page for Speed_form_blocks module.
 */

/**
 * Does the display of a basic page to explain how to use this module.
 */
function speed_form_blocks_admin() {

  $form['speed_form_blocks_main'] = array(
    '#markup' => '<h3>' . t('Speed form block management') . '</h3><h4>' .
    t('This module allows people to create simple blocks to present custom forms to an end user') . '</h4><p>'
    . t('To create a block visit') . l(t(" Block Creation"), "admin/structure/speed_form_blocks/creation")
    . '</p><p>' . t('To manage / delete blocks you have created visit')
    . l(t(" Manage / Delete"), "admin/structure/speed_form_blocks/management")
    . '</p><p>' . t('To see feedback / delete feedback received visit')
    . l(t(" responses"), "admin/structure/speed_form_blocks/responses") . '</p><p>'
    . t('To export feedback visit')
    . l(t(' data export'), "admin/structure/speed_form_blocks/export") . '</p>',
  );

  return $form;

}
