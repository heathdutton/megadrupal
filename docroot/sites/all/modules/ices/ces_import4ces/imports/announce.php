<?php
/**
 * @file
 * Functions from parse announces
 */

/**
 * @defgroup ces_import4ces_announce Parse announces from CES
 * @ingroup ces_import4ces
 * @{
 * Functions from parse announces
 */

/**
 * Parse setting.
 */
function ces_import4ces_parse_announce($import_id, $data, $row, &$context, $width_ajax = TRUE) {
  global $user;
  if (isset($context['results']['error'])) {
    return;
  }
  $tx = db_transaction();
  try {
    ob_start();
    $context['results']['import_id'] = $import_id;
    $import = ces_import4ces_import_load($import_id);

    $exchange_id = $import->exchange_id;

    $extra_info = array(
      'ID' => $data['ID'],
      'DateAdded' => $data['DateAdded'],
      'DateEvent' => $data['DateEvent'],
      'DateExpiry' => $data['DateExpiry'],
      'Keep' => $data['Keep'],
    );

    $query = db_query('SELECT uid FROM {users} where name=:name', array(':name' => $data['Owner']));
    $announce_user_id = $query->fetchColumn(0);
    if (!$announce_user_id) {
      $announce_user_id = $user->uid;
    }

    // Create a blog post.
    $node = new stdClass();
    $node->title = $data['Title'];
    $node->type = 'ces_blog';
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->uid = $announce_user_id;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 2;
    $node->ces_blog_exchange[LANGUAGE_NONE][0]['value'] = $exchange_id;
    $node->body[LANGUAGE_NONE][0] = array(
      'summary' => '',
      'value' => $data['Description'],
      'format' => 'filtered_html',
    );
    $node = node_submit($node);

    node_save($node);

    db_insert('ces_import4ces_objects')
      ->fields(array(
        'import_id' => $import_id,
        'object' => 'announces',
        'object_id' => $node->nid,
        'row' => $row,
        'data' => serialize($extra_info),
      ))->execute();
    ces_import4ces_update_row($import_id, $row);
    ob_end_clean();
  }
  catch (Exception $e) {
    ob_end_clean();
    $tx->rollback();
    $context['results']['error'] = check_plain($e->getMessage());
    $_SESSION['ces_import4ces_row_error']['row']  = $row;
    $_SESSION['ces_import4ces_row_error']['m']    = $e->getMessage();
    $_SESSION['ces_import4ces_row_error']['data'] = $data;
    if ($width_ajax) {
      $result = array('status' => FALSE, 'data' => check_plain($e->getMessage()));
      die(json_encode($result));
    }
    else {
      ces_import4ces_batch_fail_row($import_id, array_keys($data), array_values($data), $row, $context);
    }
  }
}

/** @} */
