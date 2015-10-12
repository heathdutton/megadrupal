<?php
/**
 * @file
 * Provide headbar backend setting form.
 */

/**
 * Function to generate the backend form.
 */
function headbar_admin_settings() {

  $form['msg_text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#description' => t('Insert text which will appear on the bar. You may also include HTML.'),
    '#default_value' => variable_get('headbar_text', 'This text will appear on header bar. You may also include HTML.'),
  );
  
  $form['msg_color'] = array(
    '#title' => t('Color'),
    '#type' => 'textfield',
    '#description' => t('Insert the color code : example - "#FFFF00" or "red".'),
    '#default_value' => variable_get('headbar_color', '#EB593C'),
    '#element_validate' => array('_headbar_is_empty'),
  );
  
  $form['msg_color_hover'] = array(
    '#title' => t('Hover color'),
    '#type' => 'textfield',
    '#description' => t('Insert the color code for mouse hover : example - "#FFFF00" or "red".'),
    '#default_value' => variable_get('headbar_color_hover', '#EB593C'),
  );
  
  $form['msg_delaytime'] = array(
    '#title' => t('Delay Time'),
    '#type' => 'textfield',
    '#description' => t('Insert the delay time: example - "5000" = 5 sec.'),
    '#default_value' => variable_get('headbar_delaytime', '5000'),
    '#element_validate' => array('_headbar_is_numeric'),
  );
  
  $form['#submit'][] = 'headbar_admin_settings_submit';
  
  return system_settings_form($form);
}


/**
 * Validate numeric value for delaytime.
 */
function _headbar_is_numeric($element, &$form_state) {
  if (!is_numeric($element['#value'])) {
    form_set_error($element['#name'], t('Please enter delaytime in number.'));
  }
}


/**
 * Validate empty field for all the form fields.
 */
function _headbar_is_empty($element, &$form_state) {
  if (empty($element['#value'])) {
    form_set_error($element['#name'], t('Color field should not be empty.',
      array('!module' => $element['#title'])));
  }
}


/**
 * Process headbar settings and store all values in variables.
 */
function headbar_admin_settings_submit($form, $form_state) {
  $text = $form_state['values']['msg_text'];
  $color = $form_state['values']['msg_color'];
  $color_hover = $form_state['values']['msg_color_hover'];
  $delaytime = $form_state['values']['msg_delaytime'];

  variable_set('headbar_text', $text);
  variable_set('headbar_color', $color);
  variable_set('headbar_color_hover', $color_hover);
  variable_set('headbar_delaytime', $delaytime);
}
