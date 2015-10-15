<?php
/**
 * @file
 * Administrive forms.
 */

/**
 * Returns global settings form.
 *
 * @see pgn4web_global_settings_form_submit()
 * @ingroup forms
 */
function pgn4web_global_settings_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'pgn4web') . '/pgn4web.admin.css');

  $values = isset($form_state['values']) ? $form_state['values']['pgn4web'] : pgn4web_get_global_config();

  // Various layout settings.
  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout'),
    '#collapsible' => TRUE,

    // horizontalLayout
    'hl' => array(
      '#type' => 'radios',
      '#title' => t('Layout'),
      '#options' => array(
        't' => t('Use horizontal layout'),
        'f' => t('Use vertical layout'),
      ),
      '#default_value' => $values['hl'],
    ),
/*
    // buttonDisplay
    'bd' => array(
      '#type' => 'radios',
      '#title' => t('Buttons'),
      '#options' => array(
        'h' => t('Do not display'),
        's' => t('Use browser\'s standard buttons'),
        'c' => t('Use custom buttons'),
      ),
      '#default_value' => $values['bd'],
    ),
*/
/*
    // headerDisplay
    'hd' => array(
      '#type' => 'radios',
      '#title' => t('Header'),
      '#options' => array(
        'h' => t('Do not display'),
        'j' => t('Display justified'),
        'c' => t('Display centered'),
      ),
      '#default_value' => $values['hd'],
    ),
*/
  );

  // Autoplay settings.
  $form['autoplay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autoplay'),
    '#collapsible' => TRUE,

    // autoplayMode
    'am' => array(
      '#type' => 'radios',
      '#title' => t('Autoplay mode'),
      '#options' => array(
        'g' => t('Autoplay the inital game only'),
        'l' => t('Autoplay all games in a loop'),
        'n' => t('Do not autoplay games'),
      ),
      '#default_value' => $values['am'],
    ),

    // delay
    'd' => array(
      '#type' => 'textfield',
      '#title' => t('Delay'),
      '#maxlength' => '5',
      '#size' => '5',
      '#default_value' => $values['d'],
      '#field_suffix' => 'ms',
      '#element_validate' => array('element_validate_integer_positive'),
    ),
  );

  // Colors and backgrounds.
  $form['colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colors and backgrounds'),
    '#collapsible' => TRUE,
  );

  $colors = array(
    'lch'  => t('Light squares color'),
    'dch'  => t('Dark squares color'),
    'bbch' => t('Board border color'),
    'hch'  => t('Square highlight color'),

    'cbch' => t('Buttons background color'),
    'ctch' => t('Buttons text color'),

    'fhch' => t('Header text color'),
    'fmch' => t('Moves text color'),
    'hmch' => t('Move highlight color'),
    'fcch' => t('Comments text color'),
  );

  foreach ($colors as $key => $title) {
    $form['colors'][$key] = array(
      '#type' => 'textfield',
      '#title' => $title,
      '#maxlength' => '7',
      '#size' => '7',
      '#default_value' => $values[$key],
      '#field_prefix' => '#',
      '#element_validate' => array('pgn4web_color_validate'),
    );
  }

  // We need to move all settings into one variable, so we can't use
  // system_settings_form($form) here.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ),
  );

  // ... but theme it as a system_settings_form anyway ...
  $form['#theme'] = 'system_settings_form';

  return $form;
}

/**
 * Execute global settings form.
 *
 * @see pgn4web_global_settings_form().
 */
function pgn4web_global_settings_form_submit($form, &$form_state) {
  // Only set recognised parameters.
  $default_config = pgn4web_get_global_config(TRUE);
  $config = array_intersect_key($form_state['values'], $default_config);

  variable_set('pgn4web-global', $config);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Element validate handler to ensure a hexadecimal color value.
 *
 * @see image_effect_color_validate().
 */
function pgn4web_color_validate($element, &$form_state) {
  if ($element['#value'] != '') {
    if (!preg_match('/^#?[0-9A-F]{3}([0-9A-F]{3})?$/i', $element['#value'])) {
      form_error($element, t('!name must be a hexadecimal color value.', array('!name' => $element['#title'])));
    }
    // TODO: can we change the value here to be:
    // - uppercase,
    // - without '#'
    // - always 6 hexadecimals?
  }
}
