<?php

/**
 * @file
 *
 * Form_clerk_curl does the cURL work for form_clerk.
 *
 * It is called for one line of data input  / form entry at a time.
 *
 */


/**
 * Manage the cURL form submission process.
 *
 * @param $form_state
 *  The form state argument.
 */
function form_clerk_curl_do_form(&$form_state) {

  // Create a new cURL resource, if necessary.
  if (!isset($form_state['storage']['curl_handle']) ||
    ($form_state['storage']['curl_handle'] == 0)) {
    $form_state['storage']['curl_handle'] = form_clerk_init_curl($form_state);
  }

  $curl_handle = $form_state['storage']['curl_handle'];

  if ($form_state['storage']['use_response']) {
    // This is for a two-step form submission, such as upload/save.
    $result =   $form_state['storage']['last-post-result'];
  }
  else {
    $result = form_clerk_curl_exec($form_state, FALSE, NULL);
  }
  // Find the desired form on the page.
  $form_list = form_clerk_parse_result($result);
  $form_state['storage']['form_list'] = $form_list;

  // Don't POST the form if user only wants info.
  if ($form_state['storage']['info_only'] == TRUE) {
    return TRUE;
  }

  $form_found = FALSE;
  foreach ($form_list as $page_form) {
    if ($page_form['id'] == $form_state['storage']['form_id']) {
      $form_found = TRUE;
      $curl_form_fields = form_clerk_custom_form($page_form, $form_state);
      if ($curl_form_fields === FALSE) {
        // No custom processing was done, use normal flow.
        $curl_form_fields = form_clerk_get_fields($page_form, $curl_handle, $form_state);
      }
      if ($curl_form_fields == FALSE) {
        return FALSE;
      }
    }
  }

  if (!$form_found) {
    drupal_set_message(t('Form not found: "@form_id" in line #@line in file: @file',
                array('@form_id' => check_plain($form_state['storage']['form_id']),
                      '@line' => check_plain($form_state['storage']['line_number']),
                      '@file' => check_plain($form_state['storage']['input_file']))), 'error');

    return FALSE;
  }

  form_clerk_curl_exec($form_state, TRUE, $curl_form_fields);

  return TRUE;
}

/**
 * Prepare cURL with form data.
 *
 * @param $page_form
 *  Structure of the target form.
 * @param $curl_handle
 *   cURL handle for this run of Form Clerk.
 * @param $form_state
 *  The form state argument.
 */
function form_clerk_get_fields($page_form, $curl_handle, $form_state) {
  // Reset the URL based on the form 'action' field.
  curl_setopt($curl_handle, CURLOPT_URL, $page_form['action']);

  $curl_form_fields = array();
  $user_fields = $form_state['storage']['user_fields'];

  // Start with the form defaults.
  form_clerk_get_defaults($page_form, $curl_form_fields, $form_state);
  $all_names = $page_form['all_names'];
  $checkboxes = $page_form['checkboxes'];

  // Apply values provided by the user.
  foreach ($user_fields as $name => $value) {
    // Check if user has directed us to ignore this field.
    $ignore_indicator = $form_state['storage']['ignore_indicator'];
    if ($ignore_indicator == $name) {
      continue;
    }

    // Handle $user-supplied form fields that do not belong in the form.
    if (!isset($all_names[$name])) {
      drupal_set_message(t('Unknown form field name: "@name" in line #@line in file: @file',
                array('@name' => check_plain($name),
                      '@line' => check_plain($form_state['storage']['line_number']),
                      '@file' => check_plain($form_state['storage']['input_file']))), 'error');
      continue;
    }

    $curl_form_fields[$name] = $value;

    // Handle checkboxes.
    if (isset($checkboxes[$name])) {
      if ($value == 'unchecked') {
        // Unchecked checkboxes are not submitted.
        unset($curl_form_fields[$name]);
      }
      continue;
    }


  }
  return $curl_form_fields;
}


/**
 * Helper function for form_clerk_get_fields(). Get default values.
 *
 * @param $page_form
 *  Structure of the target form.
 * @param $curl_form_fields
 *   Array of name/value pairs for cURL POST.
 * @param $$checkboxes
 *   List of names of checkbox items.
 * @param $form_state
 *  The form state argument.
 */
