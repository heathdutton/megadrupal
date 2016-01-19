<?php

/**
 * @file
 * Handles the management page for Speed_form_blocks module.
 */

/**
 * Does the 'admin/structure/speed_form_blocks/management' page.
 */
function logic_block_build() {

  $result = db_query('SELECT block_name from {logic_block_blocks_created}');

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

  foreach ($result as $block) {

    $logic = array(
      '#markup' => $block->block_name,
    );

    $actions = array(
      '#markup' => l(t('Edit'), 'admin/structure/logic_block/manage_blocks/' . $block->block_name . '/edit') . ' | '
      . l(t('Delete'), 'admin/structure/logic_block/manage_blocks/' . $block->block_name . '/delete') . ' | '
      . l(t('Duplicate'), 'admin/structure/logic_block/manage_blocks/' . $block->block_name . '/duplicate'),
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

  return $form;

}
