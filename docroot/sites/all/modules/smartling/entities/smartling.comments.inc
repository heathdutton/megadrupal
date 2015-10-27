<?php

/**
 * @file
 * Functions for comment entity.
 */

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 */
function smartling_form_comment_form_alter(&$form, $form_state) {
  if (smartling_translate_fields_configured($form['#bundle'], 'comment') && smartling_fields_method(str_ireplace('comment_node_', '', $form['#bundle']))) {
    $comment = $form['#entity'];
    if (!empty($comment->cid)) {
      if (($comment->language != LANGUAGE_NONE)) {
        $form = array_merge($form, smartling_get_comment_settings_form($form, $form_state));
      }
      else {
        $form['smartling'] = array(
          '#title' => t('Smartling management'),
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#weight' => 100,
          '#group' => 'additional_settings',
          '#attributes' => array('id' => array('smartling_fieldset')),
          '#attached' => array(
            'css' => array(
              drupal_get_path('module', 'smartling') . '/css/smartling_entity_settings.css' => array(
                'type' => 'file',
              ),
            ),
          ),
          '#modal' => TRUE,
        );

        $form['smartling']['content'] = array(
          '#type' => 'container',
        );

        $form['smartling']['content']['lang'] = array(
          '#type' => 'item',
          '#title' => '',
          '#markup' => t('This entity is set to language neutral and can\'t be translated. You must convert the entity to the default language to enable translation.'),
        );
      }
    }
  }
  else {
    drupal_set_message(t('Add translatable field to comment or <a href=\"@url?destination=@current_path\">enable translation</a> for comment_body field to enable translation.', array(
      '@current_path' => current_path(),
      '@url' => url('admin/config/regional/entity_translation/translatable/comment_body'),
        )), 'info');
  }
}

/**
 * Comment smartling form.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for Comment smartling form.
 */
function smartling_get_comment_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.comment_settings')->buildForm($form, $form_state);
}

/**
 * Comment smartling form - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_get_comment_settings_form_submit(array $form, array &$form_state) {
  drupal_container()->get('smartling.forms.comment_settings')->submitForm($form, $form_state);
  unset($_GET['destination']);
}
