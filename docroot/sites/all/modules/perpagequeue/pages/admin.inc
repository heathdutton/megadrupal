<?php
/**
 * @file
 * Administration pages.
 */

/**
 * Display a table of subqueues and a link to delete them.
 */
function perpagequeue_admin_subqueues($queue) {
  $subqueues = nodequeue_load_subqueues_by_queue($queue->qid);

  $rows = array();
  if (!empty($subqueues)) {
    foreach ($subqueues as $subqueue) {
      $rows[] = array(
        'data' => array(
          'sqid' => array(
            'data' => $subqueue->sqid,
          ),
          'path' => array(
            'data' => $subqueue->reference,
          ),
          'actions' => array(
            'data' => l(t('Edit'), 'admin/structure/nodequeue/' . $queue->qid . '/view/' . $subqueue->sqid) . ' | ' . l(t('Delete'), 'admin/structure/nodequeue/' . $queue->qid . '/path/' . $subqueue->sqid . '/delete'),
          ),
        ),
      );
    }
  }
  else {
    $rows[] = array(
      'data' => array(
        'message' => array(
          'data' => t('There are currently no active paths for this queue.'),
          'colspan' => 3,
        ),
      ),
    );
  }

  $rows[] = array(
    'data' => array(
      'message' => array(
        'data' => l(t('Add a new path'), 'admin/structure/nodequeue/' . $queue->qid . '/path/add'),
        'colspan' => 3,
      ),
    ),
  );

  $variables = array(
    'header' => array(
      'sqid' => array(
        'data' => t('Subqueue ID'),
      ),
      'path' => array(
        'data' => t('Path'),
      ),
      'actions' => array(
        'data' => t('Actions'),
      ),
    ),
    'rows' => $rows,
  );

  return theme('table', $variables);
}

/**
 * Form builder for the delete path form.
 */
function perpagequeue_admin_delete_subqueue($form, &$form_state, $queue, $subqueue) {
  $form = array();

  $form['qid'] = array(
    '#type' => 'value',
    '#value' => $queue->qid,
  );

  $form['sqid'] = array(
    '#type' => 'value',
    '#value' => $subqueue->sqid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the subqueue for the path <code>!path</code>', array('!path' => $subqueue->reference)),
    'admin/structure/nodequeue/' . $queue->qid . '/path',
    NULL,
    t('Delete Path'));
}

/**
 * Form submit handler for the delete path form.
 */
function perpagequeue_admin_delete_subqueue_submit($form, &$form_state) {
  $qid = $form_state['values']['qid'];
  $sqid = $form_state['values']['sqid'];

  nodequeue_remove_subqueue($sqid);

  $form_state['redirect'] = 'admin/structure/nodequeue/' . $qid . '/path';
}

/**
 * Form builder for the add new path form.
 */
function perpagequeue_admin_add_subqueue($form, &$form_state, $queue) {
  $form = array();

  $form['queue'] = array(
    '#type' => 'value',
    '#value' => $queue,
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Type the alias or path to the page. Notice that this modules uses <strong>alias paths</strong>, and not internal drupal paths.'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  if ($queue->use_tokens) {
    $form['path']['#description'] .= ' ' . t('This Queue uses token placeholders. You can type a percentage sign (<em>%</em>) to catch any text between two consecutive slashes (<em>/</em>).');
  }
  else {
    $form['path']['#description'] .= ' ' . t('This Queue does NOT use token placeholders. Percentage signs (<em>%</em>) will be caught verbatim.');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add path'),
  );

  return $form;
}

/**
 * Form submit handler for the add new path form.
 */
function perpagequeue_admin_add_subqueue_submit($form, &$form_state) {
  $queue = $form_state['values']['queue'];
  $path = trim($form_state['values']['path']);

  $subqueue = nodequeue_add_subqueue($queue, $path, $path);

  $form_state['redirect'] = 'admin/structure/nodequeue/' . $queue->qid . '/view/' . $subqueue->sqid;
}
