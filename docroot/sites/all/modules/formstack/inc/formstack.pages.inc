<?php
/**
 * A Menu page callback.

 * @param $formstack_id
 *   string The ID of the formstack form passed as an argument from the hook_menu() path
 * @return
 *   string The contents of the node, that will fill the page.
 */
function formstack_form_page($formstack_id = NULL) {
  if (!is_numeric($formstack_id)) {
    //drupal_access_denied();
    //return;
  }

  $access_token = variable_get('formstack_access_token', '');
  $form = Formstack::form($access_token, $formstack_id);

  if (is_numeric($form)) {
    watchdog('Formstack', 'Formstack Error: @errorcode @errormessage', array('@errorcode' => $forms, '@errormessage' => _formstack_error_message($forms)), WATCHDOG_WARNING);
    return;
  }

  $title = $form->name;
  drupal_set_title($title);

  $render_array['formstack_form_page'] = array(
   '#markup' => html_entity_decode($form->javascript),
  );
  return $render_array;
}
