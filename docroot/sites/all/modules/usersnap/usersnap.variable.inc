<?php

/**
 * @file
 * Variable API hooks and functions for the Usersnap module.
 */

/**
 * Implements hook_variable_group_info().
 */
function usersnap_variable_group_info() {
  $groups['usersnap'] = array(
    'title' => t('Usersnap'),
    'description' => t('Usersnap settings and configuration'),
    'access' => 'administer site configuration',
    'path' => array('admin/config/services/usersnap'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function usersnap_variable_info($options) {
  require_once(drupal_get_path('module', 'usersnap') . '/usersnap.admin.inc');

  // API Key.
  $variables['usersnap_apikey'] = array(
    'title' => t('Usersnap API Key', array(), $options),
    'description' => t('Enter the Usersnap API Key. You can get a Key at http://www.usersnap.com.', array(), $options),
    'type' => 'string',
    'default' => '',
    'required' => TRUE,
    'group' => 'usersnap',
  );

  // Widget button text.
  $variables['usersnap_button_text'] = array(
    'title' => t('Widget button text', array(), $options),
    'description' => t('Button text displayed on the Usersnap feedback widget.', array(), $options),
    'type' => 'string',
    'default' => 'Feedback',
    'localize' => TRUE,
    'group' => 'usersnap',
  );

  // Widget visibility.
  $variables['usersnap_visibility'] = array(
    'title' => t('Show widget on specific pages', array(), $options),
    'type' => 'select',
    'default' => USERSNAP_VISIBILITY_NOTLISTED,
    'options' => array(
      USERSNAP_VISIBILITY_NOTLISTED => t('All pages except those listed', array(), $options),
      USERSNAP_VISIBILITY_LISTED => t('Only the listed pages', array(), $options),
    ),
    'group' => 'usersnap',
  );

  // Widget paths/pages.
  $variables['usersnap_paths'] = array(
    'title' => t('Widget pages', array(), $options),
    'description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    ), $options),
    'type' => 'text',
    'default' => '',
    'group' => 'usersnap',
  );

  // Widget position.
  $variables['usersnap_position'] = array(
    'title' => t('Widget position', array(), $options),
    'description' => t('The feedback widget will be placed in this part of the screen.', array(), $options),
    'type' => 'select',
    'default' => 'bottom_right',
    'options' => array(
      'bottom_right' => t('Bottom right corner'),
      'bottom_left' => t('Bottom left corner'),
      'middle_right' => t('Middle right'),
      'middle_left' => t('Middle left'),
    ),
    'group' => 'usersnap',
  );

  // Widget interface language.
  $variables['usersnap_language'] = array(
    'title' => t('Widget language', array(), $options),
    'description' => t('Preferred interface language for feedback widget.', array(), $options),
    'type' => 'select',
    'default' => 'en',
    'options' => usersnap_supported_languages(),
    'localize' => TRUE,
    'group' => 'usersnap',
  );

  // Show e-mail field.
  $variables['usersnap_show_email_field'] = array(
    'title' => t('Show e-mail field', array(), $options),
    'type' => 'boolean',
    'default' => FALSE,
    'group' => 'usersnap',
  );

  // E-mail placeholder text.
  $variables['usersnap_email_field_placeholder'] = array(
    'title' => t('E-mail field placeholder text', array(), $options),
    'description' => t('Placeholder text shown to users in the e-mail field', array(), $options),
    'type' => 'string',
    'default' => '',
    'localize' => TRUE,
    'group' => 'usersnap',
  );

  // Make e-mail required.
  $variables['usersnap_email_field_required'] = array(
    'title' => t('Require e-mail', array(), $options),
    'type' => 'boolean',
    'default' => FALSE,
    'group' => 'usersnap',
  );

  // Show comment box.
  $variables['usersnap_show_comment_box'] = array(
    'title' => t('Show comment box', array(), $options),
    'type' => 'boolean',
    'default' => FALSE,
    'group' => 'usersnap',
  );

  // Comment box placeholder text.
  $variables['usersnap_comment_box_placeholder'] = array(
    'title' => t('Comment box placeholder text', array(), $options),
    'description' => t('Text shown to users in the comment box', array(), $options),
    'type' => 'string',
    'default' => '',
    'localize' => TRUE,
    'group' => 'usersnap',
  );

  // Make comment box required.
  $variables['usersnap_comment_box_required'] = array(
    'title' => t('Require comment', array(), $options),
    'type' => 'boolean',
    'default' => FALSE,
    'group' => 'usersnap',
  );

  return $variables;
}
