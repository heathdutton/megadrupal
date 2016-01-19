<?php
/**
 * @file
 * Functions from parse wants
 */

/**
 * @defgroup ces_import4ces_wants Parse wants from CES
 * @ingroup ces_import4ces
 * @{
 * Functions from parse wants
 */

/**
 * Parse wants.
 */
function ces_import4ces_parse_wants($import_id, $data, $row, &$context, $width_ajax = TRUE) {
  if (isset($context['results']['error'])) {
    return;
  }
  $tx = db_transaction();
  try {
    ob_start();
    $context['results']['import_id'] = $import_id;
    $import = ces_import4ces_import_load($import_id);
    $import->row = $row;

    $category_id = ces_import4ces_get_category('general', $import);

    $state = ( $data['Hidden'] == 0 ) ? 1 : 0 ;

    $want = array(
      'type' => 'want',
      'user' => $data['Advertiser'],
      'title' => $data['Title'],
      'body' => $data['Description'],
      'category' => $category_id,
      'keywords' => '',
      'state' => $state,
      'created' => strtotime($data['DateAdded']),
      'modified' => strtotime($data['DateEdited']),
      // Add 150 days more of DateAdded (need something).
      //'expire' => (strtotime($data['DateAdded']) + (60 * 60 * 24 * 150)),
      'expire' => strtotime($data['DateExpires']),
      // 'rate'       => $data['Rate'],
      // 'image'      => $data['Image'],
    );

    $extra_info = array(
      'DateStarts' => $data['DateStarts'],
    );

    // Find uid from user.
    $query = db_query('SELECT uid FROM {users} where name=:name', array(':name' => $data['Advertiser']));
    $want_user_id = $query->fetchColumn(0);

    if (empty($want_user_id)) {
      $m = t('The user @user was not found in want import row @row. It may be a
      user from another exchange not yet imported.', array('@user' => $data['Advertiser'], '@row' => $row));

      throw new Exception($m);
    }

    $want['user'] = $want_user_id;

    $o = (object) $want;
    $want = ces_offerwant_save($o);
    db_insert('ces_import4ces_objects')->fields(
      array(
        'import_id' => $import_id,
        'object' => 'wants',
        'object_id' => $want->id,
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
