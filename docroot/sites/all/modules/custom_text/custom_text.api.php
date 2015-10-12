<?php

/**
 * @file
 * Documentation of Text hook.
 */


/**
 * Defines groups for the custom text admin form as Form API elements.
 *
 * The identifier must be a valid Form API name. 
 *
 * Do NOT set #type. DO set at least #title, and possibly #description and
 * #access.
 */
function hook_custom_text_group_info() {
  return array(
    'mymodule_admin' => t('Group for admin texts'),
    'mymodule_guest' => t('Group for guest texts'),
  );
}


/**
 * Defines unique identifiers for each text and their respective Form API
 * element.
 *
 * The identifier can be any of a-z, A-Z, 0-9, hyphen (-) or underscore (_), but
 * must not contain two consecutive hyphens (--), and be up to 64 characters.
 *
 * It must match this regular expression: /^[\w_-]{1,64}$/
 * And not match this regular expression: /--/
 *
 * It must be globally unique. If you are afraid of collisions, prefix the
 * identifier with 'mymodule_'.
 *
 * The form element's #type must be one of 'textfield', 'textarea' or
 * 'text_format'. Do not set #default_value, it will be appended automatically.
 */
function hook_custom_text_info() {
  return array(
    'mymodule_admin' => array(
      'admin_form_title' => array(
        '#type' => 'textfield',
        '#title' => t('Form Title'),
        '#description' => t('The title of the special form.'),
      ),
      'admin_form_description' => array(
        '#type' => 'textarea',
        '#title' => t('Form Description'),
        '#description' => t('This text will appear above the special form that you wrote'),
      ),
    ),
    'mymodule_guest' => array(
      'guest_form_title' => array(
        '#type' => 'textfield',
        '#title' => t('Form Title'),
        '#description' => t('The title of the special form.'),
      ),
      'guest_form_description' => array(
        '#type' => 'textarea',
        '#title' => t('Form Description'),
        '#description' => t('This text will appear above the special form that you wrote'),
      ),
      'disclaimer' => array(
        '#type' => 'text_format',
        '#title' => t('Disclaimer'),
        '#description' => t('This text will appear in the bottom of every template.'),
      ),
    ),
  );
}
