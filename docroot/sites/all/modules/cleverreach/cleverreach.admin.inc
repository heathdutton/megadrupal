<?php
/**
 * @file
 * This file contains admin functions.
 */

/**
 * Show admin overview page.
 */
function _cleverreach_admin_overview() {
  $group_row = array();
  $results = db_query('SELECT * FROM {cleverreach_groups} g');

  foreach ($results as $result) {
    $group_row[] = array(
      $result->crgid,
      $result->name,
      $result->active_count,
      $result->inactive_count,
      date("Y-m-d H:i:s", $result->last_mailing),
      date("Y-m-d H:i:s", $result->last_changed),
    );
  }

  $group_rows = $group_row;
  $group_header = array(
    t('CR GroupID'),
    t('Name'),
    t('Active Count'),
    t('Inactive Count'),
    t('Last mailing'),
    t('Last changed'),
  );
  $group_table = theme('table', array(
    'header' => $group_header,
    'rows' => $group_rows,
  ));
  $block_row = array();
  $results = db_query('SELECT * FROM {cleverreach_block_forms} bf');

  foreach ($results as $result) {
    $attr = '';
    $name = _cleverreach_get_group_name($result->listid);
    $active = ($result->active == 1) ? t('Yes') : t('No');
    $fields = unserialize($result->fields);

    if (count($fields) > 0) {
      foreach ($fields as $value) {

        if ($value["active"] == 1) {
          $attr .= $value["name"] . ", ";
        }

      }

      $attr = substr($attr, 0, -2);
    }

    else {
      $attr = '-';
    }

    $block_row[] = array(
      $name,
      $attr,
      $active,
      l(t('Edit'), 'admin/config/services/cleverreach/editblock/' . $result->bid)
      . " | " .
      l(t('Delete'), 'admin/config/services/cleverreach/delblock/' . $result->bid),
    );
  }

  $block_rows = $block_row;
  $block_header = array(
    t('Group'),
    t('Attributes'),
    t('Active'),
    t('Options'),
  );
  $block_table = theme('table', array(
    'header' => $block_header,
    'rows' => $block_rows,
  ));
  $output = theme_fieldset(array('element' => array('#title' => t('Blocks'), '#children' => $block_table)));
  $output .= theme_fieldset(array('element' => array('#title' => t('Groups'), '#children' => $group_table)));
  return $output;
}

/**
 * Implements hook_form().
 */
