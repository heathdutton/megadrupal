<?php

/**
 * @file
 * Admin settings for Private messages with node.js.
 */

/**
 * Admin settings form.
 */
function privatemsg_nodejs_admin_settings() {
  $form = array();

  if (!_privatemsg_nodejs_audio_library_path()) {
    $args = array(
      '!url' => l(t('http://drupal.org/project/libraries'), 'http://drupal.org/project/libraries', array('attributes' => array('target' => '_blank'))),
    );
    $form['libraries'] = array(
      '#prefix' => '<div style="color: red">',
      '#suffix' => '</div>',
      '#markup' => t('If you want to sound working, you need to install the libraries module and audiojs library. You can find the libraries module at !url.', $args),
    );
    $args = array(
      '!url' => l(t('http://kolber.github.io/audiojs'), 'http://kolber.github.io/audiojs', array('attributes' => array('target' => '_blank'))),
    );
    $form['audiojs'] = array(
      '#prefix' => '<div style="color: red">',
      '#suffix' => '</div>',
      '#markup' => t('And audiojs library you can find at !url. Then copy a directory audiojs to sites/all/libraries. So, path to js must be - sites/all/libraries/audiojs/audio.min.js', $args),
    );
  }

  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message settings.'),
  );

  $form['message']['privatemsg_nodejs_message_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable writing status.'),
    '#default_value' => variable_get('privatemsg_nodejs_message_status', TRUE),
    '#description' => t('It is possible to see who is writing in the thread now.'),
  );

  $form['message']['privatemsg_nodejs_message_sound'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable sound for new messages.'),
    '#default_value' => variable_get('privatemsg_nodejs_message_sound', TRUE),
    '#description' => t("If you enable but do not download the sound, then the default sound will be used."),
  );

  $form['message']['privatemsg_nodejs_message_sound_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Choose a file'),
    '#default_value' => variable_get('privatemsg_nodejs_message_sound_file', 0),
    '#upload_validators'         => array(
      'file_validate_extensions' => array('wav mp3 ogg'),
    ),
    '#upload_location' => 'public://sounds/',
    '#states' => array(
      'visible' => array(
        ':input[name="privatemsg_nodejs_message_sound"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['alert'] = array(
    '#type' => 'fieldset',
    '#title' => t('Alert message settings'),
  );

  $form['alert']['privatemsg_nodejs_alert'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show alert messages'),
    '#default_value' => variable_get('privatemsg_nodejs_alert', TRUE),
  );

  $form['alert']['privatemsg_nodejs_alert_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Message length'),
    '#default_value' => variable_get('privatemsg_nodejs_alert_length', 100),
    '#description' => t('The maximum length of the message will be shown.'),
    '#states' => array(
      'visible' => array(
        ':input[name="privatemsg_nodejs_alert"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['alert']['privatemsg_nodejs_alert_sound'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable sound for alert messages'),
    '#default_value' => variable_get('privatemsg_nodejs_alert_sound', TRUE),
    '#description' => t("If you enable but do not download the sound, then the default sound will be used."),
    '#states' => array(
      'visible' => array(
        ':input[name="privatemsg_nodejs_alert"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['alert']['privatemsg_nodejs_alert_sound_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Choose a file'),
    '#default_value' => variable_get('privatemsg_nodejs_alert_sound_file', 0),
    '#upload_validators'         => array(
      'file_validate_extensions' => array('wav mp3 ogg'),
    ),
    '#upload_location' => 'public://sounds/',
    '#states' => array(
      'visible' => array(
        ':input[name="privatemsg_nodejs_alert_sound"]' => array('checked' => TRUE),
        ':input[name="privatemsg_nodejs_alert"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['minichat'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mini chats settings'),
  );

  $form['minichat']['privatemsg_nodejs_mimichats'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use mini chats'),
    '#default_value' => variable_get('privatemsg_nodejs_mimichats', TRUE),
  );

  $form['minichat']['privatemsg_nodejs_count_messages'] = array(
    '#type' => 'textfield',
    '#title' => t('Count of masseges.'),
    '#default_value' => variable_get('privatemsg_nodejs_count_messages', 5),
    '#description' => t('Count of masseges which will be loaded firstly in mini chat.'),
    '#states' => array(
      'visible' => array(
        ':input[name="privatemsg_nodejs_mimichats"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Form submit for admin settings.
 */
function privatemsg_nodejs_admin_settings_submit($form, &$form_state) {
  global $user;
  foreach ($form_state['values'] as $name => $value) {
    if (strpos($name, 'privatemsg_nodejs') !== FALSE) {
      if (in_array($name, array('privatemsg_nodejs_message_sound_file', 'privatemsg_nodejs_alert_sound_file'))) {
        if ($value) {

          // The status of the file is temp by default, it is necessary to
          // change it into permanent.
          $file = file_load($value);
          $file->status = FILE_STATUS_PERMANENT;
          file_save($file);
          file_usage_add($file, 'user', 'user', $user->uid);
        }
      }
      variable_set($name, $value);
    }
  }
}
