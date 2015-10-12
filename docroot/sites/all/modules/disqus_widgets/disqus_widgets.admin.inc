<?php
/**
 * @file
 */

/**
 * Disqus administration form.
 *
 * @return array
 *   A form render array.
 */
function disqus_widgets_disqus_admin_view() {

  $form = array();

  $form['disqus_forum'] = array(
    '#type' => 'textfield',
    '#title' => t('Shortname'),
    '#description' => t('This is the site shortname or unique domain tied to the API secret key below.'),
    '#required' => TRUE,
    '#default_value' => variable_get('disqus_forum'),
  );

  $form['disqus_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('Your Disqus API secret key as available from <a href="http://disqus.com/api/applications/">http://disqus.com/api/applications/</a>.'),
    '#required' => TRUE,
    '#default_value' => variable_get('disqus_api_secret'),
  );

  return system_settings_form($form);
}
