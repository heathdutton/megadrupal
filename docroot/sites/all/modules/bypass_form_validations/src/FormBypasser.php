<?php

namespace Drupal\bypass_form_validations;

/**
 * Contains the tools needed to bypass a form's
 * validation system and required fields.
 */
class FormBypasser {
  
  /**
   * Removes any validations in a form, and set message to user.
   */
  public static function RemoveFormValidations(&$form, $form_id) {
    if(isset($form['#validate']) || isset($form['actions']['submit']['#validate'])) {
      $validations = array();
      $validations += (isset($form['#validate']) && is_array($form['#validate'])) ? $form['#validate'] : array();
      $validations += (isset($form['actions']['submit']['#validate']) && is_array($form['actions']['submit']['#validate'])) ? $form['actions']['submit']['#validate'] : array();
      unset($form['#validate']);
      unset($form['actions']['submit']['#validate']);
      $count = count($validations);
      if ($count > 0) {
        self::FormValidationMessage(t('@count Form validations for form "@formId" have been disabled: @validations', array('@formId' => $form_id, '@validations' => implode(',', $validations), '@count' => count($validations))));
      }
    }
  }

  /**
   * Removes any required fields in a form, and set message to user.
   */
  public static function RemoveRequiredFields(&$form, $form_id) {
    // Remove required in all fields.
    $fields = static::RemoveRequiredFieldsInternal($form);
    // Tell user about results.
    if (!empty($fields)) {
      $field_titles = array();
      foreach($fields as $key => $field) {
        $field_titles[] = '' . $field['#title'] . ' (' . $key . ')';
      }
      self::FormValidationMessage(t('@count Required field/s for form "@formId" have been disabled: @fields.', array('@formId' => $form_id, '@fields' => implode(', ', $field_titles), '@count' => count($field_titles))));
    }
  }

  /**
   * Removes any required fields in a form, and set message to user.
   */
  private static function FormValidationMessage($message) {
    if ((isset($_SERVER['HTTP_X_REQUESTED_WITH']) && 
      strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) === 'xmlhttprequest')
      || (preg_match('/ajax/', $_GET['q']) === 1)) {
      // I'm AJAX! -> It is annoying to get this messages on AJAX updates
    }
    else {
      drupal_set_message($message, 'warning');
    }
  }

  /**
   * Removes required fields in a form.
   */
  private static function RemoveRequiredFieldsInternal(&$form, array $parent_key = array()) {
    if (!is_array($form)) {
      return array();
    }
    $removed = array();
    foreach ($form as $key => &$value) {
      $key_chain = $parent_key;
      $key_chain[] = $key;
      if (is_array($form[$key])) {
        if (!empty($form[$key]['#required'])) {
          if (isset($form[$key]['#type'])) {
            unset($form[$key]['#required']);
            $removed[implode(':', $key_chain)] = $form[$key];
          }
        }
      }
      $removed = array_merge($removed, self::RemoveRequiredFieldsInternal($form[$key], $key_chain));
    }
    return $removed;
  }
}
