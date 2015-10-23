<?php

/**
 * services_sso_server_helper admin form
 */
function services_sso_server_helper_admin_form($form, &$form_state) {
  $form['services_sso_server_helper_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Single sign-on cookie domain'),
    '#default_value' => variable_get('services_sso_server_helper_cookie_domain', _services_sso_server_helper_get_common_cookie_domain()),
    '#description' => t('The base domain to use in order to share cookies between sites with this SSO module enabled and the SSO server Drupal site. Note: you may only share a cookie on sites that share the same base domain. IE: <em>yoursite.co.uk</em> and <em>my.yoursite.co.uk</em>. In that case, you would put ".yoursite.co.uk" in this field. The leading period <strong>is important</strong>.'),
  );

  return system_settings_form($form);
}
