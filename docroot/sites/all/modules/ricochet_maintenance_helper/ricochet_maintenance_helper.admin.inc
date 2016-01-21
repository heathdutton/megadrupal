<?php

/**
 * Helper settings form.
 */
function ricochet_maintenance_helper_settings_form() {
  if (variable_get('ricochet_maintenance_helper_listen', FALSE)) {
    $link = l(
      'Maintenance Server',
      variable_get('ricochet_maintenance_helper_target_address', RMH_ENV_URL),
      array('external' => TRUE));
    $msg = t('The Ricochet Maintenance Helper module is listening for arriving API key from the Maintenance
    server.<br>
    Go to the !link and add/configure your site, then click Complete
    Setup', array('!link' => $link));
    drupal_set_message($msg, 'warning', FALSE);
  }
  $form['ricochet_maintenance_helper_send'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable sending update reports'),
    '#description' => t('Check this to enable sending information about available updates to the Ricochet Maintenance server.'),
    '#default_value' => variable_get('ricochet_maintenance_helper_send', 1),
    '#weight' => 1
  );
  $form['ricochet_maintenance_helper_target_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#description' => t('The target environment URL'),
    '#default_value' => variable_get('ricochet_maintenance_helper_target_address', RMH_ENV_URL),
    '#weight' => 2
  );
  $form['ricochet_maintenance_helper_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t("The API key for this site. It should contain only lower
      case letters and numbers. If you have development and staging environments,
      you should not store the API key in this field, but in your production
      environment's settings.php as follows:
      <code>\$settings['ricochet_maintenance_helper_environment_token'] = 'myapikey';</code>
      This is important if you are using different environments. See this module's
      documentation for more information."),
    '#size' => 60,
    '#maxlength' => 32,
    '#default_value' => variable_get('ricochet_maintenance_helper_key'),
    '#weight' => 4
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Advanced'),
    '#weight' => 5
  );

  $form['advanced']['ricochet_maintenance_helper_listen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Listen for new API key'),
    '#description' => t('If set, the module will listen for an API key sent from
      the Maintenance server. Once it has received an API key, it will
      immediately attempt to send updates to the maintenance server using this
      API key. If this update succeeds, the API key will be saved. When it is
      saved, the listening will be automatically stopped.'),
    '#default_value' => variable_get('ricochet_maintenance_helper_listen', 1),
  );

  $form['advanced']['ricochet_maintenance_helper_interval'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#description' => t('The default frequency for sending updates to the
      server. Use this if your cron runs very often.'),
    '#default_value' => variable_get('ricochet_maintenance_helper_interval', 3600),
    '#options' => array(
      0 => t('Every time cron runs'),
      3600 => t('Every hour'),
      43200 => t('Every 12 hours'),
      86400 => t('Every 24 hours'),
    ),
    '#weight' => 4
  );

  if (variable_get('ricochet_maintenance_helper_environment_token', FALSE)) {
    $form['ricochet_maintenance_helper_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override key stored in settings.php'),
      '#description' => t(
        "An API key <code>%key</code> has been detected in your site's
        settings.php file. If you want to override that key, check this box. The
        API key in the <strong>API key</strong> field below will then be used
        instead.",
        array('%key' => variable_get('ricochet_maintenance_helper_environment_token'))
      ),
      '#default_value' => variable_get('ricochet_maintenance_helper_override', 0),
      '#weight' => 3
    );
    $form['ricochet_maintenance_helper_key']['#states'] = array(
      'disabled' => array(
        ':input[name="ricochet_maintenance_helper_override"]' => array('checked' => FALSE),
      )
    );
  }
  $form['ricochet_maintenance_helper_send_now'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send update report when saving configuration'),
    '#description' => t('If this is checked, RMH will attempt to send updates
      to the server immediately after you have saved this form.'),
    '#weight' => 10
  );
  $form['#submit'][] = 'ricochet_maintenance_helper_send_update_submit';

  return system_settings_form($form);
}

function ricochet_maintenance_helper_send_update_submit($form, $form_state) {
  $values = $form_state['values'];
  if ($values['ricochet_maintenance_helper_send_now'] == TRUE) {
    // Specify the key if override is checked, to use the newly entered key.
    $key = NULL;
    if (isset($values['ricochet_maintenance_helper_override'])){
      $key = $values['ricochet_maintenance_helper_override'] ? $values['ricochet_maintenance_helper_key'] : NULL;
    }
    module_load_include('inc', 'ricochet_maintenance_helper', 'ricochet_maintenance_helper.send');
    if (ricochet_maintenance_helper_run_update_check($key)) {
      drupal_set_message('Triggered an update to the server.');
    }
    else {
      drupal_set_message('An error occurred while trying to send an update.', 'error');
    }
  }
}
