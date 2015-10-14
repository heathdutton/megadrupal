<?php

namespace Drupal\bypass_form_validations;

/**
 * Contains all hook implementations for the module.
 */
class FormBypassHooks {
  /**
   * Implements hook_form_alter.
   */
  public static function BypassFormHookFormAlter(&$form, &$form_state, $form_id) {
    $bypass = FALSE;
    // One must be very carefull to what forms are these rules applied. In some cases, it has been
    // found that validation logic also performs non-validation tasks thus removing
    // validation on some forms can potentially break functionality.
    if(strpos($form_id,'_node_form') !== FALSE) {
      $bypass = TRUE;
    }
    // Let's see if any other module tells us to bypass
    // current form
    if (!$bypass) {
      foreach (module_implements('bypass_form_validations') as $module) {
        $local_result = module_invoke($module, 'bypass_form_validations', $form, $form_id);
        if ($local_result == TRUE) {
          $bypass == TRUE;
          break;
        }
      }
    }
    if (!$bypass) {
      return;
    }
    global $user;
    // Manage required fields.
    if (user_access('bypass form required fields', $user)) {
      FormBypasser::RemoveRequiredFields($form, $form_id);
    }
    // Manage validation.
    if (user_access('bypass form validations', $user)) {
      FormBypasser::RemoveFormValidations($form, $form_id);
    }
  }
}
