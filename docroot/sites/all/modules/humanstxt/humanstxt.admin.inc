<?php

/**
 * @file
 * Admin file for humans txt.
 */

/**
 * Settings for humans.txt file.
 */
function humanstxt_admin_settings() {
  $form['about'] = array(
    '#markup' => t('Add here the information about the different people who have contributed to building the website, you can find more info in <a href="@humanstxt">humanstxt.org</a> and use <a href="http://humanstxt.org/humans.txt">this file</a> as base file.', array('@humanstxt' => 'http://humanstxt.org')),
  );

  $default_value = _humanstxt_get_content();
  if (empty($default_value)) {
    $default_value = <<<VALUE
/* TEAM */

/* SITE */
Tools:Drupal
VALUE;
  }

  $form['humanstxt'] = array(
    '#type' => 'textarea',
    '#title' => t('Contents of humans.txt'),
    '#default_value' => $default_value,
    '#cols' => 60,
    '#rows' => 20,
    '#wysiwyg' => FALSE,
  );
  $form['humanstxt_display_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display humans.txt in head section as a link.'),
    '#description' => t('Activating this setting will make humans.txt file to be linked in the head section of the html code'),
    '#default_value' => variable_get('humanstxt_display_link', FALSE),
  );

  return system_settings_form($form, FALSE);
}