function _cleverreach_add_block_form($form, &$form_state) {
  $form = array();
  $form['#tree'] = TRUE;
  $options = array();
  $results = db_query('SELECT * FROM {cleverreach_groups} g');

  foreach ($results as $result) {
    $options[$result->crgid] = $result->name;
  }

  $form['cr_grp_wrapper'] = array(
    '#type' => 'fieldset',
    '#theme' => 'addblock_theming',
    '#prefix' => '<div id="cr-group-attribute-wrapper">',
    '#suffix' => '</div>',
  );
  $form['cr_grp_wrapper']['cr_block_grp'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Select a group'),
    '#description' => t('Select the cleverreach group'),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_cleverreach_add_block_form_js',
      'wrapper' => 'cr-group-attribute-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  if (isset($form_state['values']['cr_grp_wrapper']['cr_block_grp'])) {
    $gid = is_numeric($form_state['values']['cr_grp_wrapper']['cr_block_grp']) ? $form_state['values']['cr_grp_wrapper']['cr_block_grp'] : NULL;

    if ($gid != NULL) {
      $attributes = db_query('SELECT grps.attributes FROM {cleverreach_groups} grps WHERE grps.crgid = :crgid', array(':crgid' => $gid))->fetchField();

      foreach (unserialize($attributes) as $value) {

        if ($value["type"] == "text") {
          $display_select = array(
            'textfield' => t('Textfield'),
            'select' => t('Select box'),
          );
        }

        elseif ($value["type"] == "number") {
          $display_select = array(
            'textfield' => t('Textfield'),
          );
        }

        elseif ($value["type"] == "gender") {
          $display_select = array(
            'select' => t('Select box'),
          );
        }

        elseif ($value["type"] == "date") {
          $display_select = array(
            'date' => t('Date select box'),
          );
        }

        $form['cr_grp_wrapper']['table'][$value["key"]]['select'] = array(
          '#type' => 'checkbox',
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['required'] = array(
          '#type' => 'checkbox',
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['var'] = array(
          '#type' => 'hidden',
          '#value' => $value["variable"],
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['type'] = array(
          '#type' => 'hidden',
          '#value' => $value["type"],
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['display'] = array(
          '#type' => 'select',
          '#options' => $display_select,
          '#default_value' => key($display_select),
          '#ajax' => array(
            'callback' => '_cleverreach_add_block_form_js',
            'wrapper' => 'cr-group-attribute-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
          ),
        );

        if (($form['cr_grp_wrapper']['table'][$value["key"]]['display']['#default_value'] == 'select') || isset($form_state['values']['cr_grp_wrapper']) && $form_state['values']['cr_grp_wrapper']['table'][$value["key"]]['display'] == 'select') {
          $form['cr_grp_wrapper']['table'][$value["key"]]['display_options'] = array(
            '#type' => 'textarea',
            '#description' => t('Select options. One per line. key|value Example: men|Men'),
          );
        }

        elseif (($form['cr_grp_wrapper']['table'][$value["key"]]['display']['#default_value'] == 'textfield') || isset($form_state['values']['cr_grp_wrapper']) && $form_state['values']['cr_grp_wrapper']['table'][$value["key"]]['display'] == 'textfield') {
          $form['cr_grp_wrapper']['table'][$value["key"]]['display_options'] = array(
            '#type' => 'textfield',
            '#description' => t('A default field value (optional)'),
          );
        }

        elseif (($form['cr_grp_wrapper']['table'][$value["key"]]['display']['#default_value'] == 'date')|| isset($form_state['values']['cr_grp_wrapper']) && $form_state['values']['cr_grp_wrapper']['table'][$value["key"]]['display'] == 'date') {
          $form['cr_grp_wrapper']['table'][$value["key"]]['display_options'] = array(
            '#type' => 'textfield',
            '#default_value' => 'Y-m-d',
            '#disabled' => TRUE,
            '#description' => t('Date format, default: Y-m-d'),
          );
        }

      }

    }

  }

  $form['cr_block_status'] = array(
    '#type' => 'radios',
    '#title' => t('Active Block'),
    '#default_value' => 1,
    '#description' => t('Should this block be active and usable on the system block page?'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#weight' => 2,
  );
  $form['cr_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 3,
    '#submit' => array('_cleverreach_add_block_form_submit'),
  );
  return $form;
}

/**
 * Theming function for addblock table with multiple checkboxes.
 */
function theme_addblock_theming($variables) {
  $output = '';
  $output .= drupal_render($variables['element']['cr_block_grp']);

  if (isset($variables['element']['table'])) {
    $rows = array();
    $header = array(
      '',
      t('Name'),
      t('Type'),
      t('Variable'),
      t('Required'),
      t('Display'),
    );

    foreach ($variables['element']['table'] as $key => $value) {

      if (substr($key, 0, 1) != '#' && is_array($value)) {
        $rows[] = array(
          drupal_render($value["select"]),
          $key,
          $value["type"]["#value"],
          $value["var"]["#value"],
          drupal_render($value["required"]),
          drupal_render($value["display"]) . drupal_render($value["display_options"]),
        );
      }

    }

    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'caption' => t('Select Fields'),
    ));
  }

  return $output;
}

/**
 * Implements hook_form_submit().
 */
function _cleverreach_add_block_form_submit($form, &$form_state) {
  $fields = array();

  foreach ($form_state['values']['cr_grp_wrapper']['table'] as $key => $value) {
    $fields[] = array(
      'name' => $key,
      'required' => $value["required"],
      'display' => $value["display"],
      'display_options' => $value["display_options"],
      'active' => $value["select"],
    );
  }

  $query = db_insert('cleverreach_block_forms')->fields(array(
    'listid',
    'fields',
    'active',
  ));
  $query->values(array(
    $form_state["values"]["cr_grp_wrapper"]["cr_block_grp"],
    serialize($fields),
    $form_state["values"]["cr_block_status"],
  ));
  $query->execute();
  drupal_set_message(t('Block added successfully.'));
  drupal_goto('admin/config/services/cleverreach');
}

/**
 * AJAX Callback function.
 */
function _cleverreach_add_block_form_js($form, &$form_state) {
  return $form['cr_grp_wrapper'];
}

/**
 * Implements hook_form().
 */
function _cleverreach_edit_block_form($form, &$form_state, $arg) {
  $bid = (is_numeric($arg) && isset($arg)) ? $arg : 0;
  $form_state['storage']['bid'] = $bid;
  $data = db_query('SELECT * FROM {cleverreach_block_forms} bf WHERE bf.bid = :bid', array(':bid' => $bid))->fetchAssoc();
  $form = array();
  $form['#tree'] = TRUE;
  $options = array();
  $results = db_query('SELECT * FROM {cleverreach_groups} g');
  $fields = unserialize($data["fields"]);
  $display_option_values = array();

  foreach ($results as $result) {
    $options[$result->crgid] = $result->name;
  }

  $form['cr_grp_wrapper'] = array(
    '#type' => 'fieldset',
    '#theme' => 'addblock_theming',
    '#prefix' => '<div id="cr-group-attribute-wrapper">',
    '#suffix' => '</div>',
  );
  $form['cr_grp_wrapper']['cr_block_grp'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Select a group'),
    '#description' => t('Select the cleverreach group'),
    '#default_value' => $data["listid"],
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_cleverreach_add_block_form_js',
      'wrapper' => 'cr-group-attribute-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  if (isset($form_state['values']['cr_grp_wrapper']['cr_block_grp']) || isset($data["listid"])) {
    $gid = (isset($form_state['values']['cr_grp_wrapper']['cr_block_grp']) && is_numeric($form_state['values']['cr_grp_wrapper']['cr_block_grp'])) ? $form_state['values']['cr_grp_wrapper']['cr_block_grp'] : $data["listid"];

    if ($gid != NULL) {
      $attributes = db_query('SELECT grps.attributes FROM {cleverreach_groups} grps WHERE grps.crgid = :crgid', array(':crgid' => $gid))->fetchField();

      foreach (unserialize($attributes) as $value) {
        $display_options = '';
        $selected = 0;
        $required = 0;

        foreach ($fields as $valued) {

          if ($value["key"] == $valued["name"]) {

            if ($valued["active"] == 1) {
              $selected = 1;
            }

            if ($valued["required"] == 1) {
              $required = 1;
            }

            if ($valued["display"] == 'select') {
              $display_options = 'select';
            }

            elseif ($valued["display"] == 'textfield') {
              $display_options = 'textfield';
            }

            elseif ($valued["display"] == 'date') {
              $display_options = 'date';
            }

            else {
              $display_options = 'textfield';
            }

          }

          $display_option_values[$valued["name"]] = $valued["display_options"];

        }

        if ($value["type"] == "text") {
          $display_select = array(
            'textfield' => t('Textfield'),
            'select' => t('Select box'),
          );
        }

        elseif ($value["type"] == "number") {
          $display_select = array(
            'textfield' => t('Textfield'),
          );
        }

        elseif ($value["type"] == "gender") {
          $display_select = array(
            'select' => t('Select box'),
          );
        }

        elseif ($value["type"] == "date") {
          $display_select = array(
            'date' => t('Date select box'),
          );
        }

        $form['cr_grp_wrapper']['table'][$value["key"]]['select'] = array(
          '#type' => 'checkbox',
          '#default_value' => $selected,
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['required'] = array(
          '#type' => 'checkbox',
          '#default_value' => $required,
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['var'] = array(
          '#type' => 'hidden',
          '#value' => $value["variable"],
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['type'] = array(
          '#type' => 'hidden',
          '#value' => $value["type"],
        );
        $form['cr_grp_wrapper']['table'][$value["key"]]['display'] = array(
          '#type' => 'select',
          '#options' => $display_select,
          '#default_value' => (isset($display_options) && !empty($display_options)) ? $display_options : key($display_select),
          '#ajax' => array(
            'callback' => '_cleverreach_add_block_form_js',
            'wrapper' => 'cr-group-attribute-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
          ),
        );

        $display = isset($form_state['values']['cr_grp_wrapper']['table'][$value["key"]]['display']) ? $form_state['values']['cr_grp_wrapper']['table'][$value["key"]]['display'] : $display_options;

        if (empty($display)) {
          $display = key($display_select);
        }

        if ($display == 'select') {
          $form['cr_grp_wrapper']['table'][$value["key"]]['display_options'] = array(
            '#type' => 'textarea',
            '#description' => t('Select options. One per line. key|value Example: men|Men'),
            '#default_value' => isset($display_option_values[$value["key"]]) ? $display_option_values[$value["key"]] : '',
          );
        }

        elseif ($display == 'textfield') {
          $form['cr_grp_wrapper']['table'][$value["key"]]['display_options'] = array(
            '#type' => 'textfield',
            '#description' => t('A default field value (optional)'),
            '#default_value' => isset($display_option_values[$value["key"]]) ? $display_option_values[$value["key"]] : '',
          );
        }

        elseif ($display == 'date') {
          $form['cr_grp_wrapper']['table'][$value["key"]]['display_options'] = array(
            '#type' => 'textfield',
            '#default_value' => isset($display_option_values[$value["key"]]) ? $display_option_values[$value["key"]] : 'Y-m-d',
            '#disabled' => TRUE,
            '#description' => t('Date format, default: Y-m-d'),
          );
        }

      }

    }

  }

  $form['cr_block_status'] = array(
    '#type' => 'radios',
    '#title' => t('Active Block'),
    '#default_value' => $data["active"],
    '#description' => t('Should this block be active and usable on the system block page?'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#weight' => 2,
  );
  $form['cr_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 3,
    '#submit' => array('_cleverreach_edit_block_form_submit'),
  );
  return $form;
}

/**
 * Implements hook_form_submit().
 */
function _cleverreach_edit_block_form_submit($form, &$form_state) {
  $bid = $form_state['storage']['bid'];
  $fields = array();

  foreach ($form_state['values']['cr_grp_wrapper']['table'] as $key => $value) {
    $fields[] = array(
      'name' => $key,
      'required' => $value["required"],
      'display' => $value["display"],
      'display_options' => $value["display_options"],
      'active' => $value["select"],
    );
  }

  db_update('cleverreach_block_forms')
    ->fields(array(
      'listid' => $form_state["values"]["cr_grp_wrapper"]["cr_block_grp"],
      'fields' => serialize($fields),
      'active' => $form_state["values"]["cr_block_status"],
    ))
    ->condition("bid", $bid)
    ->execute();
  drupal_set_message(t('Block updated successfully.'));
  drupal_goto('admin/config/services/cleverreach');
}

/**
 * Implements hook_form().
 */
function _cleverreach_del_block_form($form, &$form_state, $arg) {
  $form = array();
  $arg = isset($arg) ? $arg : 0;
  $form['bid'] = array(
    '#type' => 'hidden',
    '#value' => $arg,
  );
  return confirm_form($form, t('Are you sure you want to delete this block?'), 'admin/config/services/cleverreach', t('This action cannot be undone.'), t('Delete'));
}

/**
 * Implements hook_form_submit().
 */
function _cleverreach_del_block_form_submit($form, &$form_state) {

  if ($form_state['values']['bid'] != 0) {
    $delta = 'cr_block_' . $form_state['values']['bid'];
    db_delete('cleverreach_block_forms')
      ->condition('bid', $form_state['values']['bid'], '=')
      ->execute();
    db_delete('block')
      ->condition('module', 'cleverreach')
      ->condition('delta', $delta)
      ->execute();
    db_delete('block_role')
      ->condition('module', 'cleverreach')
      ->condition('delta', $delta)
      ->execute();
    cache_clear_all();
    drupal_set_message(t('Block deleted successfully.'));
    drupal_goto('admin/config/services/cleverreach');
  }
  else {
    drupal_goto('admin/config/services/cleverreach');
  }

}

/**
 * The admin settings (api details) system form.
 */
function _cleverreach_admin_settings() {
  $form = array();
  $form['api_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('CleverReach API Details'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['api_details']['cleverreach_api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('cleverreach_api_key', ''),
    '#title' => t('Your CleverReach API-Key'),
    '#description' => t('Enter your personal CleverReach API-Key.'),
  );
  $form['api_details']['cleverreach_wsdl_url'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('cleverreach_wsdl_url', 'http://api.cleverreach.com/soap/interface_v5.1.php?wsdl'),
    '#title' => t('CleverReach WSDL-URL'),
    '#description' => t('Enter the CleverReach WSDL-URL.'),
  );
  $form['grp_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('CleverReach Group Sync'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $timestamp = variable_get('cleverreach_last_group_fetch', '');
  $last_update = !empty($timestamp) ? date("Y-m-d H:i:s", $timestamp) : t('never');
  $form['grp_details']['last_update'] = array(
    '#markup' => t('Last group update: @date', array('@date' => $last_update)) . "<br />",
  );
  $form['grp_details']['update_button'] = array(
    '#type' => 'button',
    '#value' => t('Fetch groups now'),
    '#limit_validation_errors' => array(),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_cleverreach_group_update'),
  );
  return system_settings_form($form);
}
