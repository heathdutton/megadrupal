<?php
/**
 * @file
 * Provides the get total' administrative interface.
 */

/**
 * Creating drop down in form and also result.
 *
 * @param array $form
 *   $form will contain information about drop down.
 * @param array $form_submit
 *   Get form values form submissions.
 */
function get_total_form($form, &$form_submit) {
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
  );
  $form['filters']['content_type'] = array(
    '#title' => t('Content type'),
    '#type' => 'select',
    '#description' => t('Select the content type.'),
    '#options' => node_type_get_names(),
    '#required' => TRUE,
    '#default_value' => isset($_GET['type']) ? $_GET['type'] : NULL,
  );
  $form['filters']['content_status'] = array(
    '#title' => t('Content status'),
    '#type' => 'select',
    '#description' => t('Select the content status.'),
    '#options' => array(t('Unpublished'), t('Published')),
    '#required' => TRUE,
    '#default_value' => isset($_GET['status']) ? $_GET['status'] : NULL,
  );
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  if (isset($_GET['type']) && isset($_GET['status'])) {
    $data = _get_total_get_content(check_plain($_GET['type']), check_plain($_GET['status']));
    $html = "";
    $html .= t('Total number of node is: @total', array('@total' => count($data)));
    if (count($data) > 0) {
      $per_page = 10;
      // Initialise the pager.
      $current_page = pager_default_initialize(count($data), $per_page);
      // Split your list into page sized chunks.
      $chunks = array_chunk($data, $per_page, TRUE);
      // Show the pager.
      $html .= theme('pager', array('quantity', count($data)));
      // Show the appropriate items from the list.
      $html .= theme('item_list', array(
       'items' => $chunks[$current_page],
      ));
    }
    $form['node_title'] = array(
      '#markup' => $html,
    );
  }
  return $form;
}

/**
 * Function get_total_form_submit() will contain submission logic.
 *
 * @return drupal_goto 
 *   Redirect page to admin/config/content/get-total.
 */
function get_total_form_submit($form, &$form_state) {
  $type = $form_state['values']['content_type'];
  $status = $form_state['values']['content_status'];
  drupal_goto(current_path(), array('query' => array('type' => $type, 'status' => $status)));
}

/**
 * Function will get all content by the help of mysql query.
 */
function _get_total_get_content($content_type, $status) {
  $result = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', $content_type, '=')
    ->condition('n.status', $status, '=')
    ->execute();
  $title = array();
  while ($data = $result->fetchAssoc()) {
    $title[] = $data['title'];
  }
  return $title;
}
