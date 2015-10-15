<?php
/**
 * @file
 * This file is used by Ctools to build a plugin.
 * It's automatically executed by Ctools via the path naming convention.
 * Ctools will expect a "$plugin" Array variable to be filled with the required
 * keys to describe a plugin. This file should also contain the needed callback 
 * functions for:
 * - [plugin name]_content_type_render
 * - [plugin name]_content_type_edit_form
 * - [plugin name]_content_type_edit_form_submit
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = PasswordRecoveryPlugin::generateCtoolsPluginInfo();

/**
 * Run-time rendering of the body of the block.
 *
 * @param string $subtype
 *   Ctools subtype
 * @param array $conf
 *   Configuration as done at admin time.
 * @param array $args
 *   Ctools args
 * @param array $context
 *   Context - in this case we don't have any.
 *
 * @return array
 *   An object with at least title and content members.
 */
function password_recovery_widget_content_type_render($subtype, $conf, $args, $context) {
  return PasswordRecoveryPlugin::renderBlock($subtype, $conf, $args, $context);
}

/**
 * The 'Edit form' callback for the content type.
 * 
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form state.
 *
 * @return array
 *   Definition array to describe the custom plugin.
 */
function password_recovery_widget_content_type_edit_form($form, &$form_state) {
  return PasswordRecoveryPlugin::editForm($form, $form_state);
}

/**
 * The 'Edit form' submission callback.
 * 
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form state.
 */
function password_recovery_widget_content_type_edit_form_submit($form, &$form_state) {
  PasswordRecoveryPlugin::editFormSubmit($form, $form_state);
}
