<?php

/**
 * @file
 * Hover Card module admin inc file.
 *
 * Contains admin configuration page callback functions.
 */

/**
 * Form builder; Configure hover card settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function hover_card_settings() {
  $form['personalization'] = array(
    '#type' => 'fieldset',
    '#title' => t('Personalization'),
  );
  $form['personalization']['hover_card_user_email_display_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable User Emails on Hover'),
    '#default_value' => variable_get('hover_card_user_email_display_status', FALSE),
  );

  return system_settings_form($form);
}
