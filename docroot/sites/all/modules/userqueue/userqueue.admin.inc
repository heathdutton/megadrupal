<?php

/**
 * @file
 * Admin related functionlity.
 */

/**
 * Confirm form to delete a queue.
 */
function userqueue_admin_delete($form, &$form_state, $queue) {
  $form['uqid'] = array('#type' => 'value', '#value' => $queue->uqid);
  return confirm_form($form, t('Are you sure you want to delete "%title"?', array('%title' => $queue->title)), isset($_GET['destination']) ? $_GET['destination'] : 'admin/structure/userqueue', t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Submit function for nodequeue delete.
 */
function userqueue_admin_delete_submit($formid, &$form_state) {
  if ($form_state['values']['confirm']) {
    userqueue_delete_queue($form_state['values']['uqid']);
  }
  $form_state['redirect'] = 'admin/structure/userqueue';
}

/**
 * Page callback for the main admin page.
 */
function userqueue_list_queues() {
  $queues = userqueue_load_queues();
  if (empty($queues)) {
    return t('No userqueues exist.');
  }

  $header = array(
    array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
    array('data' => t('Max Users'), 'field' => 'size'),
    array('data' => t('Operation')),
  );
  $table_sort = tablesort_init($header);

  $uqids = array();
  $sort_primary = array();
  $sort_secondary = array();
  $sort_direction_regular = array('asc' => SORT_ASC, 'desc' => SORT_DESC);
  $sort_direction_reverse = array('asc' => SORT_DESC, 'desc' => SORT_ASC);
  foreach ($queues as $queue) {
    $sort_secondary[] = drupal_strtolower($queue->title);
    switch ($table_sort['sql']) {
      case 'title':
      default:
        $sort_primary[] = drupal_strtolower($queue->title);
        $sort_direction = $sort_direction_regular;
        break;

      case 'size':
        $sort_primary[] = $queue->size;
        $sort_direction = $sort_direction_reverse;
        break;
    }
  }

  if (!empty($table_sort)) {
    if (strtolower($table_sort['sort']) == 'desc') {
      // Re-indexes array keys; key no longer equals qid.
      array_multisort($sort_primary, $sort_direction['desc'], $sort_secondary, $queues);
    }
    else {
      // Re-indexes array keys; key no longer equals qid.
      array_multisort($sort_primary, $sort_direction['asc'], $sort_secondary, $queues);
    }
  }

  $rows = array();
  foreach ($queues as $queue) {
    $operations = array(l(t('View'), "admin/structure/userqueue/$queue->uqid/view"));
    if (user_access('administer userqueue')) {
      $operations[] = l(t('Edit'), "admin/structure/userqueue/$queue->uqid/edit");
      $operations[] = l(t('Delete'), "admin/structure/userqueue/$queue->uqid/delete");
    }

    $rows[] = array(
      array('class' => array('userqueue-title'), 'data' => check_plain($queue->title)),
      array('class' => array('userqueue-max-nodes'), 'data' => $queue->size == 0 ? t('Infinite') : $queue->size),
      array('class' => array('userqueue-operation'), 'data' => implode(' | ', $operations)),
    );
  }
  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager', array('tags' => NULL));
  return $output;
}

/**
 * Machinery for editing queue metadata (not adding/ordering/removing users).
 */
function userqueue_edit_form($form, &$form_state, $queue) {
  $form = array();
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Queue title'),
    '#default_value' => !empty($queue->title) ? check_plain($queue->title) : '',
  );
  $form['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Queue length'),
    '#size' => 5,
    '#default_value' => !empty($queue->size) ? check_plain($queue->size) : '',
    '#description' => t('Maximum number of users allowed in the queue'),
  );
  $form['reverse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show items in reverse'),
    '#default_value' => !empty($queue->reverse) ? check_plain($queue->reverse) : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (!empty($queue->uqid)) {
    $form['uqid'] = array(
      '#type' => 'hidden',
      '#value' => $queue->uqid,
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  return $form;
}

/**
 * Validate/rebuild queue edit form.
 */
function userqueue_edit_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['size'])) {
    form_set_error('size', 'Queue length must be a number.');
  }
}

/**
 * Submit function for the queue edit form.
 */
function userqueue_edit_form_submit($form, &$form_state) {
  $val = &$form_state['values'];
  if ($val['op'] == t('Delete')) {
    // Redirecting to confirmation form.
    drupal_goto('admin/structure/userqueue/' . $val['uqid'] . '/delete');
  }

  if (!empty($val['uqid'])) {
    // Update existing queue.
    userqueue_update_queue($val, $val['uqid']);
    drupal_goto('admin/structure/userqueue');
  }
  else {
    // Insert a new userqueue.
    userqueue_create_queue($val);
    drupal_goto('admin/structure/userqueue');
  }
}
