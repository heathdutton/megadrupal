<?php

/**
 * @file
 * Admin page callbacks for the browser settings configuration form.
 */

/**
 * Form constructor for the browser settings configuration.
 */
function device_detector_browser_settings_form($form, &$form_state) {
  // Variable settings for Body.
  $form['body_class_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Webpage Body Class settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['body_class_details']['device_detector_body_common_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Body Common Class Name/Names'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on <body> tag.<br />eg. classA,classB..<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_body_common_class_name', ''),
    '#required' => FALSE,
  );
  $form['body_class_details']['device_detector_url_as_body_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add the whole url value as class names.</b>'),
    '#description' => t('Adds all the arguments of the url to every page on  <body> tag.<br />eg. for "front" page it adds default class "home".<br />eg. for url "param1/param2/param3" the class name would be "param1 param2 param3".<br />eg. for url "node/6" the class name would be "node node-6".'),
    '#default_value' => variable_get('device_detector_url_as_body_class', 0),
    '#required' => FALSE,
  );
  $form['body_class_details']['device_detector_url_last_arg_as_body_class'] = array(
    '#type' => 'checkbox',
    '#title' => t("<b>Add the url's last argument value as class name</b>"),
    '#description' => t('Adds the last argument of the url to every page on  <body> tag.<br />eg. for url "param1/param2/param3" the class name would be param3.<br />eg. for url "node/6" the class name would be node-6.'),
    '#default_value' => variable_get('device_detector_url_last_arg_as_body_class', 0),
    '#required' => FALSE,
  );
  $form['body_class_details']['device_detector_node_spfc_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Node specific class name</b>'),
    '#description' => t('Adds the class names to the <body> tag specific to node.'),
    '#default_value' => variable_get('device_detector_node_spfc_class', 0),
    '#required' => FALSE,
    '#ajax' => array(
      'callback' => 'device_detector_show_dependent_field',
    ),
  );
  $form['body_class_details']['node_cstomize'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_node_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['body_class_details']['node_cstomize']['device_detector_node_urls'] = array(
    '#type' => 'textarea',
    '#title' => t("Add node url's"),
    '#description' => t("Add node url's separated by new line for multiple nodes"),
    '#default_value' => variable_get('device_detector_node_urls', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_node_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['body_class_details']['node_cstomize']['device_detector_node_class_names'] = array(
    '#type' => 'textarea',
    '#title' => t("Add class names for the corresponding node's"),
    '#description' => t("Please enter class names, comma(,) separated for multiple class names, on the same line for its corresponding node.<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_node_class_names', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_node_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  // Browser Specific Settings.
  $form['browser_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser specific Class settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['browser_details']['device_detector_brwsr_as_class_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Browser Name as class Name</b>'),
    '#description' => t("Check this box to add the browser name as the class name to body"),
    '#default_value' => variable_get('device_detector_brwsr_as_class_name', 0),
    '#required' => FALSE,
  );
  $form['browser_details']['device_detector_brwsr_vrsion_as_class_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Browser along with its Version as class Name</b>'),
    '#description' => t("Check this box to add the browser name with its version as the class name to body"),
    '#default_value' => variable_get('device_detector_brwsr_vrsion_as_class_name', 0),
    '#required' => FALSE,
  );
  $form['browser_details']['device_detector_chrm_spfc_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Custom class specific to the browser :: Google Chrome</b>'),
    '#default_value' => variable_get('device_detector_chrm_spfc_class', 0),
    '#required' => FALSE,
    '#ajax' => array(
      'callback' => 'device_detector_show_chrm_field',
      'wrapper' => 'chrm_replace_field_div',
    ),
  );
  $form['browser_details']['device_detector_chrm_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Chrome specific Class Name/Names'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB....<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_chrm_class_name', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_chrm_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['browser_details']['device_detector_firefx_spfc_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Custom class specific to the browser :: Mozilla Firefox</b>'),
    '#default_value' => variable_get('device_detector_firefx_spfc_class', 0),
    '#required' => FALSE,
    '#ajax' => array(
      'callback' => 'device_detector_show_firefx_field',
      'wrapper' => 'firefx_replace_field_div',
    ),
  );
  $form['browser_details']['device_detector_firefx_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Firefox specific Class Name/Names'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB....<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_firefx_class_name', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_firefx_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['browser_details']['device_detector_ie_spfc_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Custom class specific to the browser :: Internet Explorer</b>'),
    '#default_value' => variable_get('device_detector_ie_spfc_class', 0),
    '#required' => FALSE,
    '#ajax' => array(
      'callback' => 'device_detector_show_ie_field',
      'wrapper' => 'ie_replace_field_div',
    ),
  );
  $form['browser_details']['device_detector_ie_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('IE specific Class Name/Names'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB....<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_ie_class_name', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_ie_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['browser_details']['device_detector_safari_spfc_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Custom class specific to the browser :: Safari</b>'),
    '#default_value' => variable_get('device_detector_safari_spfc_class', 0),
    '#required' => FALSE,
    '#ajax' => array(
      'callback' => 'device_detector_show_safari_field',
      'wrapper' => 'safari_replace_field_div',
    ),
  );
  $form['browser_details']['device_detector_safari_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Safari specific Class Name/Names'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB....<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_safari_class_name', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_safari_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['browser_details']['device_detector_opera_spfc_class'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Custom class specific to the browser :: Opera Mini</b>'),
    '#default_value' => variable_get('device_detector_opera_spfc_class', 0),
    '#required' => FALSE,
    '#ajax' => array(
      'callback' => 'device_detector_show_opera_field',
      'wrapper' => 'opera_replace_field_div',
    ),
  );
  $form['browser_details']['device_detector_opera_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Opera Mini specific Class Name/Names'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB....<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_opera_class_name', ''),
    '#required' => FALSE,
    '#states' => array(
      "visible" => array(
        "input[name='device_detector_opera_spfc_class']" => array(
          "checked" => TRUE,
        ),
      ),
    ),
  );
  $form['#validate'][] = 'device_detector_browser_settings_form_validate';
  $form['#submit'][] = 'device_detector_browser_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Call back function for field "device_detector_node_spfc_class".
 */
function device_detector_show_dependent_field($form, $form_state) {
  return $form['body_class_details']['node_cstomize'];
}

/**
 * Call back function for field "device_detector_chrm_spfc_class".
 */
function device_detector_show_chrm_field($form, $form_state) {
  return $form['browser_details']['device_detector_chrm_class_name'];
}

/**
 * Call back function for field "device_detector_firefx_spfc_class".
 */
function device_detector_show_firefx_field($form, $form_state) {
  return $form['browser_details']['device_detector_firefx_class_name'];
}

/**
 * Call back function for field "device_detector_ie_spfc_class".
 */
function device_detector_show_ie_field($form, $form_state) {
  return $form['browser_details']['device_detector_ie_class_name'];
}

/**
 * Call back function for field "device_detector_safari_spfc_class".
 */
function device_detector_show_safari_field($form, $form_state) {
  return $form['browser_details']['device_detector_safari_class_name'];
}

/**
 * Call back function for field "device_detector_opera_spfc_class".
 */
function device_detector_show_opera_field($form, $form_state) {
  return $form['browser_details']['device_detector_opera_class_name'];
}

/**
 * Implementing custom validate handler for the system form.
 */
function device_detector_browser_settings_form_validate($form, &$form_state) {
  // Check for Body common class.
  if (!empty($form_state['values']['device_detector_body_common_class_name']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_body_common_class_name'])) {
    form_set_error('device_detector_body_common_class_name', t('Please provide Body Common Class Name/Names in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_body_common_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_body_common_class_name'], ",")) {
    form_set_error('device_detector_body_common_class_name', t("Don't include ',' at the beginning or end of Body Common Class Name/Names."));
  }
  // Check for Node specific class.
  if (!empty($form_state['values']['device_detector_node_spfc_class'])) {
    // Check for Node url's.
    if (!empty($form_state['values']['device_detector_node_urls'])) {
      if (empty($form_state['values']['device_detector_node_class_names'])) {
        form_set_error('device_detector_node_class_names', t("Please provide class names for the corresponding node's in required format"));
      }
      elseif (!(trim($form_state['values']['device_detector_node_urls']))) {
        form_set_error('device_detector_node_urls', t("Please provide node url's in required format."));
      }
    }
    else {
      form_set_error('device_detector_node_urls', t("Please provide node url's in required format."));
    }
    // Check for Node specific class.
    if (!empty($form_state['values']['device_detector_node_class_names'])) {
      if (empty($form_state['values']['device_detector_node_urls'])) {
        form_set_error('device_detector_node_urls', t("Please provide node url's in required format"));
      }
      elseif (!preg_match("#^[a-zA-Z,\r\n]+$#", $form_state['values']['device_detector_node_class_names'])) {
        form_set_error('device_detector_node_class_names', t("Please provide class names for the corresponding node's in required format."));
      }
      elseif (device_detector_str_starts_with($form_state['values']['device_detector_node_class_names'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_node_class_names'], ",")) {
        form_set_error('device_detector_node_class_names', t("Don't include ',' at the beginning or end of the class names for the corresponding node's."));
      }
    }
    else {
      form_set_error('device_detector_node_class_names', t("Please provide class names for the corresponding node's in required format."));
    }
  }
  elseif (!empty($form_state['values']['device_detector_node_urls']) || !empty($form_state['values']['device_detector_node_class_names'])) {
    form_set_error('device_detector_node_spfc_class', t("Please check the option 'Add Node specific class name', remove the junk node url's/class names and then uncheck the option."));
  }
  // Check for Chrome specific class.
  if (($form_state['values']['device_detector_chrm_spfc_class']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_chrm_class_name'])) {
    form_set_error('device_detector_chrm_class_name', t('Please provide Chrome specific Class Name/Names in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_chrm_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_chrm_class_name'], ",")) {
    form_set_error('device_detector_chrm_class_name', t("Don't include ',' at the beginning or end of Chrome specific Class Name/Names."));
  }
  elseif (!($form_state['values']['device_detector_chrm_spfc_class']) && !empty($form_state['values']['device_detector_chrm_class_name'])) {
    form_set_error('device_detector_chrm_spfc_class', t("Please check the option 'Add Custom class specific to the browser :: Google Chrome', remove the junk class names and then uncheck the option."));
  }
  // Check for Firefox specific class.
  if (($form_state['values']['device_detector_firefx_spfc_class']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_firefx_class_name'])) {
    form_set_error('device_detector_firefx_class_name', t('Please provide Firefox specific Class Name/Names in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_firefx_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_firefx_class_name'], ",")) {
    form_set_error('device_detector_firefx_class_name', t("Don't include ',' at the beginning or end of Chrome specific Class Name/Names."));
  }
  elseif (!($form_state['values']['device_detector_firefx_spfc_class']) && !empty($form_state['values']['device_detector_firefx_class_name'])) {
    form_set_error('device_detector_firefx_spfc_class', t("Please check the option 'Add Custom class specific to the browser :: Mozilla Firefox', remove the junk class names and then uncheck the option."));
  }
  // Check for IE specific class.
  if (($form_state['values']['device_detector_ie_spfc_class']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_ie_class_name'])) {
    form_set_error('device_detector_ie_class_name', t('Please provide IE specific Class Name/Names in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_ie_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_ie_class_name'], ",")) {
    form_set_error('device_detector_ie_class_name', t("Don't include ',' at the beginning or end of IE specific Class Name/Names."));
  }
  elseif (!($form_state['values']['device_detector_ie_spfc_class']) && !empty($form_state['values']['device_detector_ie_class_name'])) {
    form_set_error('device_detector_ie_spfc_class', t("Please check the option 'Add Custom class specific to the browser :: Internet Explorer', remove the junk class names and then uncheck the option."));
  }
  // Check for Safari specific class.
  if (($form_state['values']['device_detector_safari_spfc_class']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_safari_class_name'])) {
    form_set_error('device_detector_safari_class_name', t('Please provide Safari specific Class Name/Names in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_safari_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_safari_class_name'], ",")) {
    form_set_error('device_detector_safari_class_name', t("Don't include ',' at the beginning or end of Safari specific Class Name/Names."));
  }
  elseif (!($form_state['values']['device_detector_safari_spfc_class']) && !empty($form_state['values']['device_detector_safari_class_name'])) {
    form_set_error('device_detector_safari_spfc_class', t("Please check the option 'Add Custom class specific to the browser :: Safari', remove the junk class names and then uncheck the option."));
  }
  // Check for Opera specific class.
  if (($form_state['values']['device_detector_opera_spfc_class']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_opera_class_name'])) {
    form_set_error('device_detector_opera_class_name', t('Please provide Opera Mini specific Class Name/Names in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_opera_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_opera_class_name'], ",")) {
    form_set_error('device_detector_opera_class_name', t("Don't include ',' at the beginning or end of Opera Mini specific Class Name/Names."));
  }
  elseif (!($form_state['values']['device_detector_opera_spfc_class']) && !empty($form_state['values']['device_detector_opera_class_name'])) {
    form_set_error('device_detector_opera_spfc_class', t("Please check the option 'Add Custom class specific to the browser :: Opera Mini', remove the junk class names and then uncheck the option."));
  }
}

/**
 * Implementing custom submit handler for the system form.
 */
function device_detector_browser_settings_form_submit($form, &$form_state) {
  // Setting "device_detector_is_body_settings_chkd" variable value.
  if ((!empty($form_state['values']['device_detector_body_common_class_name'])) || (($form_state['values']['device_detector_url_as_body_class']) == 1) || (($form_state['values']['device_detector_url_last_arg_as_body_class']) == 1) || (($form_state['values']['device_detector_node_spfc_class']) == 1)) {
    variable_set('device_detector_is_body_settings_chkd', TRUE);
  }
  // Setting "device_detector_is_brwsr_settings_chkd" variable value.
  if ((($form_state['values']['device_detector_brwsr_as_class_name']) == 1) || (($form_state['values']['device_detector_brwsr_vrsion_as_class_name']) == 1) || (($form_state['values']['device_detector_chrm_spfc_class']) == 1) || (($form_state['values']['device_detector_firefx_spfc_class']) == 1) || (($form_state['values']['device_detector_ie_spfc_class']) == 1) || (($form_state['values']['device_detector_safari_spfc_class']) == 1) || (($form_state['values']['device_detector_opera_spfc_class']) == 1)) {
    variable_set('device_detector_is_brwsr_settings_chkd', TRUE);
  }
}
