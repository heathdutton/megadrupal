<?php // $Id: nodequeue_randomizer.admin.inc,v 1.1.2.2 2011/02/24 23:00:20 gekigangaa Exp $

/**
 * @file nodequeue_randomizer.admin.inc
 *
 * @author Greg Field
 * @since Nov 26th, 2009
 **/

/**
 * Admin settings for selecting which nodequeues are randomized and how often
 *
 * @param $form_state
 *   Array which represents the current form state
 *
 * @return
 *   Array which represents a Drupal form
 */
function nodequeue_randomizer_form_admin_settings($form, &$form_state) {
  global $user;

  // An unfortunate necessity to set a min date of the current time.  Form state doesn't persist
  // initially which prevents it from being used.
  if (!isset($_GET['time'])) {
    // Use the current time but round down to the nearest minute
    drupal_goto('admin/structure/nodequeue/randomizer', array(
        'query' => array(
          'time' => time() - (time() % 60)
        )
      )
    );
    $form_state['storage']['time'] = REQUEST_TIME - (REQUEST_TIME % 60);
  }

  if (variable_get('configurable_timezones', 1) && $user->uid && $user->timezone) {
    $datetimezone = new DateTimeZone($user->timezone);
  }
  else {
    $datetimezone = new DateTimeZone(variable_get('date_default_timezone', 'UTC'));
  }
  $datetime = new DateTime('now', $datetimezone);
  $form_state['storage']['offset'] = date_offset_get($datetime);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l(t('Randomizer Settings'), "admin/structure/nodequeue/randomizer");
  drupal_set_breadcrumb($breadcrumb);

  $qids = nodequeue_get_all_qids(10);

  $queues = nodequeue_load_queues($qids);
  $subqueues = nodequeue_load_subqueues_by_queue($qids);
  $nodequeue_randomizer_queues = nodequeue_randomizer_get_queues(TRUE);

  usort($queues, '_nodequeue_randomizer_alphabetical_queue_sort');

  // Create the nodequeue fieldsets
  foreach ($queues as $queue) {
    $form["nodequeue_{$queue->qid}"] = array(
      '#type'        => 'fieldset',
      '#title'       => $queue->title,
      '#collapsible' => TRUE,
      '#collapsed'   => !isset($nodequeue_randomizer_queues[$queue->qid]),
      '#tree'        => TRUE
    );
  }

  // Create the subqueue fieldsets within the nodequeues
  foreach ($subqueues as $subqueue) {
    $randomizer_settings = (isset($nodequeue_randomizer_queues[$subqueue->qid][$subqueue->sqid])) ? $nodequeue_randomizer_queues[$subqueue->qid][$subqueue->sqid] : NULL;

    $subqueue_form = array(
      '#type'        => 'fieldset',
      '#title'       => $subqueue->title,
      '#collapsible' => TRUE,
      '#collapsed'   => isset($nodequeue_randomizer_queues[$queue->qid]) && !isset($nodequeue_randomizer_queues[$queue->qid][$subqueue->sqid]),
      '#tree'        => TRUE
    );

    $subqueue_form['is_randomized'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Randomize'),
      '#description'   => t('Turn randomization on and off for this subqueue.'),
      '#default_value' => (isset($randomizer_settings)) ? TRUE : FALSE,
      '#tree'          => TRUE
    );
    $subqueue_form['period'] = array(
      '#type'          => 'select',
      '#title'         => t('Randomization period'),
      '#description'   => t('The amount of time between radomizations of this subqueue.'),
      '#options'       => array(
        3600    => t('1 hour'),
        21600   => t('6 hours'),
        43200   => t('12 hours'),
        86400   => t('1 day'),
        172800  => t('2 day'),
        604800  => t('1 week'),
        1209600 => t('2 week')
      ),
      '#default_value' => (isset($randomizer_settings)) ? $randomizer_settings['period'] : 86400,
      '#tree'          => TRUE
    );
    $subqueue_form['next_run'] = array(
      '#type'          => 'date_select',
      '#title'         => t('Next randomization'),
      '#tree'          => TRUE
    );
    if (isset($randomizer_settings)) {
      $default_time = max($randomizer_settings['last_run'] + $randomizer_settings['period'], $_GET['time']) + $form_state['storage']['offset'];
      $subqueue_form['next_run']['#default_value'] = format_date($default_time, 'custom', 'Y-m-d H:i:s', 'UTC');
    }
    else {
      $subqueue_form['next_run']['#default_value'] = format_date($_GET['time'] + $form_state['storage']['offset'], 'custom', 'Y-m-d H:i:s', 'UTC');
    }

    $subqueue_form['view'] = array(
      '#type'  => 'markup',
      '#value' => format_plural($subqueue->count, '1 node in queue', '@count nodes in queue') . ' - ' . l(t('View this subqueue.'), "admin/content/nodequeue/{$subqueue->qid}/view/{$subqueue->sqid}")
    );

    $form["nodequeue_{$subqueue->qid}"]["subqueue_{$subqueue->sqid}"] = $subqueue_form;
  }

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit')
  );
  $form['pager'] = array(
    '#type'   => 'markup',
    '#markup' => theme('pager', array('tags' => NULL))
  );

  return $form;
}

