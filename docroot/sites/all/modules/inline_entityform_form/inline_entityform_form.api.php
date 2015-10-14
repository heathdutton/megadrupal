<?hp
/**
 * @file
 * Describe hooks provided by the Inline Entityform Form module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Perform alterations before an inline entityform form is rendered.
 *
 * In addition to hook_inline_entityform_form_alter(), which is called for
 * all forms, there are two more specific form hooks available. The first,
 * hook_inline_entityform_form_BASE_FORM_ID_alter(), allows targeting of a
 * form/forms via a base form (if one exists). The second,
 * hook_inline_entityform_form_FORM_ID_alter(), can be used to target a
 * specific form directly.
 *
 * The call order is as follows: all existing form alter functions are called
 * for module A, then all for module B, etc., followed by all for any base
 * theme(s), and finally for the theme itself. The module order is determined
 * by system weight, then by module name.
 *
 * Within each module, form alter hooks are called in the following order:
 * first, hook_inline_entityform_form_alter();
 * second, hook_inline_entityform_form_BASE_FORM_ID_alter();
 * third, hook_inline_entityform_form_FORM_ID_alter().
 * So, for each module, the more general hooks are called first followed by
 * the more specific.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 * @param $form_id
 *   String representing the name of the form itself. Typically this is the
 *   name of the function that generated the form.
 * @param $base_form_id
 *   String representing the name of the function that was originally called
 *   to create form the inline_entityform _form is embedded in. Provided for
 *   convenience and also available in the array $form_state['build_info'].
 *
 * @see hook_inline_entityform_form_BASE_FORM_ID_alter()
 * @see hook_inline_entityform_form_FORM_ID_alter()
 * @see hook_form_alter()
 */
function hook_inline_entityform_form_alter(&$form, &$form_state, $form_id, $base_form_id) {
  if ($form_id == 'registration_form') {
    $form['terms_of_use'] = array(
      '#type' => 'checkbox',
      '#title' => t("I agree with the website's terms and conditions."),
      '#required' => TRUE,
    );
  }
}

/**
 * Provide a form-specific alteration instead of the global hook_inline_entityform_form_alter().
 *
 * Modules can implement hook_inline_entityform_form_FORM_ID_alter() to modify
 * a specific form, rather than implementing hook_inline_entityform_form_alter()
 * and checking the form ID, or using long switch statements to alter multiple
 * forms.
 *
 * Form alter hooks are called in the following order:
 * first, hook_inline_entityform_form_alter();
 * second, hook_inline_entityform_form_BASE_FORM_ID_alter();
 * third, hook_inline_entityform_form_FORM_ID_alter().
 * So, for each module, the more general hooks are called first followed by
 * the more specific.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 * @param $form_id
 *   String representing the name of the form itself. Typically this is the
 *   name of the function that generated the form.
 * @param $base_form_id
 *   String representing the name of the function that was originally called
 *   to create form the inline_entityform _form is embedded in. Provided for
 *   convenience and also available in the array $form_state['build_info'].
 *
 * @see hook_inline_entityform_form_alter()
 * @see hook_inline_entityform_form_BASE_FORM_ID_alter()
 * @see hook_form_alter()
 */
function hook_inline_entityform_form_FORM_ID_alter(&$form, &$form_state, $form_id, $base_form_id) {
  $form['terms_of_use'] = array(
    '#type' => 'checkbox',
    '#title' => t("I agree with the website's terms and conditions."),
    '#required' => TRUE,
  );
}

/**
 * Provide a form-specific alteration for shared ('base') forms.
 *
 * By default, when drupal_get_form() is called, Drupal looks for a function
 * with the same name as the form ID, and uses that function to build the form.
 * In contrast, base forms allow multiple form IDs to be mapped to a single base
 * (also called 'factory') form function.
 *
 * Modules can implement hook_inline_entityform_form_BASE_FORM_ID_alter() to
 * modify a specific base form, rather than implementing hook_form_alter() and
 * checking for conditions that would identify the shared form constructor.
 *
 * To identify the base form ID for a particular form (or to determine whether
 * one exists) check the $form_state. The base form ID is stored under
 * $form_state['build_info']['base_form_id'].
 *
 * See hook_forms() for more information on how to implement base forms in
 * Drupal.
 *
 * Form alter hooks are called in the following order:
 * first, hook_inline_entityform_form_alter();
 * second, hook_inline_entityform_form_BASE_FORM_ID_alter();
 * third, hook_inline_entityform_form_FORM_ID_alter().
 * So, for each module, the more general hooks are called first followed by
 * the more specific.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 * @param $form_id
 *   String representing the name of the form itself. Typically this is the
 *   name of the function that generated the form.
 * @param $base_form_id
 *   String representing the name of the function that was originally called
 *   to create form the inline_entityform _form is embedded in. Provided for
 *   convenience and also available in the array $form_state['build_info'].
 *
 * @see hook_inline_entityform_form_alter()
 * @see hook_inline_entityform_form_FORM_ID_alter()
 * @see hook_form_alter()
 */
function hook_inline_entityform_form_BASE_FORM_ID_alter(&$form, &$form_state, $form_id, $base_form_id) {
  $form['terms_of_use'] = array(
    '#type' => 'checkbox',
    '#title' => t("I agree with the website's terms and conditions."),
    '#required' => TRUE,
  );
}

/**
 * @} End of "addtogroup hooks".
 */
