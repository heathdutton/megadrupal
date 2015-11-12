<?php

/*
 * implements hook_apps_app_info
 */
function lingotek_app_apps_app_info() {
  return array(
    'configure form' => 'lingotek_app_configure_form',
  );
}

/*
 * configure form for lingotek app
 */
function lingotek_app_configure_form($form, $form_state) {
  $form['setup'] = array(
    '#type' => 'fieldset',
    '#title' => 'Lingotek Initial Configuration',
  );

  //$setup_complete = variable_get('lingotek_setup_complete', FALSE);
  $setup_complete = (lingotek_is_config_missing() === FALSE);

  $form['setup']['table'] = array(
    '#theme' => 'table',
    '#header' => array("Status", "Step", "Instructions"),// "Action"),
    '#rows' => array(
      'config' => array(
        'data' => array(
          theme("image", array("path" => $setup_complete ? "misc/watchdog-ok.png" : "misc/watchdog-warning.png", "alt" => "OK")),
          t("Lingotek Configuration"),
          t("Your Lingotek account has @been configured. !setup", array("@been" => $setup_complete ? "been" : "not yet been", "!setup" => !$setup_complete ? t("Please go through the !initial_setup", array("!initial_setup" => l("Initial Setup.", 'admin/config/lingotek/new-account', array('query' => drupal_get_destination())))) : "")),
        ),
      ),
    ),
  );

  return $form;
}
