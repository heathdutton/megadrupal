<?php

/**
 * @file
 * Administration interface for User Pic Kit Minecraft.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function userpickit_minecraft_form_userpickit_settings_form_alter(&$form, &$form_state, $form_id) {
  $form['minecraft'] = array(
    '#type' => 'fieldset',
    '#title' => t('Minecraft'),
  );

  $form['minecraft']['minecraft_username'] = array(
    '#title' => t('Username'),
    '#description' => t('Minecraft username is stored in this field.'),
    '#type' => 'select',
    '#options' => array(),
  );
}