<?php

/**
 * @file
 * Handles the management page for Speed_form_blocks module.
 */

/**
 * Does the 'admin/structure/speed_form_blocks/management' page.
 */
function speed_form_blocks_manage_blocks() {

  $query = db_query('SELECT bid, block_name, extra, button, 
    message, pre_html, post_html, class 
    FROM {speed_form_blocks_created} 
    WHERE block_name = :block_name 
    LIMIT 1',
    array(':block_name' => arg(4)));

  $block = $query->fetch();
  
  if (!empty($block)) {

    $form["blockname"] = array(
      '#type' => 'hidden',
      '#value' => $block->block_name,
    );

    $form["newname"] = array(
      '#type' => 'machine_name',
      '#title' => t("New name for this block. This block needs to be a machine name"),
      '#default_value' => $block->block_name,
      '#machine_name' => array(

        'exists' => 'speed_form_blocks_management_name_exists',

      ),
    );

    $form["display"] = array(
      '#markup' => '<strong>' . t('The following are fields used in this form') . '</strong>',
    );

    $data = unserialize($block->extra);
    $size = count($data);

    if ($size != 0) {

      $form["extra"] = array(

        '#type'  => 'fieldset',
        '#title' => t('%block_name existing fields', array('%block_name' => $block->block_name)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,

      );

    }

    for ($x = 0; $x < $size; $x++) {

      $form["extra"][$x]['type'] = array(
        '#type' => 'select',
        '#title' => t("Selected type"),
        '#description' => t("The input type (in HTML) for this form element"),
        '#options' => array(
          0 => t('checkbox'),
          1 => t('date'),
          2 => t('text_format'),
          3 => t('textarea'),
          4 => t('textfield'),
        ),
        '#default_value' => $data[$x]['type'],
      );

      $form["extra"][$x]['name'] = array(
        '#type'  => 'textfield',
        '#size'  => 100,
        '#title' => t("Name for this form element"),
        '#default_value' => $data[$x]['name'],
        '#element_validate' => array('_speed_form_blocks_management_element_no_space'),
        '#description' => t("This field can't contain any whitespace."),
        '#required' => TRUE,
      );

      $form["extra"][$x]['desc'] = array(
        '#type'  => 'textfield',
        '#size'  => 100,
        '#title' => t("User prompt for this element"),
        '#default_value' => $data[$x]['desc'],
        '#required' => TRUE,
      );

      $form["extra"][$x]['delete'] = array(
        '#type'  => 'checkbox',
        '#title' => t("Delete this field"),
      );

    }

    /*
     * New fields
     */

    $form["new"] = array(

      '#type' => 'fieldset',
      '#title' => t("Add more fields (click to expand)"),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,

    );

    $form["new"]['type'] = array(
      '#type'  => 'select',
      '#title' => t("Selected type"),
      '#description' => t("The input type (in HTML) you wish to add to this form"),
      '#options' => array(
        0 => t('checkbox'),
        1 => t('date'),
        2 => t('text_format'),
        3 => t('textarea'),
        4 => t('textfield'),
      ),
    );

    $form["new"]['name'] = array(
      '#type'  => 'textfield',
      '#size'  => 100,
      '#title' => t("Name for this form element"),
      '#element_validate' => array('_speed_form_blocks_management_element_no_space'),
      '#description' => t("This field can't contain any whitespace."),
    );

    $form["new"]['desc'] = array(
      '#type'  => 'textfield',
      '#size'  => 100,
      '#title' => t("User prompt for this element"),
    );

    /*
     * Other fields
     */

    $form["other"] = array(

      '#type'  => 'fieldset',
      '#title' => t('%block_name Other settings (click to minimise)', array('%block_name' => $block->block_name)),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,

    );

    $form["other"]["pre_html"] = array(
      '#type'  => 'textarea',
      '#title' => t("HTML to display before the form"),
      '#default_value' => $block->pre_html,
      '#description' => t("HTML to show before the form"),
    );

    $form["other"]["button"] = array(
      '#type'  => 'textfield',
      '#title' => t("Button label"),
      '#default_value' => $block->button,
      '#description' => t("This text that will appear on the form 'submit' button for this block"),
    );

    $form["other"]["message"] = array(
      '#type'  => 'textarea',
      '#title' => t("Message post submission"),
      '#default_value' => $block->message,
      '#description' => t("This message will appear after a form is submitted"),
    );

    $form["other"]["post_html"] = array(
      '#type'  => 'textarea',
      '#title' => t("HTML to display after the form"),
      '#default_value' => $block->post_html,
      '#description' => "HTML to show after the form",
    );

    $form["other"]["class"] = array(
      '#type'  => 'textfield',
      '#title' => t("CSS class for the form"),
      '#default_value' => $block->class,
      '#description' => "HTML Class for the form",
    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('speed_form_blocks_management_handler'),
    );

    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete this block'),
      '#submit' => array('speed_form_blocks_deletion_handler'),
    );

    $form["bid"] = array(
      '#type'  => 'hidden',
      '#value' => $block->bid,
    );

    }
    else {

      $form["display"] = array(
        '#markup' => t('No block with this name exists (it may have just been deleted).'),
      );

    }

  return $form;

}

