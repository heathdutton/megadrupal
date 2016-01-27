<?php

/**
 * @file
 * Form callbacks for cloudwords_i18n.module.
 */

/**
 * Custom submit callback for i18n_string_translate_page_overview_form().
 */
function cloudwords_i18n_string_translate_page_overview_form_submit($form, &$form_state) {
  $lang_codes = array_filter($form_state['values']['languages']);
  $context = $form_state['values']['object']->get_string_context();

  // Find existing translatables.
  $existing = cloudwords_get_translatables_by_property(array(
    'type' => $context[0],
    'objectid' => $context[1],
  ), 'language');

  switch ($form_state['values']['operation']) {
    case 'add':
      foreach ($lang_codes as $lang_code) {
        if (isset($existing[$lang_code]) && $existing[$lang_code]->status == CLOUDWORDS_QUEUE_NOT_IN_QUEUE) {
          $existing[$lang_code]->queue();
          drupal_set_message(t('Marked %label for translation.', array('%label' => $existing[$lang_code]->label)), 'status', FALSE);
        }
      }
      break;

    case 'remove':
      foreach ($lang_codes as $lang_code) {
        if (isset($existing[$lang_code]) && $existing[$lang_code]->status == CLOUDWORDS_QUEUE_QUEUED) {
          $existing[$lang_code]->dequeue();
          drupal_set_message(t('Unarked %label for translation.', array('%label' => $existing[$lang_code]->label)), 'status', FALSE);
        }
      }
      break;
  }
}

/**
 * Submit handler to let us know when string translations have been updated.
 */
function cloudwords_i18n_form_i18n_string_translate_page_form_submit($form, &$form_state) {
  $translation_status = CLOUDWORDS_TRANSLATION_STATUS_NOT_TRANSLATED;

  foreach ($form_state['values']['strings'] as $name => $value) {
    if (i18n_string_build($name)->get_translation($form_state['values']['langcode'])) {
      $translation_status = CLOUDWORDS_TRANSLATION_STATUS_TRANSLATION_EXISTS;
    }
    break;
  }

  $keys = array_keys($form_state['values']['strings']);
  $context = reset($keys);
  list($textgroup, $string_type, $objectid) = explode(':', $context);
  $type = cloudwords_i18n_get_string_type($textgroup, $string_type);

  $translatable = cloudwords_get_translatables_by_property(array(
    'textgroup' => $textgroup,
    'type' => $type,
    'objectid' => $objectid,
    'language' => $form_state['values']['langcode'],
  ));

  $translatable = reset($translatable);

  if (!$translatable) {
    $translatable = cloudwords_translatable_create(array(
      'textgroup' => $textgroup,
      'type' => $type,
      'objectid' => $objectid,
      'language' => $form_state['values']['langcode'],
      'label' => i18n_string_build($context)->get_source(),
    ));
  }

  $translatable->translation_status = $translation_status;
  $translatable->save();
}
