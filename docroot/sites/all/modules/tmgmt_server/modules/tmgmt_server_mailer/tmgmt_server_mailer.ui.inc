<?php
/**
 * @file
 *   Contains TMGMTFileMailerTranslatorUIController
 */

/**
 * Extends the file translator UI controller to provide mailer settings.
 */
class TMGMTFileMailerTranslatorUIController extends TMGMTFileTranslatorUIController {

  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['new_job_notification'] = array(
      '#type' => 'fieldset',
      '#title' => t('New job email notification for the translator.'),
    );
    $form['new_job_notification']['translator_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Translator email'),
      '#description' => t('Provide email address where new job notifications will be sent.'),
      '#default_value' => tmgmt_server_mailer_notification_email($translator),
    );
    $form['new_job_notification']['subject'] = array(
      '#type' => 'textfield',
      '#title' => t('New job notification subject'),
      '#default_value' => tmgmt_server_mailer_notification_subject_template($translator),
      '#description' => t('Create the notification subject. You can use token replace patterns.')
    );
    if (module_exists('token')) {
      $form['new_job_notification']['subject_token_help_wrapper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['new_job_notification']['subject_token_help_wrapper']['info'] = array(
        '#type' => 'markup',
        '#token_types' => array('tmgmt_server_mailer_job_info'),
        '#theme' => 'token_tree'
      );
    }
    $form['new_job_notification']['body'] = array(
      '#type' => 'textarea',
      '#title' => t('New job notification body'),
      '#default_value' => tmgmt_server_mailer_notification_body_template($translator),
      '#description' => t('Create the notification body. You can use token replace patterns.')
    );
    if (module_exists('token')) {
      $form['new_job_notification']['body_token_help_wrapper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['new_job_notification']['body_token_help_wrapper']['info'] = array(
        '#type' => 'markup',
        '#token_types' => array('tmgmt_server_mailer_job_info'),
        '#theme' => 'token_tree'
      );
    }
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

}
