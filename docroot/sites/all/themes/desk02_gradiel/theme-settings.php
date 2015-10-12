<?php
// $Id: theme-settings.php,v 1.1 2011/02/04 14:35:58 jax Exp $

/**
 * @file
 * Theme setting callbacks for the Desk02 Gradiel theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function desk02_gradiel_form_system_theme_settings_alter(&$form, $form_state) {
  $form['top_region'] = array(
    '#type'          => 'fieldset',
    '#title'         => 'Front page top region',
    '#description'   => t('Change the display variables of the <em>Top</em> region on the front page.'),
  );
  $form['top_region']['top_height'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Top region height'),
    '#default_value' => theme_get_setting('top_height'),
    '#field_suffix'  => 'px',
    '#description'   => t('Change the height of the top region on the front page.'),
  );
  $form['top_region']['top_overflow'] = array(
    '#type'          => 'select',
    '#title'         => t('Top region overflow'),
    '#default_value' => theme_get_setting('top_overflow'),
    '#options'       => array(
                          'auto' => 'auto',
                          'hidden' => 'hidden',
                          'scroll' => 'scroll',
                          'visible' => 'visible',
                        ),
    '#description'   => t('Change the type of overflow for the blocks in the top region.'),
  );
}