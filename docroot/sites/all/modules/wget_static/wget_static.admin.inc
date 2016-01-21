<?php
/**
 * @file
 * Administration configurations for Wget static module.
 */

/**
 * Implements hook_form().
 */
function wget_static_config_form($form, &$form_state) {

  if (!(variable_get('wget_static_cmd_availability', TRUE))) {
    $form['wget_static_warning'] = array(
      '#markup' => '<div class= "messages warning"> ' . t('Wget Static Module was not able to find wget command access') . '</div>',
    );
    $form['wget_static_command'] = array(
      '#type' => 'textfield',
      '#title' => t('Wget URL on the server'),
      '#default_value' => variable_get('wget_static_command', '/usr/local/bin/wget'),
      '#description' => t('Important! If incorrect value provided, this module is of no use.<br> Please contact your server administrators for confirmation.'),
    );
  }

  $form['wget_static_usage'] = array(
    '#markup' => t('To use Wget Static Module use following urls: <br>
      a) For NODE - wget_static/node <br>
      b) For PATH - wget_static/path <br>
      c) For WEBSITE - wget_static/website <br>'),
  );

  $form['wget_static_content_tab_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Wget Static form Content Selection Tab Title',
    '#default_value' => variable_get('wget_static_content_tab_title', t('Select Content')),
  );

  $form['wget_static_content_tab_description'] = array(
    '#type' => 'textfield',
    '#title' => 'Wget Static form Content Selection Tab Description',
    '#default_value' => variable_get('wget_static_content_tab_description', t('Select contents for static HTML generation')),
  );

  $form['wget_static_settings_tab_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Wget Static form Settings Tab Title',
    '#default_value' => variable_get('wget_static_settings_tab_title', t('Settings')),
  );

  $form['wget_static_settings_tab_description'] = array(
    '#type' => 'textfield',
    '#title' => 'Wget Static form Settings Tab Description',
    '#default_value' => variable_get('wget_static_settings_tab_description', t('Settings for static HTML generation')),
  );

  $form['wget_static_success'] = array(
    '#type' => 'select',
    '#title' => t('After successful wget operation'),
    '#options' => array(
      0 => t('Display Success Message'),
      1 => t('Redirect to page'),
    ),
    '#default_value' => variable_get('wget_static_success', 0),
  );
  $form['wget_static_success_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter success message here.'),
    '#default_value' => variable_get('wget_static_success_message', 'Operation Successfully Completed'),
    '#states' => array(
      'visible' => array(
        ':input[name="wget_static_success"]' => array('value' => 0),
      ),
    ),
  );
  $form['wget_static_success_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter url here.'),
    '#default_value' => variable_get('wget_static_success_redirect', '<front>'),
    '#states' => array(
      'visible' => array(
        ':input[name="wget_static_success"]' => array('value' => 1),
      ),
    ),
  );

  $form['wget_static_save_download'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Wget Save/Download Option\'s Configuration'),
    '#descriptions' => t('Selected Options would be available for users.'),
    '#options' => array(
      'download' => t('Download to Local'),
      'ftp' => t('Save to FTP Server'),
      'webdav' => t('Save to Webdav Server'),
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('wget_static_save_download', array('download')),
  );

  $form['wget_static_enable_wget_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Wget debug mode'),
    '#description' => t('When enabled, wget will write logs to dblog each time it operates. Used for developer purpose only.'),
    '#default_value' => variable_get('wget_static_enable_wget_log', FALSE),
  );

  return system_settings_form($form);
}
