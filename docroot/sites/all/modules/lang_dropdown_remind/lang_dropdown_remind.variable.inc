<?php

/**
 * @file
 * Hooks and functions used by the Variable module.
 */

/**
 * Implements hook_variable_info().
 */
function lang_dropdown_remind_variable_info($options) {
  // Language Dropdown Reminder message.
  $variables['lang_dropdown_remind_message'] = array(
    'title' => t('Reminder message', array(), $options),
    'description' => t('Reminder message shown to end-users when translated content is available in their preferred language.', array(), $options),
    'type' => 'string',
    'element' => array(
      '#type' => 'textarea',
    ),
    'required' => TRUE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => t('This page is available in your preferred language. Navigate there using <a id="trigger-langdropdown" href="#">the language drop down menu</a>.', array(), $options),
  );

  // Language dropdown Reminder close text.
  $variables['lang_dropdown_remind_close'] = array(
    'title' => t('Reminder close text', array(), $options),
    'description' => t('Text used on the button that closes out the reminder message.', array(), $options),
    'type' => 'string',
    'required' => TRUE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => '&#10006;',
  );

  // Number of times to repeat the message
  $variables['lang_dropdown_remind_repeat'] = array(
    'title' => t('Reminder limit', array(), $options),
    'description' => t('The maximum number of times an end-user will see the reminder message; a value of "0" will effectively disable this module.', array(), $options),
    'type' => 'number',
    'required' => TRUE,
    'default' => 1,
  );

  // Markup wrapping for the Language Dropdown Reminder message box.
  $variables['lang_dropdown_remind_markup'] = array(
    'title' => t('Reminder markup', array(), $options),
    'description' => t('Wrapper markup for the Language switcher reminder message. Must include !message and !close_button tokens.', array(), $options),
    'type' => 'string',
    'required' => TRUE,
    'element' => array(
      '#type' => 'textarea',
    ),
    'localize' => FALSE,
    'default' => '<div class="inner">!message !close_button</div>',
    'validate callback' => 'lang_dropdown_remind_markup_validate',
  );

  // The jQuery selector for the HTML element to which the above is prepended.
  $variables['lang_dropdown_remind_prependto'] = array(
    'title' => t('Reminder jQuery selector', array(), $options),
    'description' => t('The jQuery selector used to prepend the above markup.', array(), $options),
    'type' => 'string',
    'required' => TRUE,
    'default' => '#page',
  );

  // Boolean to determine whether or not to use the high-performance callback.
  $variables['lang_dropdown_remind_high_performance'] = array(
    'title' => t('Use high-performace callback'),
    'description' => t('Uses a high-performance method to determine user language preference; may not work on all Drupal installations.'),
    'default' => FALSE,
    'type' => 'boolean',
  );

  return $variables;
}

/**
 * Validation callback for the lang_dropdown_remind_markup variable.
 */
function lang_dropdown_remind_markup_validate($element) {
  foreach (array('message', 'close_button') as $token) {
    if (strpos($element['value'], '!' . $token) === FALSE) {
      return t('Missing reference to the !token_name token.', array(
        '!token_name' => '!' . $token,
      ));
    }
  }
}
