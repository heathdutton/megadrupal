<?php
/**
 * @file
 * Provides the Bazaarvoice Reviews reviews interface.
 */

/**
 * Create content for the write a review page for a product based on method.
 *
 * @param string $type
 *   The entity type.
 * @param object $entity
 *   The entity object.
 *
 * @return string
 *   Generated content HTML.
 */
function bazaarvoice_reviews_new_review($type, $entity) {
  module_load_include('module', 'bazaarvoice_products');
  $content = '';
  // Attempt to get product info.
  if ($product_info = bazaarvoice_products_get_product_info($type, $entity)) {
    // Get the method for writing a review.
    $method = variable_get('bazaarvoice_reviews_method', 'hosted');
    // Bazaarvoice hosted form?
    if ($method == 'hosted') {
      $content = bazaarvoice_reviews_new_hosted_review($product_info);
    }
    // Else assume self hosted.
    else {
      $content = bazaarvoice_reviews_new_conversations_review($product_info);
    }
  }

  return $content;
}

/**
 * Return content for hosted write a review form.
 *
 * @param array $product_info
 *    Array of product entity information.
 *
 * @return string
 *   HTML output for the page.
 */
function bazaarvoice_reviews_new_hosted_review(array $product_info) {
  // Add the external Bazaarvoice js url.
  drupal_add_js(bazaarvoice_reviews_get_hosted_js_url(), array('type' => 'external'));
  // Get submission url.
  $submission_url = bazaarvoice_reviews_get_hosted_submission_url($product_info['type'], $product_info['eid']);
  // Get user token.
  $user_token = bazaarvoice_reviews_get_user_token();
  // Add inline javascript.
  drupal_add_js('
    $BV.configure("global", {submissionContainerUrl: "' . $submission_url . '"});
    $BV.ui("submission_container", {userToken: "' . $user_token . '"});
  ', array('type' => 'inline'));
  $review = array(
    '#theme' => 'bazaarvoice_reviews_new_hosted_review',
    '#form_container' => '<div id="BVSubmissionContainer"></div>',
  );
  // Returned themed content.
  return render($review);
}

/**
 * Build specific url for external Bazaarvoice javascript.
 *
 * @return string
 *   the url to the external javascript.
 */
function bazaarvoice_reviews_get_hosted_js_url() {
  $url = 'http' . (variable_get('bazaarvoice_reviews_secure', TRUE) ? 's' : '');
  $url .= '://';
  $url .= variable_get('bazaarvoice_client_code', '') . '.ugc.bazaarvoice.com/';
  $url .= variable_get('bazaarvoice_mode', 'stg') == 'stg' ? 'bvstaging/' : '';
  $url .= 'static/' . variable_get('bazaarvoice_reviews_display_id', '') . '-';
  $url .= bazaarvoice_get_locale(FALSE, TRUE) . '/';
  $url .= 'bvapi.js';
  return $url;
}

/**
 * Generate a url to the write a review page for a product.
 *
 * @param string $type
 *   The entity type.
 * @param string $eid
 *   The entity id.
 *
 * @return string
 *   Absolute URL to write-a-review page for the product.
 */
function bazaarvoice_reviews_get_hosted_submission_url($type, $eid) {
  $write_review_uri = bazaarvoice_reviews_get_write_a_review_uri();
  $uri = $type . '/' . $eid . '/' . $write_review_uri;
  return url($uri, array('absolute' => TRUE));
}

/**
 * Generate a unique user token.
 *
 * @return string
 *   The generated user token.
 */
function bazaarvoice_reviews_get_user_token() {
  global $user;

  $user_token = '';
  // Logged-in user with data?
  if ($user->uid) {
    // Get the shared key.
    $shared_key = variable_get('bazaarvoice_reviews_shared_key', '');
    // Build user token.
    $user_token = array(
      'date=' . date('Ymd'),
      'userid=' . $user->uid,
      'emailaddress=' . urlencode($user->mail),
    );
    // Implode token, joint by '&'.
    $user_token = implode('&', $user_token);
    // Hash the user token.
    $user_token = md5($shared_key . $user_token) . bin2hex($user_token);
  }

  return $user_token;
}

/**
 * Return themed output for writing a review page for Conversations API.
 *
 * @param array $product_info
 *   Product entity information.
 *
 * @return string
 *   HTML for review page.
 */
function bazaarvoice_reviews_new_conversations_review(array $product_info) {
  // Get the form.
  $form = array(
    '#theme' => 'bazaarvoice_reviews_new_conversations_review',
    '#form' => drupal_get_form('bazaarvoice_reviews_conversations_review_form', $product_info),
    '#product_info' => $product_info,
  );
  // Theme the output.
  return render($form);
}

/**
 * Build the write a review form for conversations (self hosted) form.
 */
function bazaarvoice_reviews_conversations_review_form($form, &$form_state, $product_info) {
  $form = array();
  $form['#product_info'] = $product_info;
  // Get previously submitted values.
  $previous_values = isset($form_state['values']) ? $form_state['values'] : array();
  // Get the Bazaarvoice form fields.
  if ($fields = bazaarvoice_reviews_get_conversations_drupal_fields($product_info['product_id'])) {
    // Set default values.
    bazaarvoice_reviews_set_conversations_field_values($fields, $previous_values);
    // Add the fields to the forms.
    $form += $fields;
  }

  // Add device fingerprinting.
  // See: https://developer.bazaarvoice.com/apis/conversations/tutorials/device_fingerprinting
  $form['fp'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'id' => 'fp',
      'value' => '',
    ),
  );

  // Inline fingerprint settings.
  $fingerprint_js_settings = "window.io_bbout_element_id = 'fp';
window.io_install_stm = false;
window.io_exclude_stm = 12;
window.io_install_flash = false;
window.io_enable_rip = true;";

  // Add snare.js fingerprinting settings.
  $form['#attached']['js'][$fingerprint_js_settings] = array(
    'type' => 'inline',
  );

  // Add snare.js for fingerprinting.
  $form['#attached']['js']['https://mpsnare.iesnare.com/snare.js'] = array(
    'type' => 'external',
  );

  // Submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#weight' => 1000,
  );

  return $form;
}

