<?php

/**
 * @file
 * Integration with variable.module.
 */


/**
 * Implements hook_variable_info().
 */
function site_disclaimer_variable_info($options) {
  $variables['site_disclaimer_node_title'] = array(
    'title' => t('Node title'),
    'type' => 'string',
    'default_value' => '',
    'description' => t('Enter the <em>title</em> of an existing node. Leave empty and also use no token in the [x] checkbox label below to disable Site Disclaimer.'),
    'access' => 'administer site configuration',
    'token' => TRUE, // We'll produce tokens automatically for this one
    'group' => 'site_disclaimer',
    'localize' => FALSE,
  );
  $variables['site_disclaimer_node_id'] = array(
    'title' => t('Node id'),
    'description' => t('Enter the <em>id</em> of an existing node. Leave empty and also use no token in the [x] checkbox label below to disable Site Disclaimer.'),
    'type' => 'number',
    'default_value' => '',
    'access' => 'administer site configuration',
    'token' => TRUE, // We'll produce tokens automatically for this one
    'group' => 'site_disclaimer',
  );

  $variables['site_disclaimer_title'] = array(
    'title' => t('Title'),
    'type' => 'string',
    'default_value' => SITE_DISCLAIMER_DEFAULT_TITLE,
    'description' => t('Enter a title to put on the Site Disclaimer form.'),
    'group' => 'site_disclaimer',
    'localize' => TRUE,
  );
  $variables['site_disclaimer_fieldset'] = array(
    'title' => t('Enclose in a fieldset'),
    'type' => 'boolean',
    'default_value' => 1,
    'description' => t('Enclose the Site Disclaimer form and the [x] checkbox in a fieldset. Helps visually group the form as typically themes display fieldset in a box / add borders.'),
    'group' => 'site_disclaimer',
  );

  $variables['site_disclaimer_checkbox_label'] = array(
    'title' => t('[x] checkbox label'),
    'type' => 'string',
//?    'maxlength' => 256, // Increase max length to allow lengthier fields (Site Disclaimer can be wordy)
    'default_value' => SITE_DISCLAIMER_DEFAULT_CHECKBOX_LABEL,
    'description' => t('Enter a Site Disclaimer statement that visitors have to accept'),
    'group' => 'site_disclaimer',
    'localize' => TRUE,
  );
  $variables['site_disclaimer_node_height'] = array(
    'title' => t('Max height'),
    'type' => 'string',
    'default_value' => '',
    'description' => t('Leave empty to show Site Disclaimer form with text in full height. Enter something like <b>20em</b> or <b>120px</b> to set the height of the Site Disclaimer form. Vertical scrollbar is added automatically if text height exceeds the max height setting.'),
    'group' => 'site_disclaimer',
    'localize' => FALSE,
  );

  $variables['site_disclaimer_version_details'] = array(
    'type' => 'string', // @todo: what type corresponds to textarea?
    'title' => t('Changes'),
    'description' => t('Describe changes in the latest version. Note: if you are not changing version, this description will be shown to all users who have not accepted the current version.'),
    'default_value' => '',
    'group' => 'site_disclaimer',
    'localize' => TRUE,
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function site_disclaimer_variable_group_info() {
  $groups['site_disclaimer'] = array(
    'title' => t('Site disclaimer'),
    'description' => t('Site disclaimer'),
    'access' => 'administer site configuration',
    'path' => 'admin/config/people/site_disclaimer',
  );
  return $groups;
}