function form_clerk_get_defaults(&$page_form, &$curl_form_fields, $form_state) {
  $page_fields = $page_form['elements'];
  $all_names = array();
  $checkboxes = array();
  foreach ($page_fields as $page_field) {
    $all_names[$page_field['name']] = 1;
    if ($page_field['disabled'] == 'disabled') {
      // These should not be submitted.
      continue;
    }
    // The form may have multiple buttons. Pick one.
    if ($page_field['type'] == 'submit') {
      if ($form_state['storage']['which_submit'] != $page_field['name']) {
        continue;
      }
    }
    // Only 'checked' checkboxes get submitted. We will make a second pass through these later.
    if ($page_field['type'] == 'checkbox') {
      $checkboxes[$page_field['name']] = 1 ;
      if ($page_field['checked'] != 'checked') {
        continue;
      }
    }
    $curl_form_fields[$page_field['name']] = $page_field['value'];
  }
  $page_form['all_names'] = $all_names;
  $page_form['checkboxes'] = $checkboxes;
  return;
}


/**
 * Parse the forms and form inputs with php Document Object Model.
 *
 * @param $result
 *  Form HTML provided by the target seerver.
*/
function form_clerk_parse_result($result) {
  // Php DOM works great. See http://php.net/manual/en/book.dom.php
  $DOM = new DOMDocument;
  $DOM->loadHTML($result);
  $form_items = array('name', 'id', 'action');
  $items = array('tag', 'id', 'name', 'value', 'type', 'checked', 'disabled');
  $form_list = array();
  // Get all forms.
  $forms = $DOM->getElementsByTagName('form');

  foreach ($forms as $DOM_form) {
    $this_form = array();
    foreach ($form_items as $form_item) {
      $this_form[$form_item]   = $DOM_form->getAttribute($form_item);
    }

    // Identify each element by name&id. We assume that the pair is unique.
    $name_index = array();
    $parsed_elements = array();
    $elements = $DOM_form->getElementsByTagName('*');
    foreach ($elements as $element) {
      $attributes = array();
      $name = $element->getAttribute('name');
      if ($name == '') {
        continue;
      }
      $attributes['tag'] = '';
      $attributes['label'] = '';
      foreach ($items as $item) {
        $attributes[$item] = $element->getAttribute($item);
      }
      $parsed_elements[] = $attributes;
      $name_id = $attributes['name'] . '&' . $attributes['id'];

      if (isset($name_index[$name_id])) {
        drupal_set_message(t('Duplicate name/id pair: @name_id',
                         array('@name_id' => check_plain($name_id)), 'warning'));
}
      $name_index[$name_id] = count($parsed_elements) - 1;
    }

    // Correlate labels with elements.
    form_clerk_parse_labels($DOM_form, $parsed_elements);

    // Identify what kind of tag an element is.
    form_clerk_parse_tags($DOM_form, $parsed_elements, $name_index);

    // 'Select' elements need another pass to get the options.
    form_clerk_parse_options($DOM_form, $parsed_elements, $name_index);

    $this_form['elements'] = $parsed_elements;
    $form_list[] = $this_form;
  }
  return $form_list;
}

/**
 * Correlate labels with elements.
 * Helper function for form_clerk_parse_result().
 *
 * @param $DOM_form
 *  Current DOM form object.
 * @param $parsed_elements
 *  Array of elements in the form.
 */
function form_clerk_parse_labels($DOM_form, &$parsed_elements) {
  $label_elements = $DOM_form->getElementsByTagName('label');
  foreach ($label_elements as $label_element) {
    $for = $label_element->getAttribute('for');
    $text = $label_element->nodeValue;
    foreach ($parsed_elements as $index => $parsed_element) {
      if ($parsed_element['id'] == $for) {
        $parsed_elements[$index]['label'] = $text;
      }
    }
  }
}

/**
 * Find options for 'select' elements.
 * Helper function for form_clerk_parse_result().
 *
 * @param $DOM_form
 *  Current DOM form object.
 * @param $parsed_elements
 *  Array of elements in the form.
 * @param $name_index
 *  Array of name/id info.
 */
function form_clerk_parse_options($DOM_form, &$parsed_elements, $name_index) {
  $selects =  $DOM_form->getElementsByTagName('select');
  foreach ($selects as $select) {
    $opts = $select->getElementsByTagName('option');
    $options = array();
    foreach ($opts as $opt) {
      $options[] = $opt->getAttribute('value');
      $selected = $opt->getAttribute('selected');
      if ($selected == "selected") {
        $selected_value = $opt->getAttribute('value');
      }
    }
    $name = $select->getAttribute('name');
    $id = $select->getAttribute('id');
    $name_id = $name . '&' . $id;
    $index = $name_index[$name_id];
    $parsed_elements[$index]['options'] = $options;
    $parsed_elements[$index]['value'] = $selected_value;
  }
}