/**
 * Validation callback for conversations review form.
 */
function bazaarvoice_reviews_conversations_review_form_validate($form, &$form_state) {
  // Get product info.
  $product_info = $form['#product_info'];
  // Get submitted values.
  $values = $form_state['values'];
  // Validate the conversation fields. Any errors?
  if ($errors = bazaarvoice_reviews_conversations_field_errors($product_info['product_id'], $values)) {
    // Loop through all of the errors.
    foreach ($errors as $field_id => $field_errors) {
      // For each error on a field.
      foreach ($field_errors as $error) {
        // Print a form error.
        form_set_error($field_id, check_plain($error));
      }
    }
  }
}

/**
 * Submit callback for conversations review form.
 */
function bazaarvoice_reviews_conversations_review_form_submit($form, &$form_state) {
  // Get product info.
  $product_info = $form['#product_info'];
  // Attempt to submit review.
  if ($submit = bazaarvoice_reviews_submit_conversation_review($product_info['product_id'], $form_state['values'])) {
    // Any errors?
    if (isset($submit['errors'])) {
      // Print errors.
      foreach ($submit['errors'] as $error) {
        form_set_error('', check_plain($error));
      }
    }
    else {
      $message = t('Your review has been submitted.');
      if (isset($submit['success'])) {
        $message = $submit['success'];
      }
      drupal_set_message(check_plain($message));
      // Redirect to product page?
      drupal_goto($product_info['type'] . '/' . $product_info['eid']);
    }
  }
  // Not able to submit, return generic error.
  else {
    form_set_error('', t('There was an error submitting your review'));
  }
}

/**
 * Wrapper to submit review data to Bazaarvoice.
 *
 * @param string $product_id
 *   The id of the product to submit a review for.
 * @param array $values
 *   Values to submit as part of the review.
 *
 * @return array|bool
 *   Array of submission data or boolean FALSE.
 */
