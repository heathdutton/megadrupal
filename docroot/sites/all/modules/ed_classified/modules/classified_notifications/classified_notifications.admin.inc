<?php
/**
 * @file
 * Admin page(s) for the classified module.
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * New code implementing a feature set derived from the ed_classified module:
 *
 * ---- Information about ed_classified ----
 *
 * Michael Curry, Exodus Development, Inc.
 * exodusdev@gmail.com
 * for more information, please visit:
 *   http://exodusdev.com/drupal/modules/ed_classified.module
 *
 * Copyright (c) 2006, 2007 Exodus Development, Inc.  All Rights Reserved.
 *
 * Licensed under the terms of the GNU Public License (GPL) version 2.  Please
 * see LICENSE.txt for license terms.  Possession and use of this code signifies
 * acceptance of license terms.
 *
 * ---- /ed-classified ----
 */

/**
 * Implements the former hook_settings().
 */
function classified_notifications_admin_settings($form, &$form_state) {
  $sections = array(
    'half-life' => t('Half-life'),
    'pre-expire' => t('Pre-expiration'),
    'expire' => t('Expiration'),
    'pre-purge' => t('Pre-purge'),
    'purge' => t('Purge'),
  );
  $weight = - count($sections);

  foreach ($sections as $name => $title) {
    $wrapper = $name . '-wrapper';
    $form[$wrapper] = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => $weight++,
    );

    $form[$wrapper]['classified-notifications-' . $name . '-subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => _classified_get('notifications-' . $name . '-subject'),
    );

    $form[$wrapper]['classified-notifications-' . $name . '-body'] = array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#default_value' => _classified_get('notifications-' . $name . '-body'),
    );
  }

    // Display the user documentation of placeholders supported by
    // this module, as a description on the last pattern
    // @see theme_token_tree()
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('current-date', 'site', 'user'),
      '#global_types' => FALSE,
    );

  return system_settings_form($form);
}
