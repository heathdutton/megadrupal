<?php

/**
 * @file
 * Hide Site module administration
 */

/**
 * Module configuration page callback
 */
function hide_site_config() {
  return render(drupal_get_form('hide_site_config_form'));
}

/**
 * Module configuration form
 */
function hide_site_config_form($form, &$form_state) {

  $form['hide_site_enabled'] = array(
    '#type' => 'radios',
    '#title' => t("Email Hide Site"),
    '#description' => t("Enable the Hide Site module functionality. This can globally switch on or off the functionality. This can also be overriden by setting the key hide_site_enabled variable in the variable table."),
    '#options' => array(
      1 => t("Enabled"),
      0 => t("Disabled"),
    ),
    '#default_value' => variable_get('hide_site_enabled', 0),
  );

  $form['hide_site_whitelist'] = array(
    '#type' => 'textarea',
    '#title' => t("Domain whitelist"),
    '#description' => t("Optionaly list host domains where the Hide Site settings should be ignored. One domain per line. Don't include the protocol (e.g. http://)"),
    '#default_value' => variable_get('hide_site_whitelist', ''),
  );
  
  $form['hide_site_exclude_paths'] = array(
    '#type' => 'textarea',
    '#title' => t("Exclude paths"),
    '#description' => t("Optionaly exclude paths. Don't include the protocol or site name- just the path with no leading slash. This is useful if you have web services URLs or page pages that shouldn't be protected by the hide site login."),
    '#default_value' => variable_get('hide_site_exclude_paths', ''),
  );

  $form['hide_site_message'] = array(
    '#type' => 'textfield',
    '#title' => t("Hidden message"),
    '#description' => t("This message is displayed as the page title when prompting for access credentials."),
    '#default_value' => variable_get('hide_site_message', t("This site is currently hidden")),
  );

  $form['hide_site_credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t("Set site wide access credentials"),
    '#description' => t("If current host is not in the whitelist, then these credentials will be prompted for before allowing access to the site."),
  );

  $form['hide_site_credentials']['hide_site_user'] = array(
    '#type' => 'textfield',
    '#title' => t("Username"),
    '#description' => t("Specify a username"),
    '#default_value' => variable_get('hide_site_user', ''),
  );

  $form['hide_site_credentials']['hide_site_pass'] = array(
    '#type' => 'textfield',
    '#title' => t("Password"),
    '#description' => t("Specify a password. Warning: this is stored as a plain text password. It's strongly advised NOT to use the same password as a user account for security reasons. The Hide Site module doesn't consider that this password needs to be particularly strong as it's main purpose is to prevent crawlers and wandering vagrants."),
    '#default_value' => variable_get('hide_site_pass', ''),
  );

  return system_settings_form($form);

}
