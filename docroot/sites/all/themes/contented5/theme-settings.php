<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function contented5_form_system_theme_settings_alter(&$form, &$form_state)  {
  $form['themedev']['zen_layout'] = array(
    '#type'          => 'radios',
    '#title'         => t('Layout method'),
    '#options'       => array(
                          'zen-columns-liquid' => t('Liquid layout') . ' <small>(layout-liquid.css)</small>',
                          'zen-columns-fixed' => t('Fixed layout') . ' <small>(layout-fixed.css)</small>',
                        ),
    '#default_value' => theme_get_setting('zen_layout'),
  );
}
