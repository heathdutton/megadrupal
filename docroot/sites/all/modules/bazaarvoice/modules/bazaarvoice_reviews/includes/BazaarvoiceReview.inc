<?php
/**
 * @file
 * Contains wrapper code for a BazaarvoiceReview object.
 */

/**
 * Class BazaarvoiceReview.
 */
class BazaarvoiceReview extends Bazaarvoice {

  /**
   * Get Drupal form fields structure from Bazaarvoice API elements.
   *
   * @param string $product_id
   *   The id of the product to build review form for.
   * @param string $locale
   *   The locale to fetch the review form for.
   *
   * @return array
   *   Structured associative array of Drupal form elements.
   */
  public function getDrupalFormFields($product_id, $locale) {
    $form = array();
    // Get the conversations form.
    if ($form_data = $this->getConversationsFormData($product_id, $locale)) {
      // Build Drupal form elements.
      $form = $this->generateConversationsDrupalFormElements($form_data['Form'], $form_data);
    }

    return $form;
  }

  /**
   * Get array of form fields for a product review.
   *
   * @param string $product_id
   *   The id of the product to retrieve review fields.
   * @param string $locale
   *   The locale string to retrieve the review fields.
   *
   * @return array
   *   Array of form fields.
   */
  public function getFormFields($product_id, $locale) {
    $fields = array();
    if ($form_data = $this->getConversationsFormData($product_id, $locale)) {
      foreach ($form_data['Data']['Fields'] as $id => $field) {
        // Do not include Bazaarvoice internal fields.
        if (!$this->isInternalBazaarvoiceField($id)) {
          $fields[$id] = $field;
        }
      }
    }

    return $fields;
  }

  /**
   * Retrieve form Data from Bazaarvoice.
   *
   * @param string $product_id
   *   The ID of the product to get review form for.
   * @param string $locale
   *   The locale to get the review for for.
   *
   * @return mixed|null|string
   *   Return data request from API call.
   */
  public function getConversationsFormData($product_id, $locale) {
    // Just a wrapper for an API request call.
    return $this->apiRequest('submitreview', array(
      'ProductId' => $product_id,
      'Locale' => $locale,
      'Action' => '',
    ));
  }

  /**
   * Build Drupal Form element from Bazaarvoice API elements.
   *
   * @param array $form_structure
   *   The structured API form elements to build.
   * @param array $form_data
   *   The form element data from Bazaarvoice.
   *
   * @return array
   *   Array of Drupal form elements.
   */
  private function generateConversationsDrupalFormElements(array $form_structure, array $form_data) {
    $form = array();
    // Loop through each form elements.
    foreach ($form_structure as $form_element) {
      $id = $form_element['Id'];
      // Check if it is an internal Bazaarvoice field.
      $internal = $this->isInternalBazaarvoiceField($id);
      // Check if element exists.
      $exists = isset($form_data['Data'][$form_element['Type'] . 's'][$id]);
      // If not internal and exists.
      if (!$internal && $exists) {
        // Get the form element.
        $element = $form_data['Data'][$form_element['Type'] . 's'][$id];
      }
      else {
        // Continue to next element.
        continue;
      }
      // Switch depending on element type.
      switch ($element['Type']) {
        // Groups to become a fieldset.
        case 'InputGroup':
          $id .= '_group';
          // Have sub elements?
          if (isset($element['SubElements']) && count($element['SubElements']) > 0) {
            // Add sub array.
            $form[$id] = $this->generateConversationsDrupalFormElements($element['SubElements'], $form_data);
            // Have a label?
            if (isset($element['Label']) && !empty($element['Label'])) {
              // Make it a fieldset.
              $form[$id]['#type'] = 'fieldset';
            }
          }
          break;

        // Boolean to be checkbox.
        case 'BooleanInput':
          $form[$id]['#type'] = 'checkbox';
          break;

        // Integer to be radios, 1 to 5 values.
        case 'IntegerInput':
          $form[$id]['#type'] = 'radios';
          $form[$id]['#options'] = range(1, 5);
          break;

        // Select to be a select.
        case 'SelectInput':
          $form[$id]['#type'] = 'select';
          // Loop through all of the options.
          foreach ($element['Options'] as $option) {
            $label = $option['Label'] ?: $option['Value'];
            // If label is blank, then it is the 'select' option.
            if (empty($label)) {
              $label = t('Select');
            }
            // Add to options array.
            $form[$id]['#options'][$option['Value']] = $label;
            // The selected option?
            if ($option['Selected']) {
              $form[$id]['#default'] = $option['Value'];
            }
          }
          break;

        // Textarea is a textarea.
        case 'TextAreaInput';
          $form[$id]['#type'] = 'textarea';
          break;

        // Text is textfield.
        case 'TextInput':
          $form[$id]['#type'] = 'textfield';
          break;

        default:
          // Do nothing.
          break;
      }

      // Have a set type for this form element?
      if (isset($form[$id]['#type'])) {
        // Required?
        if (isset($element['Required']) && $element['Required']) {
          $form[$id]['#required'] = TRUE;
        }
        // Default value?
        if (isset($element['Default']) && $element['Default']) {
          $form[$id]['#default_value'] = $element['Default'];
        }
        // Label?
        if (isset($element['Label']) && !empty($element['Label'])) {
          $form[$id]['#title'] = $element['Label'];
        }
        // Else, if not a group add title based on id.
        elseif ($element['Type'] != 'InputGroup') {
          $title = str_replace('_', ' ', $element['Id']);
          $title = ucwords(trim($title));
          if (!empty($title)) {
            $form[$id]['#title'] = $title;
          }
        }
        // Character count?
        if ($element['Type'] != 'InputGroup' && (isset($element['MaxLength']) || isset($element['MinLength']))) {
          $description = '';
          // Has a minlength?
          if ($element['MinLength']) {
            $description .= t('Must be at least @count characters', array('@count' => $element['MinLength']));
          }
          // Has a maxlength?
          if ($element['MaxLength']) {
            // Add max length to form element.
            $form[$id]['#maxlength'] = $element['MaxLength'];
            // Add to description.
            $description = ($description ? $description . '<br />' : '');
            $description .= t('Must be no longer than @count characters', array('@count' => $element['MaxLength']));
          }
          // Have description text?
          if ($description) {
            // Add to form element.
            $form[$id]['#description'] = $description;
          }
        }
      }
    }

    return $form;
  }

