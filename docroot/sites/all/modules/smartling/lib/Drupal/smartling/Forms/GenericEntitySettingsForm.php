<?php

namespace Drupal\smartling\Forms;

class GenericEntitySettingsForm implements FormInterface {
  protected $entity_name_translated;
  protected $entity_key;


  public function __construct() {
    $this->entity_name_translated = t('Entity');
    $this->entity_key = '#entity';
  }

  protected function getOriginalEntity($entity) {
    return $entity;
  }

  protected function targetLangFormElem($id, $entity_type, $entity, $default_language) {
    $languages = smartling_language_list();

    if (!is_null($id)) {
      foreach ($languages as $d_locale => $language) {
        //if ($language->enabled != '0') {

        $entity_data = smartling_entity_load_by_conditions(array(
          'rid' => $id,
          'entity_type' => $entity_type,
          'target_language' => $d_locale,
        ));
        $language_name = check_plain($language->name);

        if ($entity_data !== FALSE) {
          $options[$d_locale] = smartling_entity_status_message($this->entity_name_translated, $entity_data->status, $language_name, $entity_data->progress);
        }
        else {
          $options[$d_locale] = $language_name;
        }

        $check[] = ($entity_data) ? $d_locale : FALSE;
        //}
      }

      $elem = array(
        '#type' => 'checkboxes',
        '#title' => 'Target Locales',
        '#options' => $options,
        '#default_value' => $check,
      );
    }
    else {
      foreach ($languages as $d_locale => $language) {
        $options[$d_locale] = check_plain($language->name);
      }

      $elem = array(
        '#type' => 'checkboxes',
        '#title' => 'Target Locales',
        '#options' => $options,
      );
    }
    return $elem;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_get_entity_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {
//    if (isset($form_state['confirm_delete']) && $form_state['confirm_delete'] === TRUE) {
//      return array();
//    }

    $entity = $this->getOriginalEntity((object) $form[$this->entity_key]);
    $entity_type = $form['#entity_type'];
    $wrapper = entity_metadata_wrapper($entity_type, $entity);
    $bundle = $wrapper->getBundle();
    $id = $wrapper->getIdentifier();

    if (!smartling_translate_fields_configured($bundle, $entity_type)) {
      return array();
    }

    $form['smartling'] = array(
      '#title' => t('Smartling management'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      //'#weight' => 100,
      '#group' => 'additional_settings',
      '#attributes' => array('id' => array('smartling_fieldset')),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'smartling') . '/css/smartling_entity_settings.css' => array(
            'type' => 'file',
          ),
        ),
        'js' => array(drupal_get_path('module', 'smartling') . '/js/smartling_check_all.js'),
      ),
      '#modal' => TRUE,
    );
    drupal_add_js(array('smartling' => array('checkAllId' => array('#edit-target'))), 'setting');

    if (language_default('language') !== field_valid_language(NULL, FALSE)) {
      //Otherwise if "title" module is enabled - it will spoil the title of the original node.
      $form['smartling']['error_default_language'] = array(
        '#type' => 'markup',
        '#markup' => '<p>Please switch to site\'s default language in order to execute the operations. The default language is: ' . language_default('language') . '</p>',
      );
      return $form;
    }

    if (entity_language($entity_type, $entity) !== language_default('language')) {
      $form['smartling']['error_default_language'] = array(
        '#type' => 'markup',
        '#markup' => '<p>Only content in the site default language can be submitted to Smartling for translation.</p>',
      );
      return $form;
    }

    $form['smartling']['content'] = array(
      '#type' => 'container',
    );

    $form['smartling']['content']['target'] = $this->targetLangFormElem($id, $entity_type, $entity, $form['language']['#default_value']);

    $form['smartling']['submit_to_translate'] = array(
      '#type' => 'submit',
      '#value' => 'Send to Smartling',
      '#submit' => array($this->getFormId() . '_submit'),
      '#states' => array(
        'invisible' => array(
          // Hide the button if term is language neutral.
          'select[name=language]' => array('value' => LANGUAGE_NONE),
        ),
      ),
    );

    $form['smartling']['submit_to_download'] = array(
      '#type' => 'submit',
      '#value' => 'Download Translation',
      '#submit' => array('smartling_download_translate_form_submit'),
      '#states' => array(
        'invisible' => array(
          // Hide the button if term is language neutral.
          'select[name=language]' => array('value' => LANGUAGE_NONE),
        ),
      ),
    );

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
    if (!smartling_is_configured()) {
      return;
    }

    $res = array();
    try {
      $res = drupal_container()
        ->get('smartling.queue_managers.upload_router')
        ->routeUploadRequest($form['#entity_type'], $form[$this->entity_key], $form_state['values']['target']);
    } catch (\Drupal\smartling\SmartlingExceptions\SmartlingGenericException $e) {
      smartling_log_get_handler()->error($e->getMessage() . '   ' . print_r($e, TRUE));
      drupal_set_message($e->getMessage());
    }

    if ($res['status']) {
      drupal_set_message($res['message']);
    }
  }
}