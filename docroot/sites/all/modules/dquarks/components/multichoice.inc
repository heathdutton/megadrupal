<?php

/**
 * @file
 * Quiz module multichoice component.
 * @author IP-TECH
 */

/**
 * Create a default multichoice component.
 */
function _dquarks_defaults_multichoice() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'items' => array(
        "choices" => array(),
        "labels" => array(),
        "notes" => array(),
      ),
      'multiple' => NULL,
      'description' => '',
      'justificatif' => '',
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form.
 *
 * @param array $currfield
 *   The current field.
 * @param array $node
 *   The node object.
 * @param array $form_state
 *   An array of form items of multichoice component.
 *
 * @return array
 *   An array of form items to be displayed on the edit component page.
 */
function _dquarks_edit_multichoice($currfield, $node, &$form_state) {
  if (!isset($form_state['values'])) {
    $items = $currfield['extra']['items'];
  }
  else {
    $items = $form_state['values']['extra']['items'];
  }

  $form = array();

  $form['extra']['items'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#weight' => -2,
    '#theme' => 'dquarks_edit_multichoice_items',
    '#prefix' => "<div id='all_questions'>",
    '#suffix' => "</div>",
  );
  $ajax_path = "node/" . $node->nid . "/dquarks/components";

  foreach ($items["notes"] as $key => $option) {
    $form['extra']['items']["choices"][$key] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($items["choices"][$key]) ? $items["choices"][$key] : FALSE,
    );
    $form['extra']['items']["labels"][$key] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#default_value' => isset($items["labels"][$key]) ? $items["labels"][$key] : '',
    );
    $form['extra']['items']["notes"][$key] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#default_value' => isset($items["notes"][$key]) ? $option : 0,
    );
    $form['extra']['items']["operations"][$key] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array("_dquarks_edit_submit_multichoice_del_questions"),
      '#ajax' => array(
        'path' => $ajax_path . "/callback/" . $key,
        'callback' => '_dquarks_edit_submit_multichoice_questions_ajax_callback',
        'wrapper' => 'all_questions',
        'effect' => 'fade',
        'method' => 'replace',
      ),
    );
  }
  $form['extra']['items']["choices"]['add'] = array(
    '#type' => 'checkbox',
  );
  $form['extra']['items']["labels"]['add'] = array(
    '#type' => 'textfield',
    '#size' => 30,
  );
  $form['extra']['items']["notes"]['add'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#default_value' => 0,
  );
  $form['extra']['items']["operations"]['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => array('_dquarks_edit_submit_multichoice_add_questions'),
    '#ajax' => array(
      'callback' => '_dquarks_edit_submit_multichoice_questions_ajax_callback',
      'wrapper' => 'all_questions',
      'effect' => 'fade',
    ),
  );

  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#return_value' => 'Y',
    '#default_value' => $currfield['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
  );
  $form['extra']['justificatif'] = array(
    '#type' => 'textarea',
    '#title' => t('Justification'),
    '#weight' => -2,
    '#default_value' => $currfield['extra']['justificatif'],
    '#description' => t('Put your justification here.'),
  );
  return $form;
}

/**
 * Ajax callback to update an option fieldset.
 *
 * @param arrray $form
 *   An array of form items of multichoice component.
 * @param array $form_state
 *   An array of form items of multichoice component.
 *
 * @return array
 *   Return the option fieldset after update.
 */
function _dquarks_edit_submit_multichoice_questions_ajax_callback($form, $form_state) {
  return $form['extra']['items'];
}

/**
 * Format the text output of option fieldset for this component.
 *
 * @param array $variables
 *   Contains an array of form item.
 *
 * @return string
 *   Return a themed output
 */
