<?php

/**
 * @file
 * Absolute Messages module admin functions.
 */

/**
 * Implements hook_admin_settings().
 */
function notification_bar_admin_settings_form() {
  $form = array();

  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
  );
  $form['message']['notification_bar_message_text_left'] = array(
    '#type' => 'textarea',
    '#title' => t('Left Section'),
    '#default_value' => variable_get('notification_bar_message_text_left', ''),
    '#weight' => -2,
  );
  $notification_bar_message_array = variable_get('notification_bar_message');
  $form['message']['notification_bar_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Message'),
    '#default_value' => isset($notification_bar_message_array['value']) ? $notification_bar_message_array['value'] : '',
    '#format' => isset($notification_bar_message_array['format']) ? $notification_bar_message_array['format'] : '',
    '#weight' => -1,
  );
  $form['message']['cta'] = array(
    '#type' => 'fieldset',
    '#title' => t('CTA'),
    '#description' => 'Call to Action',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
  );
  $form['message']['cta']['notification_bar_message_cta_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => variable_get('notification_bar_message_cta_text', ''),
  );
  $form['message']['cta']['notification_bar_message_cta_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => variable_get('notification_bar_message_cta_url', ''),
  );

  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification Visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'visibility',
    '#weight' => 0,
  );
  $options = array(
    BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
  $form['visibility']['path']['notification_bar_page_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show notification on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('notification_bar_page_visibility', BLOCK_VISIBILITY_NOTLISTED),
  );
  $form['visibility']['path']['notification_bar_page_custom'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . t('Pages') . '</span>',
    '#default_value' => variable_get('notification_bar_page_custom', ''),
    '#description' => $description,
  );

  return system_settings_form($form);
}
