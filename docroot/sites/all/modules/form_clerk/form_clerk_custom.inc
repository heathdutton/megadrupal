<?php

/**
 * @file
 *
 * Form_clerk_custom allows customized handling of form fields.
 *
 */

/**
 * Provide custom treatment for individual forms.
 *
 * This function returns an array of name/value pairs for the current form.
 * The returned array will be passed unchanged to cURL as CURLOPT_POSTFIELDS.
 * Therefore the array must be complete.
 *
 * This function will be called at least once for each form, including the
 * login form.
 *
 * It will be called once per step for multi-step forms.
 *
 * Examine form_clerk_get_fields() in form_clerk_curl.inc for details about
 * how forms fields are processed by default.
 *
 */
function form_clerk_custom_form($page_form, &$form_state) {

  $form_id = $page_form['id'];

  // Assume that no customized handling is needed.
  $curl_form_fields = FALSE;

  // Check for form_ids that we are interested in, and process them.
  $form_id = $page_form['id'];
  switch ($form_id) {
    case 'my_form_id':
      // Do custom form processing here.

      // $curl_form_fields = array();
      // $form_state['storage']['report'] .= "Form id $form_id custom processing.\n";
      break;

    default:

  }
  return $curl_form_fields;
}
