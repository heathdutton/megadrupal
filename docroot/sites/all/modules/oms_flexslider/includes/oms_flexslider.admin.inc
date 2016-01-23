<?php
/**
 * @file
 * OMS Flexslider module admin settings.
 */

/**
 * Wrapper function to make page with table and form.
 *
 * @return string
 *   html content
 */
function oms_flexslider_admin_page() {
  $page = oms_flexslider_admin_table();
  $adminform = drupal_get_form('oms_flexslider_admin_create_form');
  $page .= drupal_render($adminform);
  return $page;
}

/**
 * Oms_flexslider_admin_create_form.
 *
 * @access public
 * @return array
 *   form array
 */
function oms_flexslider_admin_create_form() {
  $form['oms_flexslider_create_block_name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['oms_flexslider_create_block'] = array(
    '#type' => 'submit',
    '#value' => t('Create New Block'),
    '#submit' => array(
      'oms_flexslider_new_block',
    ),
  );
  return $form;
}

/**
 * Oms_flexslider_admin_delete_form.
 *
 * @param mixed $block_id
 *   block delta
 *
 * @access public
 * @return array
 *   form array
 */
function oms_flexslider_admin_delete_form($form, &$form_state, $block_id) {
  $form['oms_flexslider_delete_block'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Block'),
    '#submit' => array(
      'oms_flexslider_delete_block',
    ),
  );
  $form['oms_flexslider_delete_block_id'] = array(
    '#type' => 'hidden',
    '#value' => ($block_id),
  );
  return $form;
}


/**
 * Creates a table of all OMS Flexslider blocks.
 *
 * @return form
 *   A table of blocks with an add more form.
 */
function oms_flexslider_admin_table() {
  global $base_url;
  $results = oms_flexslider_block_query();
  $rows = array();
  foreach ($results as $record) {
    $deleteform = drupal_get_form('oms_flexslider_admin_delete_form', $record->fsid);
    $rows[] = array(
      '<a href="' . $base_url . '/admin/structure/block/manage/oms_flexslider/' . $record->fsid . '/configure">' . $record->name . '</a>',
      drupal_render($deleteform),
    );
  }
  $output = array(
    'header' => array(
      t('Block Name'),
      t('Delete Block'),
    ),
    'rows' => $rows,
  );
  return theme('table', $output);
}

/**
 * Oms_flexslider_new_block.
 *
 * @param mixed $form
 *   form
 * @param mixed $form_state
 *   form state
 *
 * @access public
 */
function oms_flexslider_new_block($form, &$form_state) {
  $slidedefaults = '{"animation":"fade","direction":"horizontal","reverse":false,"animationLoop":true,"smoothHeight":false,"startAt":0,"slideshow":true,"slideshowSpeed":7000,"animationSpeed":600,"pauseOnAction":true,"pauseOnHover":false,"useCSS":true,"touch":true,"video":false,"controlNav":true,"directionNav":true,"prevText":"Previous","nextText":"Next","keyboard":true,"pausePlay":false,"pausetext":"Pause","playText":"Play"}';
  $new_block = $form['oms_flexslider_create_block_name']['#value'];
  db_insert('oms_flexslider')->fields(array(
    'name' => $new_block,
    'slider_options' => $slidedefaults,
  ))->execute();
}


/**
 * Oms_flexslider_delete_block.
 *
 * @param mixed $form
 *   form
 * @param mixed $form_state
 *   form state
 *
 * @access public
 */
function oms_flexslider_delete_block($form, &$form_state) {
  $block_id = $form_state['input']['oms_flexslider_delete_block_id'];
  db_delete('oms_flexslider')->condition('fsid', $block_id)->execute();
  db_delete('block')->condition('delta', $block_id)->condition('module', 'oms_flexslider')->execute();
}