  /**
   * Check if a Bazaarvoice API element is for internal Bazaarvoice use only.
   *
   * @param string $element_id
   *   The id of the element to check.
   *
   * @return bool
   *   Boolean TRUE or FALSE.
   */
  private function isInternalBazaarvoiceField($element_id) {
    $is_internal = FALSE;
    // Create array of internal bazaarvoice fields.
    // https://developer.bazaarvoice.com/apis/conversations/tutorials/field_types#internal-fields.
    $internal_fields = array(
      'HostedAuthentication_RememberMe',
      'isratingsonly',
      'IsUserAnonymous',
      'netpromoterexpanded',
      'socialconnect_facebook_usertoken',
      'socialconnect_facebook_hostname',
      'socialconnect_facebook_postresult',
      'socialconnect_facebook_postdesired',
      'socialconnect_facebook_postonsubmit',
      'userlocationgeocode_latitude',
      'userlocationgeocode_longitude',
      'userlocationgeocode_country',
      'userlocationgeocode_region',
      'userlocationgeocode_city',
    );

    // In array of field IDs or matches 'productrecommendationcaption_[N]' ?
    if (in_array($element_id, $internal_fields) || substr($element_id, 0, 29) == 'productrecommendationcaption_') {
      $is_internal = TRUE;
    }

    return $is_internal;
  }

  /**
   * Check if the submitted value is not valid for a given field.
   *
   * @param array $field
   *   The Bazaarvoice Field array.
   * @param mixed $value
   *   The value to validate.
   *
   * @return array|bool
   *   Array of errors or boolean TRUE|FALSE.
   */
  public function getFieldErrors(array $field, $value) {
    $errors = array();
    // Get the field label.
    $label = $field['Id'];
    if (isset($field['Label']) && !empty($field['Label'])) {
      $label = $field['Label'];
    }
    else {
      $label = ucwords(trim(str_replace('_', ' ', $label)));
    }

    // Validate based on field type.
    switch ($field['Type']) {
      case 'BooleanInput':
        if (!is_bool($value) && !in_array($value, array(0, 1))) {
          $errors[] = t('Invalid value submitted for @field', array('@field' => $label));
        }
        break;

      case 'IntegerInput':
        if (!in_array($value, range(0, 5))) {
          $errors[] = t('Invalid selection for @field', array('@field' => $label));
        }
        break;

      case 'SelectInput':
        $valid_option = FALSE;
        // Loop through the options.
        foreach ($field['Options'] as $option) {
          if ($option['Value'] == $value) {
            $valid_option = TRUE;
            break;
          }
        }

        if (!$valid_option) {
          $errors[] = t('Invalid selection for @field', array('@field' => $label));
        }
        break;

      case 'TextAreaInput':
      case 'TextInput':
        // Check that it is a plain value.
        if (check_plain($value) != $value) {
          $errors[] = t('Only plain text is accepted for @field', array('@field' => $label));
        }

        // Have a min length?
        if (isset($field['MinLength']) && (strlen($value) < $field['MinLength'])) {
          $errors[] = t('@field text must be a minimum of @count characters.', array('@field' => $label, '@count' => $field['MinLength']));
        }

        // Have a max length?
        if (isset($field['MaxLength']) && (strlen($value) > $field['MaxLength'])) {
          $errors[] = t('@field text must be a maximum of @count characters.', array('@field' => $label, '@count' => $field['MaxLength']));
        }
        break;
    }

    return !empty($errors) ? $errors : FALSE;
  }

