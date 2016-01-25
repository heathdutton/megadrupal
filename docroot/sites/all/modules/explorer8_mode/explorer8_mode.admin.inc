<?php
// $Id: explorer8_mode.admin.inc,v 1.1.4.1 2010/05/06 13:14:19 quiptime Exp $

/**
 * @file
 * The module admin settings.
 */

/**
 * Menu callback
 *
 * @see explorer8_mode_menu()
 */
function explorer8_mode_settings() {
  $form['mode'] = array(
    '#type' => 'fieldset',
    '#title' => t('Document compatibility mode'),
    '#description' => t('Please choose the Explorer version for the document compatibility mode.<br />If "IE 8" is used then there is no entry of an additional HTML tag in the header.')
  );
  $form['mode']['explorer8_mode_setting'] = array(
    '#type' => 'radios',
    '#title' => t('Mode'),
    '#options' => array(
      'IE=5' => t('<strong>IE 5</strong><br />The code for the page is like the Quirks mode in Internet Explorer 7 is displayed.'),
      'IE=7' => t('<strong>IE 7</strong><br />The code of the page as if by the standards mode in Internet Explorer 7 is displayed, without taking into account the/a DOCTYPE statement.'),
      'IE=8' => t('<strong>IE 8</strong><br />The IE8- or IE8-standards mode. This is the default for Internet Explorer 8.'),
      'IE=EmulateIE7' => t('<strong>Emulate IE 7</strong><br />The code of the page like Internet Explorer 7 appears, however, considering the/a DOCTYPE specification (the standard mode, as displayed by IE 7 and Quirks mode, as indicated by the IE 5).'),
      'IE=EmulateIE8' => t('<strong>Emulate IE 8</strong><br />Does the website a standard DOCTYPE indication, it appears in IE8 mode, the other in Quirks mode (IE5 mode) are shown.'),
    ),
    '#default_value' => variable_get('explorer8_mode_setting', 'IE=8'),
  );
  return system_settings_form($form);
}
