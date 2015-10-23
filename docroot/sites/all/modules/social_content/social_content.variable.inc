<?php
/**
 * @file
 * Social content variable file
 */

/**
 * Implements hook_variable_group_info().
 */
function social_content_variable_group_info() {
  $groups['social_content'] = array(
    'title' => t('Social Content'),
    'description' => t('Social Content settings'),
    'access' => 'administer site configuration',
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function social_content_variable_info($options) {
  $admin_link = l(t('!settings_form settings form', array('!settings_form' => 'Social content')), 'admin/config/services/social-content/global');
  $variable[SocialContent::getFormRootElementKey()] = array(
    'title' => t('Social Content'),
    'type' => 'array',
    'group' => 'social_content',
    'localize' => TRUE,
    'description' => t('Please configure these settings through the !settings_form.', array('!settings_form' => $admin_link)),
  );

  $classes = social_content_get_classes();
  foreach ($classes as $class) {
    $key = $class::getFormRootElementKey();
    if (!isset($variable[$key])) {
      $class = new $class();
      // TODO: Apparently it is possible to pass tokens into the variable name
      // (see variable.api.php hook_variable_info())
      // But I was unable to get this working.
      // NOTE: Using type => array means that all variables
      // get type => 'textfield'.  If we ever want to use different types
      // we will need to declare each variable separatley.
      $admin_link = l(t('!settings_form settings form', array('!settings_form' => $class->getLabel())), 'admin/config/services/social-content/global');
      $variable['social_content_' . $key] = array(
        'title' => t('Social Content: !title', array('!title' => $class->getLabel())),
        'type' => 'array',
        'group' => 'social_content',
        'localize' => TRUE,
        'description' => t('Please configure these settings through the !settings_form.', array('!settings_form' => $admin_link)),
      );
    }
  }
  return $variable;
}
