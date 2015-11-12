<?php

/**
 * @file
 * Documents API functions for Virtual Keyboard module.
 */

/**
 * Alter the options used by Virtual Keyboard plugin.
 * @see https://github.com/Mottie/Keyboard/wiki/Options
 *
 * @param array $options
 *   Options used by Virtual Keyboard jQuery plugin.
 */
function hook_virtual_keyboard_options_alter(&$options) {
  // Use custom layout instead of default qwerty layout.
  $options['layout'] = 'custom';
  $options['customLayout'] = array(
    'default' => array(
      'C D E F',
      '8 9 A B',
      '4 5 6 7',
      '0 1 2 3',
      '{bksp} {a} {c}'
    ),
  );
}

/**
 * Registers layouts to be used with Virtual Keyboard.
 *
 * @return array
 */
function hook_virtual_keyboard_layout_info() {
  // Add numeric keyboard layout.
  $layouts = array();
  $layouts['num'] = array(
    'default' => array(
      '= ( ) {b}',
      '{clear} / * -',
      '7 8 9 +',
      '4 5 6 {sign}',
      '1 2 3 %',
      '0 . {a} {c}'
    ),
  );
  return $layouts;
}

/**
 * Alters layouts used by Virtual Keyboard.
 *
 * @param $layouts
 */
function hook_virtual_keyboard_layout_info_alter(&$layouts) {
  // Remove row with numbers from qwerty layout.
  if (isset($layouts['qwerty'])) {
    $layouts['qwerty'] = array(
      'default' => array(
        // '` 1 2 3 4 5 6 7 8 9 0 - = {bksp}',
        '{tab} q w e r t y u i o p [ ] \\',
        'a s d f g h j k l ; \' {enter}',
        '{shift} z x c v b n m , . / {shift}',
        '{accept} {space} {cancel}'
      ),
      'shift' => array(
        // '~ ! @ # $ % ^ & * ( ) _ + {bksp}',
        '{tab} Q W E R T Y U I O P { } |',
        'A S D F G H J K L : " {enter}',
        '{shift} Z X C V B N M < > ? {shift}',
        '{accept} {space} {cancel}'
      )
    );
  }
}