/**
 * Hack to identify what kind of tag an element is.
 * Helper function for form_clerk_parse_result().
 *
 * @param $DOM_form
 *  Current DOM form object.
 * @param $parsed_elements
 *  Array of elements in the form.
 * @param $name_index
 *  Array of name/id info.
 */
function form_clerk_parse_tags($DOM_form, &$parsed_elements, $name_index) {
  // This is sort of a hack.
  // TODO: see if DOM functions can do this better
  $tags = array('input', 'textarea', 'select', 'option');
  foreach ($tags as $tag) {
    $tag_instances = $DOM_form->getElementsByTagName($tag);
    foreach ($tag_instances as $tag_instance) {
      $name = $tag_instance->getAttribute('name');
      if ($name == '') {
        continue;
      }
      $id = $tag_instance->getAttribute('id');
      $name_id = $name . '&' . $id;
      $index = $name_index[$name_id];
      $parsed_elements[$index]['tag'] = $tag;
    }
  }
}

/**
 * Post the form with cURL.
 *
 * @param $form_state
 *  The form state argument.
 * @param $post
 *  Boolean - posting or getting
 * @param $parsed_elements
 *  Array of elements in the form.
 * @param $curl_form_fields
 *  Array of form field name/value pairs.
 */
function form_clerk_curl_exec(&$form_state, $post, $curl_form_fields) {
  $curl_handle = $form_state['storage']['curl_handle'];
  if ($post) {
    curl_setopt($curl_handle, CURLOPT_POST, 1);
    curl_setopt($curl_handle, CURLOPT_POSTFIELDS, $curl_form_fields);
  }
  else {
    curl_setopt($curl_handle, CURLOPT_POST, 0);
  }

  // Post  the form.
  curl_setopt($curl_handle, CURLOPT_REFERER, $form_state['storage']['url']);
  curl_setopt($curl_handle, CURLOPT_URL, $form_state['storage']['url']);
  $result = curl_exec($curl_handle);
  $form_state['storage']['last-post-result'] = $result;
  if ($form_state['storage']['verbose']) {
    $report_number = $form_state['storage']['report_number']++;
    $dir = $form_state['storage']['report_dir'];
    $file_name = 'report-' . $report_number . '.html';
    fwrite(fopen($dir . $file_name, 'w'), $result);
    if (stripos($result, 'Error message') > 0) {
      drupal_set_message(t('HTML result contains "Error message": line #@line in file: @file. HTML result filename: @file_name',
                array(
                      '@line' => check_plain($form_state['storage']['line_number']),
                      '@file' => check_plain($form_state['storage']['input_file']),
                      '@file_name' => check_plain($file_name), )), 'warning');
    }
  }

  $curl_info = curl_getinfo($curl_handle);

  if ($curl_info['http_code'] != '200') {
    drupal_set_message(t('Error posting form in line #@line in file: @file. HTTP code: @http_code',
                array('@http_code' => check_plain($curl_info['http_code']),
                      '@line' => check_plain($form_state['storage']['line_number']),
                      '@file' => check_plain($form_state['storage']['input_file']))), 'error');

  }
  return $result;
}

/**
 * Initialize curl for this session.
 *
 * @param $form_state
 *  The form state argument.
 */
function form_clerk_init_curl(&$form_state) {

  // Todo: offer option to reuse cookie.
  $dir = $form_state['storage']['cookie_dir'];
  $file_name = $form_state['storage']['cookie_file'];
  $cookie_file = $dir . '/' . $file_name;
  if (file_exists($cookie_file)) {
    unlink($cookie_file);
  }

  // Create a new cURL resource.
  $curl_handle = curl_init();

  // Support proxy. This is useful for debugging and form analysis.
  if ($form_state['storage']['proxy'] != '') {
    curl_setopt($curl_handle, CURLOPT_PROXY, $form_state['storage']['proxy']);
    $form_state['storage']['report'] .= 'Proxy: ' . $form_state['storage']['proxy'] . "\n";
  }

  curl_setopt($curl_handle, CURLOPT_COOKIEFILE, $cookie_file);
  curl_setopt($curl_handle, CURLOPT_COOKIEJAR,  $cookie_file);
  curl_setopt($curl_handle, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 1);

  return $curl_handle;
}
