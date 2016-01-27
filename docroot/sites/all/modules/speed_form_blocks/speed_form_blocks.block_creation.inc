<?php

/**
 * @file
 * Handles the block creation page for Speed_form_blocks module.
 */

/**
 * Does the generation of the Block Creation Admin Page.
 */
function speed_form_blocks_creation($form, $form_state) {

  $form['speed_form_blocks_feedback_block_creation'] = array(
    '#markup' => '<h3>' . t('Speed form block creation') . '</h3>',
  );

  if (empty($form_state['storage']['new_block_machine_name'])) {

    $form["new_block_machine_name"] = array(
      '#type' => 'machine_name',
      '#title' => t("Enter the name you'd like for this block"),
      '#description' => t("This is the name of your block in Structure | Blocks. The name must contain only lowercase letters, numbers, and underscores (a machine name)"),
      '#machine_name' => array(

        'exists' => 'speed_form_blocks_creation_name_exists',

      ),

    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Start block creation'),
      '#submit' => array('speed_form_blocks_add_field_handler'),
    );

  }
  else {

    $form["new_block_machine_name"] = array(
      '#type' => 'machine_name',
      '#title' => t("Current Block Name"),
      '#description' => t("You may change the name of this block. It should not contain any white space"),
      '#machine_name' => array(

        'exists' => 'speed_form_blocks_creation_name_exists',

      ),
      '#required' => TRUE,
      '#default_value' => $form_state['storage']["new_block_machine_name"],

    );

    if (isset($form_state['storage']['form_elements'])) {

      $size = count($form_state['storage']['form_elements']);

      for ($x = 0; $x < $size; $x++) {

        $form['temp_' . $x . '_type'] = array(
          '#type' => 'select',
          '#title' => t("Selected type"),
          '#description' => t("The input type (in HTML) you wish to add to this form"),
          '#options' => array(
            0 => t('checkbox'),
            1 => t('date'),
            2 => t('text_format'),
            3 => t('textarea'),
            4 => t('textfield'),
          ),
          '#default_value' => $form_state['storage']['form_elements'][$x][0],
        );

        $form['temp_' . $x . '_name'] = array(
          '#type' => 'textfield',
          '#title' => t("Name for this form element"),
          '#default_value' => $form_state['storage']['form_elements'][$x][1],
          '#description' => t("You may change the name of this block. It should not contain any white space"),
          '#required' => TRUE,
        );

        $form['temp_' . $x . '_desc'] = array(
          '#type' => 'textarea',
          '#title' => t("User prompt for this element"),
          '#default_value' => $form_state['storage']['form_elements'][$x][2],
          '#required' => TRUE,
        );

      }

    }

    if (isset($form_state['storage'])) {

      if (count($form_state['storage']) >= 2) {

        $form["prompt"] = array(
          '#markup' => '<h3>' . t('Add a new field, or edit the field above') . '</h3>',
        );

      }
      else {

        $form["prompt"] = array(
          '#markup' => '<h3>' . t('Add a new field') . '</h3>',
        );

      }

    }

    $form['new_field_select'] = array(
      '#type' => 'select',
      '#title' => t("Choose a new field type"),
      '#description' => t("The input type (in HTML) you wish to add to this form"),
      '#options' => array(
        0 => t('checkbox'),
        1 => t('date'),
        2 => t('text_format'),
        3 => t('textarea'),
        4 => t('textfield'),
      ),
      '#default_value' => 0,
    );

    $form["new_name"] = array(
      '#type' => 'textfield',
      '#title' => t("Name for this form element"),
      '#description' => t("This should contain no white space"),
      '#default_value' => t('Enter name here'),
      '#element_validate' => array('_speed_form_blocks_element_no_space'),
      '#required' => TRUE,
    );

    $form["new_description"] = array(
      '#type' => 'textarea',
      '#title' => t("User guidance for this field"),
      '#default_value' => t('Enter User prompt'),
      '#required' => TRUE,
    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Add this field'),
      '#submit' => array('speed_form_blocks_add_field_handler'),
    );

  }

  if (isset($form_state['storage'])) {

    if (count($form_state['storage']) >= 2) {

      $form['final'] = array(
        '#type' => 'submit',
        '#value' => t('Finish Block (no fields will be added)'),
        '#submit' => array('speed_form_blocks_creation_finalise'),
      );

    }

  }

  return $form;

}

/**
 * Checks to see if a form name element contains whitespace.
 *
 * @param string $value_to_check
 *   suggested machine name for the block.
 */
function _speed_form_blocks_element_no_space($value_to_check) {

  if (strpos($value_to_check['#value'], " ") !== FALSE) {

    form_error($value_to_check, t("This cannot contain whitespace"));

  }

}

/**
 * Checks to see if a block of this name already exists.
 *
 * @param string $value_to_check
 *   suggested machine name for the block.
 */
function speed_form_blocks_creation_name_exists($value_to_check) {

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
 * Does the submission of the block and fields into the database.
 */
function speed_form_blocks_creation_finalise($form, &$form_state) {

  $insert_data = array();
  $post_keys = array_keys($form_state['values']);

  for ($x = 0; $x < count($post_keys); $x++) {

    if (strpos($post_keys[$x], "temp_") !== FALSE) {

      $value = explode("_", $post_keys[$x]);
      $insert_data[$value[1]][$value[2]] = $form_state['values']['temp_' . $value[1] . '_' . $value[2]];

    }

  }

  $record = array(
    'block_name' => $form_state['values']['new_block_machine_name'],
    'extra' => serialize($insert_data),
  );

  drupal_write_record('speed_form_blocks_created', $record);
  drupal_set_message(t("Block created. The block can be managed on the 'Block Management' tab. You should use the block management tab before making this block 'active'."), 'status');

}

/**
 * Does the process of adding a new form element to the form.
 */
function speed_form_blocks_add_field_handler($form, &$form_state) {

  $form_state['rebuild'] = TRUE;

  if (empty($form_state['storage'])) {

    drupal_set_message(t("Block creation started. Please now add fields"), 'status');

  }
  else {

    if (is_numeric($form_state['values']['new_field_select'])) {

      $form_element = $form_state['values']['new_field_select'];

      if (is_string($form_state['values']['new_name']) && is_string($form_state['values']['new_description'])) {

        $name = $form_state['values']['new_name'];

        $description = $form_state['values']['new_description'];

        if (!isset($form_state['storage']['form_elements'])) {

          $form_state['storage']['form_elements'] = array(
            array(
              $form_element,
              $name,
              $description,
            ),
          );

        }
        else {

          drupal_set_message(t("New Element added. Please continue adding fields or save"), 'status');

          array_push($form_state['storage']['form_elements'], array(
              $form_element,
              $name,
              $description,
            )
          );

        }

      }

    }

  }

  $form_state['storage']["new_block_machine_name"] = $form_state['values']['new_block_machine_name'];

}
