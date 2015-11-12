<?php

/**
 * @file
 * Handles the management page for Speed_form_blocks module.
 */

/**
 * Does the 'admin/structure/speed_form_blocks/management' page.
 */
function speed_form_blocks_management() {

  $result = db_query('SELECT bid, block_name, extra, button, 
  message, pre_html, 
  post_html, class 
  from {speed_form_blocks_created}');
  $counter = 0;
  
  $header = array(
    array('data' => t('Name')),
    array('data' => t('Actions')),
  );

  foreach ($result as $block) {

    if ($counter == 0) {

      $form['intro'] = array(
        '#markup' => '<h3>' . t('Manage speed forms blocks') . '</h3>',
      );

    }

    $counter++;

    $tablerow = array(
      array('data' => check_plain($block->block_name)),
      array('data' => l(t('Manage'), 'admin/structure/speed_form_blocks/manage_blocks/'. $block->block_name) . " | " 
	          . l(t('Delete'), 'admin/structure/speed_form_blocks/delete/'. $block->block_name) . " | "
	          . l(t('Feedback'), 'admin/structure/speed_form_blocks/feedback/'. $block->block_name) . " | "
	          . l(t('Export'), 'admin/structure/speed_form_blocks/export/'. $block->block_name)
			),
    );
    $rows[] = $tablerow;

  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'speed_form_blocks'),
  );
  return $build;

}