function bazaarvoice_reviews_submit_conversation_review($product_id, array $values) {
  $submit = FALSE;
  // Create new bazaarvoiceReview object.
  $bazaarvoice_review = new BazaarvoiceReview();
  // Get the locale.
  $locale = bazaarvoice_get_locale(FALSE, TRUE);
  // Store field values.
  $field_values = array();
  // Get the fields.
  if ($fields = $bazaarvoice_review->getFormFields($product_id, $locale)) {
    // Loop through each field and validate.
    foreach ($fields as $id => $field) {
      $value = NULL;
      // Get the field value.
      if (isset($values[$id])) {
        $value = $values[$id];
      }

      if (!is_null($value)) {
        $field_values[$id] = $value;
        // Validate the value.
        if ($field_errors = $bazaarvoice_review->getFieldErrors($field, $value)) {
          $errors[$id] = $field_errors;
        }
      }
    }
  }

  // No errors?
  if (empty($errors)) {
    // Add device fingerprint.
    $submitted_fields['fp'] = isset($values['fp']) ? $values['fp'] : '';
    // Have bazaarvoice submit.
    if ($data = $bazaarvoice_review->submitReview($product_id, $locale, $field_values)) {
      $submit = array('success' => t('Your review has been submitted.'));
      // Know typical hours till display?
      if (isset($data['TypicalHoursToPost'])) {
        // Add to success message.
        $submit['success'] .= ' ' . t('Reviews can take up to @hours hours to display.', array('@hours' => $data['TypicalHoursToPost']));
      }
    }
    else {
      // Get the errors.
      if ($review_errors = $bazaarvoice_review->getApiErrors()) {
        foreach ($review_errors['FieldErrors'] as $field => $error) {
          $errors[$field] = array($error['Message']);
        }
        foreach ($review_errors['Errors'] as $error) {
          $errors[] = $error;
        }
        if (!empty($errors)) {
          $submit = array('errors' => $errors);
        }
      }
      // Else, some other error occurred.
      else {
        $submit = array('errors' => array('' => t('There is an error with the data you are trying to submit.')));
      }
    }
  }
  else {
    $submit = array('errors' => $errors);
  }

  return $submit;
}

/**
 * Return array of Drupal form fields for conversations review form.
 *
 * @param string $product_id
 *   The product to retrieve the fields for.
 *
 * @return mixed
 *   Array ofr Drupal form elements.
 */
function bazaarvoice_reviews_get_conversations_drupal_fields($product_id) {
  // Create a new review object.
  $bazaarvoice_review = new BazaarvoiceReview();
  // Get the locale.
  $locale = bazaarvoice_get_locale(FALSE, TRUE);
  // Retrieve the form fields.
  return $bazaarvoice_review->getDrupalFormFields($product_id, $locale);
}

/**
 * Set the default values for fields from previous values.
 *
 * @param array $fields
 *   Array of form fields to set default values.
 * @param array $previous_values
 *   Array of previously submitted values.
 */
function bazaarvoice_reviews_set_conversations_field_values(array &$fields, array $previous_values) {
  foreach ($fields as $field_id => &$field) {
    // Field Id an attribute from a recursive call?
    if (substr($field_id, 0, 1) == '#') {
      // Skip it.
      continue;
    }
    // Field has a previous value?
    if (isset($previous_values[$field_id])) {
      // Set it as the default.
      $field['#default_value'] = $previous_values[$field_id];
    }
    // Does field have children?
    if (element_children($field)) {
      // Recursive call for children.
      bazaarvoice_reviews_set_conversations_field_values($field, $previous_values);
    }
  }
}

/**
 * Check for errors in submitted values for Bazaarvoice fields.
 *
 * @param string $product_id
 *   The id of the current product review is being written for.
 * @param array $values
 *   The values being submitted for the review fields.
 *
 * @return array|bool
 *   Array of field error messages or boolean FALSE.
 */
function bazaarvoice_reviews_conversations_field_errors($product_id, array $values) {
  $errors = array();
  // Create a new review object.
  $bazaarvoice_review = new BazaarvoiceReview();
  // Get the locale.
  $locale = bazaarvoice_get_locale(FALSE, TRUE);
  // Store field values.
  $field_values = array();
  // Get the fields.
  if ($fields = $bazaarvoice_review->getFormFields($product_id, $locale)) {
    // Loop through each field and validate.
    foreach ($fields as $id => $field) {
      $value = NULL;
      // Get the field value.
      if (isset($values[$id])) {
        $value = $values[$id];
      }

      if (!is_null($value)) {
        $field_values[$id] = $value;
        // Validate the value.
        if ($field_errors = $bazaarvoice_review->getFieldErrors($field, $value)) {
          $errors[$id] = $field_errors;
        }
      }
    }
  }

  // No errors yet?
  if (empty($errors)) {
    // Have bazaarvoice validate.
    if (!$bazaarvoice_review->validateReview($product_id, $locale, $field_values)) {
      // Get the errors.
      if ($review_errors = $bazaarvoice_review->getApiErrors()) {
        foreach ($review_errors['FieldErrors'] as $field => $error) {
          $errors[$field] = array($error['Message']);
        }
      }
      // Else, some other error occurred.
      else {
        $errors = array('' => t('There is an error with the data you are trying to submit.'));
      }
    }
  }

  return empty($errors) ? FALSE : $errors;
}
