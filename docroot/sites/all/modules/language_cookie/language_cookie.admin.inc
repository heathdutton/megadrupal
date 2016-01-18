<?php
/**
 * @file
 * Language Cookie admin settings form.
 */

/**
 * The URL language provider configuration form.
 */
function language_cookie_admin_form($form, &$form_state) {
  $form['language_cookie_param'] = array(
    '#title' => t('Cookie parameter'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_param', 'language'),
    '#description' => t('Name of the cookie parameter used to determine the desired language.'),
  );

  $form['language_cookie_time'] = array(
    '#title' => t('Cookie duration'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_time', 31536000),
    '#description' => t('The time the cookie expires. This is the number of seconds from the current time.'),
  );

  $form['language_cookie_path'] = array(
    '#title' => t('Cookie path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_path', '/'),
    '#description' => t('The cookie available server path'),
  );

  $form['language_cookie_domain'] = array(
    '#title' => t('Cookie domain scope'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_domain', ''),
    '#description' => t('The cookie domain scope'),
  );

  $form['language_cookie_set_on_every_pageload'] = array(
    '#title' => t('Re-send cookie on every page load'),
    '#type' => 'checkbox',
    '#description' => t('This will re-send a cookie on every page load, even if a cookie has already been set. This may be useful if you use a page cache such as Varnish and you plan to cache the language cookie. This prevents a user who already has a cookie visiting an uncached page and the cached version not setting a cookie.'),
    '#default_value' => variable_get('language_cookie_set_on_every_pageload', FALSE),
  );

  $language_selection = variable_get('language_selection_page_path', 'language_selection');
  $form['language_cookie_blacklisted_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths blacklist'),
    '#default_value' => variable_get('language_cookie_blacklisted_paths', $language_selection),
    '#size' => 10,
    '#description' => t('Specify on which paths the language selection pages should be circumvented.') . '<br />'
    . t("Specify pages by using their aliased paths. Enter one path per line. The '*' character is a wildcard."),
  );


  $form_state['redirect'] = 'admin/config/regional/language/configure';
  return system_settings_form($form);
}