function theme_dquarks_edit_multichoice_items($variables) {

  $items = $variables['form'];
  $header = array(t("Correct"), t("Answer"), t("Note"), t("Operations"));
  foreach (element_children($items["choices"]) as $name) {
    $rows[] = array(
      drupal_render($items["choices"][$name]),
      drupal_render($items["labels"][$name]),
      drupal_render($items["notes"][$name]),
      drupal_render($items["operations"][$name]),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 *  Add choice in the option fieldset.
 *
 * @param array $form
 *   An array of form items of multichoice component.
 * @param array $form_state
 *   An array of form items values of multichoice component.
 */
function _dquarks_edit_submit_multichoice_add_questions($form, &$form_state) {
  $items = array('choices', 'labels', 'notes');
  foreach ($items as $keys) {
    $add[$keys] = $form_state['input']['extra']['items'][$keys]['add'];
    unset($form_state['input']['extra']['items'][$keys]['add']);
  }
  if (count($form_state['values']['extra']['items']['choices']) > 1) {
    $max = 1 + max(array_keys($form_state['values']['extra']['items']['choices']));
  }
  else {
    $max = 1 + array_pop(array_keys($form_state['values']['extra']['items']['choices']));
  }
  if (!empty($add['labels'])) {
    foreach ($items as $keys) {
      $form_state['values']['extra']['items'][$keys][$max] = $add[$keys];
      unset($form_state['values']['extra']['items'][$keys]['add']);
    }
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback function to delete a choice / question.
 *
 * @param array $form
 *   An array of form items of multichoice component.
 * @param array $form_state
 *   An array of form items of multichoice component.
 */
function _dquarks_edit_submit_multichoice_del_questions($form, &$form_state) {
  $items = array('choices', 'labels', 'notes', 'operations');
  $id = (int) check_plain($_POST['del_question']);
  foreach ($items as $keys) {
    unset($form_state['values']['extra']['items'][$keys][$id]);
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Implements hook_submit().
 *
 * @param array $form
 *   An array of form items of multichoice component.
 * @param array $form_state
 *   An array of form items of multichoice component.
 */
function _dquarks_edit_submit_multichoice($form, &$form_state) {
  unset($form_state["values"]["extra"]["items"]["operations"]);
  $items = array("choices", "labels", "notes");
  foreach ($items as $keys) {
    unset($form_state["values"]["extra"]["items"][$keys]["add"]);
  }
  unset($form_state["storage"]["items"]);
}

/**
 * Module specific instance of hook_theme().
 */
function _dquarks_theme_multichoice() {
  return array(
    'dquarks_edit_multichoice' => array(
      'variables' => array('form' => NULL, 'component' => NULL),
      'file' => 'components/multichoice.inc',
    ),
    'dquarks_mail_multichoice' => array(
      'variables' => array(
        'data' => NULL,
        'component' => NULL,
      ),
      'file' => 'components/multichoice.inc',
    ),
  );
}

/**
 * Build a form item array containing all the properties of this component.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 *
 * @return array
 *   An array of a form item to be displayed on the client-side dquarks.
 */
function _dquarks_render_multichoice($component) {
  $form_item = array(
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => _dquarks_filter_descriptions($component['extra']['description']),
    '#prefix' => '<div class="dquarks-component-' . $component['type'] . '" id="dquarks-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  // Convert the user-entered options list into an array.
  $default_value = _dquarks_filter_values($component['value'], NULL, NULL, FALSE);
  $options = _dquarks_multichoice_options($component['extra']['items']);

  // Set the component options.
  $form_item['#options'] = $options;

  if ($component['extra']['multiple'] === 'Y') {
    // Set display as a checkbox set.
    $form_item['#type'] = 'checkboxes';
    // Drupal 7 hack to properly render on multipage forms.
    $form_item['#process'] = array('dquarks_expand_multichoice_checkboxes');
  }
  else {
    // Set display as a radio set.
    $form_item['#type'] = 'radios';
  }
  return $form_item;
}

/**
 * Expand multichoice checkboxes.
 *
 * @param array $element
 *   The multichoice element.
 *
 * @return array
 *   The values of multichoice element.
 */
function dquarks_expand_multichoice_checkboxes($element) {
  // Elements that have a value set are already in the form structure cause
  // them not to be written when the expand_checkboxes function is called.
  $default_value = array();
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['#default_value'])) {
      $default_value[$key] = $element[$key]['#default_value'];
      unset($element[$key]);
    }
  }

  $element = form_process_checkboxes($element);
  foreach ($default_value as $key => $val) {
    $element[$key]['#default_value'] = $val;
  }
  return $element;
}

/**
 * Display the result of a textfield submission.
 *
 * @param array $data
 *   An array of information containing the submission result, directly
 *   correlating to the dquarks_submitted_data database schema.
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 * @param boolean $enabled
 *   If enabled, the value may be changed. Otherwise it will set to readonly.
 *
 * @return array
 *   Textual output formatted for human reading.
 */
function _dquarks_submission_display_multichoice($data, $component, $enabled = FALSE) {
  $form_item = _dquarks_render_multichoice($component);
  $options = $form_item['#options'];
  foreach ($options as $key => $value) {
    $options2[$value] = trim($value);
  }
  $form_item['#options'] = $options2;
  if ($component['extra']['multiple'] === 'Y') {
    // Set the value as an array.
    $form_item['#default_value'] = array();
    foreach ((array) $data['value'] as $key => $value) {
      $form_item['#default_value'][] = $value;
    }
  }
  else {
    // Set the value as a single string.
    $form_item['#default_value'] = '';
    foreach ((array) $data['value'] as $key => $value) {
      $form_item['#default_value'] = $value;
    }
  }
  $form_item['#disabled'] = !$enabled;
  return $form_item;
}

/**
 * Convert FAPI 0/1 values into something saveable.
 *
 * @param array $data
 *   The POST data associated with the component.
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 *
 * @return array
 *   Nothing.
 */
function _dquarks_submit_multichoice(&$data, $component) {
  $corrects = $childs = $return = array();
  $options = $component['extra']['items']["labels"];
  $wrong = FALSE;
  $score = 0;
  $max_score = 0;
  $corrects = array_intersect_key($component['extra']['items']["labels"], array_filter($component['extra']['items']["choices"]));
  if (is_array($data)) {
    foreach ($data as $key => $value) {
      if ($value != '') {
        if ($component['extra']['items']["choices"][$key]) {
          $childs[] = t("Correct answer :") . ' ' . $options[$key];
          $score = $score + $component['extra']['items']["notes"][$key];
        }
        else {
          $childs[] = t("Wrong answer :") . ' ' . $options[$key];
          $wrong = TRUE;
        }
        $data[$key] = $options[$key];
        $max_score = +array_sum(array_values($component['extra']['items']["notes"]));
      }
      else {
        unset($data[$key]);
      }
    }
  }
  else {
    if (!empty($data) && isset($component['extra']['items']["choices"][$data]) && $component['extra']['items']["choices"][$data]) {
      $childs[] = t("Correct answer :") . ' ' . $options[$data];
      $score = $score + $component['extra']['items']["notes"][$data];
    }
    else {
      $text = (!empty($data)) ? $options[$data] : "You have not select any option";
      $childs[] = t("Wrong answer :") . ' ' . $text;
      $wrong = TRUE;
    }
    $max_score = +array_sum(array_values($component['extra']['items']["notes"]));
    if (!isset($data)) {
      $data = array(-1 => NULL);
    }
    else {
      $data = array($data => $options[$data]);
    }
  }
  if ($wrong) {
    if (count($corrects)) {
      $childs[] = array("data" => t("Correct answers are :"), "children" => $corrects);
    }
    else {
      $childs[] = array("data" => t("The Correct answer is :"), "children" => $corrects);
    }
  }
  if (!empty($component['extra']['justificatif'])) {
    $childs[] = $component['extra']['justificatif'];
  }
  $return[] = array("data" => $component['name'], "children" => $childs);

  return array(
    "message" => $return,
    "note" => $score,
    "max_score" => $max_score,
  );
}

/**
 * Module specific instance of hook_help().
 */
function _dquarks_help_multichoice($section) {
  switch ($section) {
    case 'admin/settings/dquarks#multichoice_description':
      return t('Allows creation of checkboxes, radio buttons, or select menus.');
  }
}

/**
 * Calculate and returns statistics about results for this component.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema
 * @param array $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will
 * be limited to these sids.
 *
 * @return array
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _dquarks_analysis_rows_multichoice($component, $sids = array()) {
  $options = _dquarks_multichoice_options($component['extra']['items'], TRUE);
  $sidfilter = count($sids) ? " AND sid IN :sids " : "";
  $result = db_query("SELECT data, count(data) as datacount FROM {dquarks_submitted_data} WHERE nid = :nid AND cid = :cid AND data <> '0' AND data <>''" . $sidfilter . "GROUP BY data", array(
    ':nid' => $component['nid'],
    ':cid' => $component['cid'],
    ':sids' => $sids)
  );

  $rows = array();
  foreach ($result as $data) {
    if (isset($options[$data->data])) {
      $display_option = $options[$data->data];
    }
    else {
      $display_option = $data->data;
    }
    $rows[] = array($display_option, $data->datacount);
  }
  return $rows;
}

/**
 * Return the result of this component's submission for display in a table.
 *
 * @param array $data
 *   An array of information containing the submission result, directly
 *   correlating to the dquarks_submitted_data database schema
 *
 * @return string
 *   Textual output formatted for human reading.
 */
function _dquarks_table_data_multichoice($data) {
  $output = '';
  // Set the value as a single string.
  if (is_array($data['value'])) {
    foreach ($data['value'] as $value) {
      if ($value !== '0') {
        $output .= check_plain($value) . '<br />';
      }
    }
  }
  else {
    $output .= check_plain(empty($data['value']['0']) ? '' : $data['value']['0']);
  }
  return $output;
}

/**
 * Return the header information for this component to be displayed.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 *
 * @return array
 *   An array of data to be displayed in the first three rows of a Excel file,
 *   not including either prefixed or trailing commas.
 */
function _dquarks_xlsx_headers_multichoice($component) {
  $headers = array(
    array(array('colspan' => 0, 'value' => '')),
    array(array('colspan' => 0, 'value' => '')),
    array(array('colspan' => 0, 'value' => $component['name'])),
  );
  return $headers;
}

/**
 * Return the result of a textfield submission.
 *
 * @param array $data
 *   An array of information containing the submission result, directly
 * correlating to the dquarks_submitted_data database schema.
 * @param array $component
 *   The component of node.
 *
 * @return array
 *   Textual output formatted for Excel, not including either prefixed or
 * trailing commas.
 */
function _dquarks_xlsx_data_multichoice($data, $component) {
  $return = array();
  $output = '';
  // Set the value as a single string.
  if (is_array($data['value'])) {
    foreach ($data['value'] as $value) {
      if ($value !== '0') {
        $output .= check_plain($value) . '; ';
      }
    }
    $output = drupal_substr($output, 0, drupal_strlen($output) - 2);
  }
  else {
    $output .= check_plain(empty($data['value']['0']) ? '' : $data['value']['0']);
  }
  $return[] = array('colspan' => 0, 'value' => $output);
  return $return;
}

/**
 * Return the labels of multichoice component.
 *
 * @param array $items
 *   Items of multichoice component.
 *
 * @return Array
 *   Return the labels of items
 */
function _dquarks_multichoice_options($items) {
  return $items["labels"];
}

/**
 * Format the text output to be included in the mail.
 *
 * @param array $variables
 *   Contains the data and the component values.
 *
 * @return string
 *   Generate a themed output
 */
function theme_dquarks_mail_multichoice($variables) {

  $data = $variables['data'];
  $component = $variables['component'];
  $childs = array();
  $options = $component['extra']['items']["labels"];
  $wrong = FALSE;
  $corrects = array_intersect_key($component['extra']['items']["labels"], array_filter($component['extra']['items']["choices"]));
  if (is_array($data)) {
    foreach ($data as $key => $value) {
      if ($value != '') {
        if ($component['extra']['items']["choices"][$key]) {
          $childs[] = t("Correct answer :") . ' ' . $options[$key];
        }
        else {
          $childs[] = t("Wrong answer :") . ' ' . $options[$key];
          $wrong = TRUE;
        }
        $data[$key] = $options[$key];
      }
      else {
        unset($data[$key]);
      }
    }
  }
  else {
    if ($component['extra']['items']["choices"][$data]) {
      $childs[] = t("Correct answer :") . ' ' . $options[$data];
    }
    else {
      $childs[] = t("Wrong answer :") . ' ' . $options[$data];
      $wrong = TRUE;
    }
    $data = array($data => $options[$data]);
  }
  if ($wrong) {
    if (count($corrects)) {
      $childs[] = array(
        "data" => t("Correct answers are :"),
        "children" => $corrects,
      );
    }
    else {
      $childs[] = array(
        "data" => t("The Correct answer is :"),
        "children" => $corrects,
      );
    }
  }
  if (!empty($component['extra']['justificatif'])) {
    $childs[] = $component['extra']['justificatif'];
  }
  $return[] = array("data" => $component['name'], "children" => $childs);
  return theme("item_list", array(
            'title' => $component['name'],
            'items' => $childs)
  );
}