/**
 * Validation for nodequeue randomizer admin settings
 *
 * @param $form
 *   Array which represents a Drupal form
 * @param $form_state
 *   Array which represents the current form state
 */
function nodequeue_randomizer_form_admin_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  $min_date = $_GET['time'];
  $offset = $form_state['storage']['offset'];

  foreach ($values as $key => $value) {
    if (drupal_strlen($key) > 10 && drupal_substr($key, 0, 10) == 'nodequeue_') {
      foreach ($value as $subkey => $subvalue) {
        if (drupal_strlen($subkey) > 9 && drupal_substr($subkey, 0, 9) == 'subqueue_') {
          if ($subvalue['is_randomized']) {
            $next_run = strtotime($subvalue['next_run'] . ' UTC') - $offset;

            if ($next_run < $min_date) {
              $date = date(variable_get('date_format_short', 'm/d/Y - H:i'), $min_date);
              form_set_error("{$value}][{$subvalue}][next_run", t('The specified time of the next run must on or after !date.', array('!date' => $date)));
            }
          }
        }
      }
    }
  }
}

/**
 * Submission for nodequeue randomizer admin settings
 *
 * @param $form
 *   Array which represents a Drupal form
 * @param $form_state
 *   Array which represents the current form state
 */
function nodequeue_randomizer_form_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];
  $offset = $form_state['storage']['offset'];

  // Remove current entries in the randomizer table
  db_delete('nodequeue_randomizer')->execute();

  // Repopulate the table with the new values
  foreach ($values as $key => $value) {
    if (drupal_strlen($key) > 10 && drupal_substr($key, 0, 10) == 'nodequeue_') {
      $qid = substr($key, 10);

      foreach ($value as $subkey => $subvalue) {
        if (drupal_strlen($subkey) > 9 && drupal_substr($subkey, 0, 9) == 'subqueue_') {
          $sqid = substr($subkey, 9);
          if ($subvalue['is_randomized']) {
            $next_run = strtotime($subvalue['next_run'] . ' UTC') - $offset;
            $last_run = $next_run - $subvalue['period'];

            db_insert('nodequeue_randomizer')
              ->fields(array(
                  'qid'      => $qid,
                  'sqid'     => $sqid,
                  'period'   => $subvalue['period'],
                  'last_run' => $last_run
                )
              )
              ->execute();
          }
        }
      }
    }
  }

  drupal_set_message(t('Nodequeue randomizer settings have been updated.'));
}

/**
 * Form of setting the weighting of items within a subqueue for randomization
 *
 * @param $form_state
 *   Array which represents the current form state
 * @param $queue
 *   The nodequeue object who's subqueue is being edited
 * @param $subqueue (optional)
 *   The subqueue of the nodequeue who's nodes are being weighted, optional if nodequeue has only 1 subqueue
 *
 * @return
 *   Array which represents a Drupal form
 */