  /**
   * Submit review values to Bazaarvoice to be validated.
   *
   * @param string $product_id
   *   The id of the product being reviewed.
   * @param string $locale
   *   The locale code being reviewed.
   * @param array $values
   *   Array of fields and values being submitted.
   *
   * @return bool
   *   Boolean TRUE or FALSE is the data is valid.
   */
  public function validateReview($product_id, $locale, array $values) {
    $endpoint = 'submitreview';
    $params = array(
      'ProductId' => $product_id,
      'Locale' => $locale,
      'Action' => 'Preview',
    );

    $params = array_merge($params, $values);
    // Make an API request.
    if ($data = $this->apiRequest($endpoint, $params, 'POST')) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Submit review values to Bazaarvoice.
   *
   * @param string $product_id
   *   The id of the product being reviewed.
   * @param string $locale
   *   The locale code being reviewed.
   * @param array $values
   *   Array of fields and values being submitted.
   *
   * @return bool|mixed|null|string
   *   Response data or boolean FALSE if failed.
   */
  public function submitReview($product_id, $locale, array $values) {
    $endpoint = 'submitreview';
    $params = array(
      'ProductId' => $product_id,
      'Locale' => $locale,
      'Action' => 'submit',
    ) + $values;

    // Need to pass additional headers.
    $request_options = array(
      'headers' => array(
        'X-Forwarded-For' => ip_address(),
      ),
    );

    // Make API Call.
    if ($data = $this->apiRequest($endpoint, $params, 'POST', 60, $request_options)) {
      return $data['HasErrors'] ? FALSE : $data;
    }

    return FALSE;
  }

  /**
   * Retrieve all reviews for a product.
   *
   * @param string $product_id
   *   The product id.
   * @param string $locale
   *   The locale code.
   *
   * @return array|bool
   *   Array of reviews or boolean FALSE.
   */
  public function getAllReviews($product_id, $locale) {
    $reviews = array();
    // First need to get stats.
    $endpoint = 'statistics';
    $parameters = array(
      'filter' => array(
        'ProductId' => $product_id,
        'ContentLocale' => $locale,
      ),
      'stats' => 'NativeReviews',
    );

    $total_reviews = 0;
    if ($data = $this->apiRequest($endpoint, $parameters)) {
      // @TODO: need to be more dynamic with fetching this data.
      $total_reviews = $data['Results'][0]['ProductStatistics']['NativeReviewStatistics']['TotalReviewCount'];
    }

    // Have reviews?
    if ($total_reviews > 0) {
      // API only returns up to 100, so make multiple requests to get all.
      for ($offset = 0; $offset < $total_reviews; $offset += 100) {
        $params = array(
          'Offset' => $offset,
          'Limit' => 100,
        );
        if ($page_reviews = $this->getReviews($product_id, $locale, $params)) {
          $reviews += $page_reviews;
        }
        else {
          $offset = $total_reviews;
        }
      }
    }

    return $reviews;
  }

  /**
   * Method for getting reviews for a product.
   *
   * @param string $product_id
   *   The unique product id.
   * @param string $locale
   *   The locale code.
   * @param array $parameters
   *   Parameters to filter/order reviews by.
   *
   * @return bool|array
   *   Array of reviews for boolean FALSE.
   */
  public function getReviews($product_id, $locale, $parameters = array()) {
    $return = FALSE;
    $endpoint = 'reviews';
    // Add product id to Filter array.
    $parameters['Filter']['ProductId'] = $product_id;
    $parameters['Locale'] = $locale;
    // API request for product reviews. Add a larger cache since it is a lot
    // of data.
    if ($data = $this->apiRequest($endpoint, $parameters, 'GET', 3600)) {
      $return = $data['Results'];
    }

    return $return;
  }

}
