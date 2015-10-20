<?php

/**
 * @file
 * Hooks and functions used by the Variable module.
 */

/**
 * Implements hook_variable_info().
 */
function i18n_reminder_variable_info($options) {

  // Translation Reminder messages.
  $variables['i18n_reminder_msg_edit_translated'] = array(
    'title' => t('Viewing translated content', array(), $options),
    'description' => t('Shown prior to editing content that has been translated already. Both single language and translation set content gets this message.', array(), $options),
    'type' => 'string',
    'element' => array(
      '#type' => 'textarea',
    ),
    'required' => FALSE,
    'token' => FALSE,
    'localize' => TRUE,
    'default' => t("This content is translated. Please check with translation staff before making changes. Thanks!", array(), $options),
  );
  $variables['i18n_reminder_msg_editing_translated'] = array(
    'title' => t('Editing translated content', array(), $options),
    'description' => t('Shown while editing content that has been translated already. Both single language and translation set content gets this message.', array(), $options),
    'type' => 'string',
    'element' => array(
      '#type' => 'textarea',
    ),
    'required' => FALSE,
    'token' => FALSE,
    'localize' => TRUE,
    'default' => t("Hold up! This content is translated. If you're updating links or making other important changes, you should be working with translation staff. Thanks!", array(), $options),
  );
  $variables['i18n_reminder_msg_non_neutral'] = array(
    'title' => t('Untranslated non-neutral content', array(), $options),
    'description' => t('Message will be shown on content that has been marked as non-language neutral, but translations do not yet exist.', array(), $options),
    'type' => 'string',
    'element' => array(
      '#type' => 'textarea',
    ),
    'required' => FALSE,
    'token' => FALSE,
    'localize' => TRUE,
    'default' => t("This page is marked with a language but no translations exist. If you're not planning to translate it but want users of all languages to see it, please change it back to \"language neutral.\" Thanks!", array(), $options),
  );

  return $variables;
}