function nodequeue_randomizer_form_admin_weight($form, &$form_state, $queue, $subqueue = NULL) {
  $form = array();

  if (!isset($subqueue) || !$subqueue->sqid) {
    if ($queue->subqueues == 1) {
      $subqueues = nodequeue_load_subqueues_by_queue($queue->qid);
      $subqueue = array_shift($subqueues);
    }
    else {
      // NOTE: This is not tested as our site does not have a nodequeue with mutiple subqueues
      $subqueue_list = nodequeue_view_subqueues($queue);
      $subqueue_list = preg_replace('/admin\/content\/nodequeue\/(\d+)\/view\/(\d+)/', 'admin/content/nodequeue/$1/weight/$2', $subqueue_list);

      return array(
        'subqueue_list' => array(
          '#type' => 'markup',
          '#value' => $subqueue_list
        )
      );
    }
  }

  if (!nodequeue_api_subqueue_access($subqueue, NULL, $queue)) {
    return drupal_not_found();
  }

  $is_randomized = db_query(
    "SELECT TRUE
    FROM {nodequeue_randomizer}
    WHERE qid = :qid AND sqid = :sqid",
    array(':qid' => $queue->qid, ':sqid' => $subqueue->sqid)
  )->rowCount();
  if (!$is_randomized) {
    drupal_set_message(t('This queue is currently not being randomized.  To activate scheduled randomization, go to !link.', array(
        '!link' => l(t('the nodequeue randomizer settings'), 'admin/content/nodequeue/randomizer')
      )
    ));
  }

  drupal_set_title(t("Subqueue '@title'", array('@title' => nodequeue_title_substitute($queue->subqueue_title, $queue, $subqueue))));
  $form_state['storage']['queue'] = $queue;
  $form_state['storage']['subqueue'] = $subqueue;

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($queue->title, "admin/structure/nodequeue/{$queue->qid}");
  drupal_set_breadcrumb($breadcrumb);

  $node_results = db_query(
    "SELECT n.nid, n.title, nn.weight
    FROM {node} n
    INNER JOIN {nodequeue_nodes} nn ON (n.nid = nn.nid)
    WHERE nn.sqid = :sqid
    ORDER BY nn.position ASC",
    array(':sqid' => $subqueue->sqid)
  );

  foreach ($node_results as $node_info) {
    $form["node_{$node_info->nid}"] = array(
      '#type'          => 'textfield',
      '#default_value' => $node_info->weight,
      '#maxlength'     => 4,
      '#size'          => 6,
      '#attributes'    => array(
        'title' => $node_info->title
      )
    );
  }

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit')
  );

  return $form;
}

/**
 * Validation for setting the weighting of items within a subqueue for randomization
 *
 * @param $form
 *   Array which represents a Drupal form
 * @param $form_state
 *   Array which represents the current form state
 */
function nodequeue_randomizer_form_admin_weight_validate($form, &$form_state) {
  $values = $form_state['values'];

  foreach ($values as $key => $value) {
    if (strlen($key) > 5 && substr($key, 0, 5) == 'node_') {
      if (!is_numeric($value) || $value < 0) {
        form_set_error($key, t('Weights must me numbers equal to or greater than 0.'));
      }
    }
  }
}

/**
 * Submission for setting the weighting of items within a subqueue for randomization
 *
 * @param $form
 *   Array which represents a Drupal form
 * @param $form_state
 *   Array which represents the current form state
 */
function nodequeue_randomizer_form_admin_weight_submit($form, &$form_state) {
  $values = $form_state['values'];
  $queue = $form_state['storage']['queue'];
  $subqueue = $form_state['storage']['subqueue'];

  foreach ($values as $key => $value) {
    if (strlen($key) > 5 && substr($key, 0, 5) == 'node_') {
      $nid = substr($key, 5);

      db_query(
        "UPDATE {nodequeue_nodes}
        SET weight = :weight
        WHERE qid = :qid AND sqid = :sqid AND nid = :nid",
        array(':weight' => $value, ':qid' => $queue->qid, ':sqid' => $subqueue->sqid, ':nid' => $nid)
      );
    }
  }
}

/**
 * Function used by usort to sort nodequeues in alphabetical order
 *
 * @param $a
 *   Nodequeue object used in ordering comparison
 * @param $b
 *   Nodequeue object used in ordering comparison
 *   
 * @return
 *   Integer value denoting the result of comparison, either -1, 0, or 1
 */
function _nodequeue_randomizer_alphabetical_queue_sort($a, $b) {
  $title_a = strtolower($a->title);
  $title_b = strtolower($b->title);

  if ($title_a == $title_b) {
    return 0;
  }

  return ($title_a < $title_b) ? -1 : 1;
}
