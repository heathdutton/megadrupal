<?php

namespace Drupal\smartling\Forms;

class AdminExpertSettingsForm implements FormInterface {

  protected $settings;
  protected $logger;

  public function __construct($settings, $logger) {
    $this->settings = $settings;
    $this->logger = $logger;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_expert_info_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {
    $settings = $this->settings;

    $form['log_info']['log_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Smartling log'),
      '#default_value' => $settings->getLogMode(),
      '#options' => $settings->getLogModeOptions(),
      '#description' => t('Log enabled dy default.'),
    );

    $form['log_info']['async_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Asynchronous mode'),
      '#description' => t('Content will be submitted immediately to Smartling when asynchronous mode is disabled.'),
      '#default_value' => $settings->getAsyncMode(),
    );

    $form['log_info']['convert_entities_before_translation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically convert entities for translation'),
      '#description' => t('Entities will be converted to the site default language before content is sent to Smartling.'),
      '#default_value' => $settings->getConvertEntitiesBeforeTranslation(),
    );

    $form['log_info']['ui_translations_merge_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('UI translation mode'),
      '#description' => t('Smartling will maintain existing translations during import/download and only create translations for new strings. If disabled, Smartling will overwrite existing translations.'),
      '#default_value' => $settings->getUITranslationsMergeMode(),
    );

    $form['log_info']['auto_resubmit_on_save'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatic content resubmission'),
      '#description' => t('Smartling will automatically resubmit previously translated content when updates are detected to the source (entities only).'),
      '#default_value' => $settings->getAutoResubmitFlag(),
    );

    $form['log_info']['custom_regexp_placeholder'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom placeholder (regular expression)'),
      '#description' => t('The content matching this regular expression will not be editable by translators in Smartling.'),
      '#default_value' => $settings->getCustomRegexpPlaceholder(),
    );

    $form['log_info']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    $form['#submit'][] = 'smartling_admin_expert_settings_form_submit';

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {

  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    if (isset($form_state['values']['log_mode'])) {
      if ($form_state['values']['log_mode'] == FALSE) {
        $this->logger->info('Smartling log OFF', array(), TRUE);
      }
      elseif ($form_state['values']['log_mode'] == TRUE) {
        $this->logger->info('Smartling log ON', array(), TRUE);
      }
      $this->settings->setLogMode($form_state['values']['log_mode']);
    }

    $this->settings->setAsyncMode($form_state['values']['async_mode']);
    $this->settings->setConvertEntitiesBeforeTranslation($form_state['values']['convert_entities_before_translation']);
    $this->settings->setUITranslationsMergeMode($form_state['values']['ui_translations_merge_mode']);
    $this->settings->setCustomRegexpPlaceholder($form_state['values']['custom_regexp_placeholder']);
    $this->settings->setAutoResubmitFlag($form_state['values']['auto_resubmit_on_save']);

    drupal_goto(current_path(), array('fragment' => 'smartling-expert-settings'));
  }
}