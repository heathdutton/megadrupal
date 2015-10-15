<?php

/**
 * @file
 * Admin settings for Url formatter for private messages.
 */

/**
 * Admin settings form.
 */
function url_formatter_privatemsg_admin_settings($form, &$form_state) {
  $form['#tree'] = TRUE;
  $formatter_settings = url_formatter_privatemsg();
  foreach ($formatter_settings['url_formatter']['settings'] as $class => $settings) {
    if (class_exists($class)) {
      $formatter = new $class();
      $field['field_name'] = $class;
      $instance['display'][$class]['settings'] = $formatter_settings['url_formatter']['settings'];
      $form['url_formatter_privatemsg'][$class] = $formatter->form($field, $instance, $class, $form, $form_state);
    }
  }
  return system_settings_form($form);
}
