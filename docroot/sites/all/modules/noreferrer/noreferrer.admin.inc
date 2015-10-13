<?php

/**
 * Admin settings form.
 */
function noreferrer_admin() {
  $form['noreferrer_whitelisted_domains'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Whitelisted domains'),
    '#default_value' => variable_get('noreferrer_whitelisted_domains', ''),
    '#description'   => t('Enter a space-separated list of domains to which referrer URLs will be sent (e.g. <em>example.com example.org</em>). Links to all other domains will have a <code>rel="noreferrer"</code> attribute added.'),
  );
  return system_settings_form($form);
}