/**
 * Does the submission the management data when saved.
 */
function speed_form_blocks_deletion_handler($form, &$form_state) {

  $num_deleted = db_delete('speed_form_blocks_created')
    ->condition('bid', $form_state['values']['bid'])
    ->execute();

  drupal_set_message(t("Block %block_name has been deleted.", array("%block_name" => $form_state['values']['blockname'])), 'status');

}

/**
 * Does the submission the management data when saved.
 */
function speed_form_blocks_management_handler($form, &$form_state) {

  db_update('speed_form_blocks_created')
    ->condition('bid', $form_state['values']['bid'])
    ->fields(
     array(
        'block_name' => $form_state['values']['newname'],
        'button' => $form_state['values']['other']['button'],
        'message' => $form_state['values']['other']['message'],
        'pre_html' => $form_state['values']['other']['pre_html'],
        'post_html' => $form_state['values']['other']['post_html'],
        'class' => $form_state['values']['other']['class'],
        )
      )
    ->execute();

  $extra = array();

  if (isset($form_state['values']['extra'])) {

    foreach ($form_state['values']['extra'] as $form_element => $form_element_data) {
 
      if ($form_element_data['delete']!=1) {

        array_push($extra, $form_element_data);

      }
      else {

        drupal_set_message(t("Form element deleted."), 'status');

      }

    }

  }

  if ($form_state['values']["new"]['name']!="") {

    drupal_set_message(t("Form element added."), 'status');

    array_push($extra, $form_state['values']['new']);

  }

  if (drupal_strlen(serialize($extra)) < 500) {

    db_update('speed_form_blocks_created')
      ->condition('bid', $form_state['values']['bid'])
      ->fields(array('extra' => serialize($extra)))
      ->execute();

    drupal_set_message(t("Block Updated"), 'status');

  }
  else {

    drupal_set_message(t("Other Settings (button label, post submission message etc.) are too long"), 'error');

  }

}

/**
 * Does the checking to see if a block of this name already exists.
 *
 * @param string $value_to_check
 *   suggested machine name for the block.
 *
 * @return boolean
 *   TRUE is block exists, and FALSE if not.
 */
function speed_form_blocks_management_name_exists($value_to_check) {

  $query = db_query('SELECT n.block_name 
  FROM {speed_form_blocks_created} n 
  WHERE n.block_name = :block_name 
  LIMIT 1',
  array(':block_name' => $value_to_check));

  $exists = $query->fetch();

  if ($exists) {

    drupal_set_message(t("Block name %block_name is already in use. Please choose another name", array("%block_name" => $value_to_check)), 'error');
    return TRUE;

  }

  return FALSE;

}

/**
 * Checks to see if a form name element contains whitespace.
 *
 * @param string $value_to_check
 *   suggested machine name for the block.
 */
function _speed_form_blocks_management_element_no_space($value_to_check) {

  if (strpos($value_to_check['#value'], " ") !== FALSE) {

    form_error($value_to_check, t("This cannot contain whitespace"));

  }

}