<?php

/**
 * @file
 * Handles the management page for Speed_form_blocks module.
 */

/**
 * Generates a list of all available blocks.
 */
function logic_block_list_blocks() {

  $result = db_query('SELECT block_name, logic from {logic_block_blocks_created}');

  $form['intro'] = array(
    '#markup' => '<h3>' . t('Block Logic blocks created') . '</h3>',
  );

  $form['logic_block'] = array(
    '#prefix' => '<div id="logic_block_built">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(t('Block Logic types'), t('Actions available for this type')),
    '#rows' => array(),
  );

  $counter = 0;

  foreach ($result as $block) {

    $counter++;

    $block_logic = unserialize($block->logic);

    $logic = array(
      '#markup' => $block->block_name . " ( " . str_replace("_", " ", str_replace(".inc", "", $block_logic[0])) . " )" ,
    );

    $actions = array(
      '#markup' => l(t('Edit'), 'admin/structure/logic_block/manage/' . $block->block_name . '/edit') . ' | '
      . l(t('Delete'), 'admin/structure/logic_block/manage/' . $block->block_name . '/delete') . ' | '
      . l(t('Duplicate'), 'admin/structure/logic_block/manage/' . $block->block_name . '/duplicate'),
    );

    $form['logic_block'][] = array(
      'fname' => &$logic,
      'sname' => &$actions,
    );

    $form['logic_block']['#rows'][] = array(
      array('data' => &$logic),
      array('data' => &$actions),
    );

    unset($logic);
    unset($actions);

  }

  if ($counter == 0) {

    unset($form['logic_block']);
    $form['empty'] = array(
      '#markup' => '<p>' . t('No Block Logic blocks created') . '</p>',
    );

    $form['link'] = array(
      '#markup' => l(t('Create a block logic block'), 'admin/structure/logic_block/create'),
    );

  }

  return $form;

}

/**
 * Toggles the option for each block depending on the arg array.
 */
function logic_block_manage() {

  if (count(arg()) == 4) {

    return logic_block_list_blocks();

  }
  else {

    switch (arg(5)) {

      case "edit":
      return logic_block_edit_block();
      break;

      case "duplicate":
      return logic_block_duplicate_block();
      break;

      case "delete":
      return logic_block_delete_block();
      break;

      default:
        break;
    }

  }

}

/**
 * Does the prompt page before deleting a block.
 */
function logic_block_delete_block() {

  $form['intro'] = array(
    '#markup' => '<h3>' . t('Are you sure you want to delete :block_name ?', array(':block_name' => arg(4))) . '</h3>',
  );

  $form['delete'] = array(
    '#type' => 'hidden',
    '#value' => arg(4),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Block'),
    '#submit' => array('logic_block_delete_block_handler'),
  );

  return $form;

}

/**
 * Does the block deletion.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form array.
 */
function logic_block_delete_block_handler($form, $form_state) {

  $num_deleted = db_delete('logic_block_blocks_created')
    ->condition('block_name', $form_state['values']['delete'])
    ->execute();

  drupal_set_message(t("Block :block_name deleted.", array(":block_name" => $form_state['values']['delete'])), 'status');
  $form_state['redirect'] = 'admin/structure/logic_block/manage';
  drupal_redirect_form($form_state);

}

/**
 * Does the prompt page when confirming block duplication.
 */
function logic_block_duplicate_block() {

  $form['intro'] = array(
    '#markup' => '<h3>' . t('Are you sure you want to duplicate :block_name ?', array(':block_name' => arg(4))) . '</h3>',
  );

  $form['duplicate'] = array(
    '#type' => 'hidden',
    '#value' => arg(4),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Duplicate Block'),
    '#submit' => array('logic_block_duplicate_block_handler'),
  );

  return $form;

}

/**
 * Does the block duplication.
 */
function logic_block_duplicate_block_handler($form, $form_state) {

  $query = db_query('SELECT n.block_name 
  FROM {logic_block_blocks_created} n 
  WHERE n.block_name = :block_name 
  LIMIT 1',
  array(':block_name' => arg(4) . " copy"));

  $exists = $query->fetch();

  if (!isset($exists->block_name)) {

    $query = db_query('SELECT block_name,base_bid,logic from {logic_block_blocks_created}
    WHERE block_name = :block_name 
    LIMIT 1',
    array(':block_name' => arg(4)));

    $block = $query->fetch();

    $record = array(
      'block_name' => $block->block_name . " copy",
      'base_bid' => $block->base_bid,
      'logic' => $block->logic,
    );

    drupal_write_record('logic_block_blocks_created', $record);

    drupal_set_message(t("Block ':block_name copy' duplicated.", array(":block_name" => $form_state['values']['duplicate'])), 'status');

    $form_state['redirect'] = 'admin/structure/logic_block/manage';
    drupal_redirect_form($form_state);

  }
  else {

    drupal_set_message(t("Block name '%block_name copy' is already in use. Please choose another name", array("%block_name" => arg(4))), 'error');

    $form_state['redirect'] = 'admin/structure/logic_block/manage';
    drupal_redirect_form($form_state);

  }

}

/**
 * Does the display of the edit block.
 */
function logic_block_edit_block() {

  $query = db_query('SELECT block_name,base_bid,logic from {logic_block_blocks_created} 
  WHERE block_name = :block_name 
  LIMIT 1',
  array(':block_name' => arg(4)));

  $block = $query->fetch();
  $block_info = unserialize($block->logic);

  if (file_exists(dirname(__FILE__) . "/logic_types/" . $block_info[0])) {

    require_once dirname(__FILE__) . "/logic_types/" . $block_info[0];

    $file_name = explode("/", $block_info[0]);

    $func = "logic_block_" . str_replace(".inc", "", $file_name[1]) . "_block_edit";

    return $func($block->block_name, $block_info);

  }

}
