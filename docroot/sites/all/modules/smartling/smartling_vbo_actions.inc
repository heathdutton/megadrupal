<?php
/**
 * @file
 * Contains callbacks for VBO actions.
 */

/**
 * Smartling do translate action.
 *
 * @param object $entity
 *   Action entity object.
 * @param array $context
 *   Context info.
 *
 * @global object $user
 *   Drupal user object.
 */
function smartling_do_translate_action($entity, array $context) {
  try {
    drupal_container()
      ->get('smartling.queue_managers.upload_router')
      ->routeUploadRequest($context['entity_type'], $entity, $context['translate_to_langs']);
  } catch (\Drupal\smartling\SmartlingExceptions\SmartlingGenericException $e) {
    smartling_log_get_handler()->error($e->getMessage() . '   ' . print_r($e, TRUE));
  }
}

/**
 * Smartling do translate action form.
 *
 * @param array $context
 *   Context info array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for smartling do translate action form.
 */
function smartling_do_translate_action_form(array $context, array &$form_state) {
  $locale_options = smartling_language_options_list();
  $form['do_translate_action']['languages'] = array(
    '#type' => 'checkboxes',
    '#options' => $locale_options,
    '#title' => t('Select languages'),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'smartling') . '/js/smartling_check_all.js'),
    ),
  );
  if (count($locale_options) >= 6) {
    $form['do_translate_action']['languages']['#attached']['css'] = array(drupal_get_path('module', 'smartling') . '/css/smartling_admin.css');
  }
  drupal_add_js(array('smartling' => array('checkAllId' => array('#edit-languages'))), 'setting');

  return $form;
}

/**
 * Smartling do translate action form - Form Validate.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_do_translate_action_validate(array $form, array &$form_state) {
  $select_languages = array();
  if (!empty($form_state['input']['languages'])) {
    foreach ($form_state['input']['languages'] as $d_locale) {
      if (!empty($d_locale)) {
        $select_languages[] = $d_locale;
      }
    }
  }

  if (count($select_languages) > 0) {
    $form_state['translate_to_langs'] = $select_languages;
  }
  else {
    form_set_error('languages', t('Please select a language.'));
  }
}

/**
 * Smartling do translate action form - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return array with languages to translate.
 */
function smartling_do_translate_action_submit(array $form, array $form_state) {
  if (!smartling_is_configured()) {
    return;
  }

  $count_op = count($form_state['selection']);
  smartling_log_get_handler()->info(format_plural($count_op, 'Smartling Bulk Submit - add 1 entity to queue', 'Smartling Bulk Submit - add @count entities to queue'));

  return array('translate_to_langs' => $form_state['translate_to_langs']);
}


function smartling_do_download_translation_action(&$entity, $context) {
  smartling_queue_download_translated_item_process($entity->eid);
}

function smartling_do_resend_translation_action(&$entity, $context) {
  drupal_container()
    ->get('smartling.wrappers.smartling_submission_wrapper')
    ->setEntity($entity)
    ->setStatusByEvent(SMARTLING_STATUS_EVENT_SEND_TO_UPLOAD_QUEUE)
    ->save();
  drupal_container()
    ->get('smartling.queue_managers.upload')
    ->add(array($entity->eid));

  drupal_set_message(t('Entity "@title" has been queued for resubmission.', array('@title' => $entity->title)));
}

function smartling_do_cancel_action($entity, array $context) {
  drupal_container()
    ->get('smartling.wrappers.smartling_submission_wrapper')
    ->setEntity($entity)
    ->setStatus(SMARTLING_STATUS_PENDING_CANCEL)
    ->save();
}
