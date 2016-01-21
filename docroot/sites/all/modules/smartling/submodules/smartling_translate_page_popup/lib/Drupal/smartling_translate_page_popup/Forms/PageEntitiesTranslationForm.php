<?php

namespace Drupal\smartling_translate_page_popup\Forms;

class PageEntitiesTranslationForm implements \Drupal\smartling\Forms\FormInterface {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_page_entities_translation_form';
  }

  public function buildForm(array $form, array &$form_state) {
    //Don't want to use DIC for the render of this form, as it is processed on every page for admin and
    //the issue is not solved yet: https://www.drupal.org/node/2495617
  }

  public function validateForm(array &$form, array &$form_state) {
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $errors = form_get_errors();
    if ($errors) {
      $commands[] = ajax_command_replace('#translation_result', '<div id="translation_result" class="failed">' . implode(" ", $errors) . '</div>');
      return array('#type' => 'ajax', '#commands' => $commands);
    }

    $langs = array_filter($form_state['input']['languages']);
    foreach ($form_state['input']['items'] as $v) {
      if (empty($v)) {
        continue;
      }
      $v = explode('_||_', $v);
      $id = (int) $v[0];
      $entity_type = $v[1];
      $entity = entity_load_single($entity_type, $id);

      $res = array();
      try {
        $res = drupal_container()
          ->get('smartling.queue_managers.upload_router')
          ->routeUploadRequest($entity_type, $entity, $langs);
      } catch (\Drupal\smartling\SmartlingExceptions\SmartlingGenericException $e) {
        smartling_log_get_handler()->error($e->getMessage() . '   ' . print_r($e, TRUE));
        drupal_set_message($e->getMessage());
      }

      if ($res['status']) {
        drupal_set_message($res['message']);
      }
    }

    $commands[] = ajax_command_replace('#translation_result', '<div id="translation_result" class="success">' . t('Selected entities have been successfully queued for translation.') . '</div>');
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
}