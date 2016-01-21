<?php
// $Id$

/**
 * @file
 * Contains the administrative functions of the dynOZ module.
 *
 * This file is included by the core dynOZ module, and includes the
 * settings form.
 */

/**
 * Menu callback for the dynOZ module settings form.
 *
 * @ingroup forms
 */
function dynoz_main_settings() {
  include_once(drupal_get_path('module', 'dynoz') . '/help/dynoz.help.inc');

  $form['help'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Help and instructions'),
  );

  $form['help']['help'] = array(
    '#type'  => 'markup',
    '#value' => dynoz_help_text(),
  );

  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('dynOZ Blocks'),
  );

  $form['blocks']['dynoz_number_blocks'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of blocks'),
    '#default_value' => variable_get('dynoz_number_blocks', DYNOZ_NUMBER_BLOCKS_DEFAULT),
    '#size' => 2,
    '#maxlength' => 2,
  );
  return system_settings_form($form);
}


