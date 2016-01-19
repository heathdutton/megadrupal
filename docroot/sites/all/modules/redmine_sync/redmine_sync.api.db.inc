<?php


/**
 * redmine_sync_init_db_rdm().
 */
function redmine_sync_init_db_rdm() {
  $err_code = null;
  $src_db_params = array(
    'driver'   => variable_get('redmine_sync_src_db_driver', REDMINE_SYNC_DEF_VAL_SRC_DB_DRIVER),
    'host'     => variable_get('redmine_sync_src_db_host',   REDMINE_SYNC_DEF_VAL_SRC_DB_HOST),
    'port'     => variable_get('redmine_sync_src_db_port',   REDMINE_SYNC_DEF_VAL_SRC_DB_PORT),
    'database' => variable_get('redmine_sync_src_db_name',   REDMINE_SYNC_DEF_VAL_SRC_DB_NAME),
    'username' => variable_get('redmine_sync_src_db_user_name'),
    'password' => variable_get('redmine_sync_src_db_user_pass'),
  );
  if (!in_array('', $src_db_params)) {
    try {
      Database::addConnectionInfo('redmine', 'default', $src_db_params);
      db_set_active('redmine');
      $query = db_select('time_entries', 't');
      $query->addExpression('count(*)', 'total');
      $query->execute();
      $err_code = REDMINE_SYNC_DB_INIT_OK;
    } catch (Exception $e) {
      $err_code = REDMINE_SYNC_DB_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE;
    }
  } else {
    $err_code = REDMINE_SYNC_DB_INIT_EMPTY_PARAMETERS;
  }
  db_set_active();
  return $err_code;
}


/**
 * redmine_sync_db_get_rdm_custom_fields().
 * redmine_sync_db_get_rdm_data().
 * redmine_sync_db_get_drp_data().
 * redmine_sync_db_get_rdm_ids().
 * redmine_sync_db_get_drp_ids().
 * redmine_sync_db_get_rdm_total_non_imported_count().
 * redmine_sync_db_get_rdm_total_non_updated_count().
 * redmine_sync_db_get_drp_total_imported_count().
 * redmine_sync_db_get_drp_last_id().
 * redmine_sync_db_get_rdm_last_update_timestamp().
 * redmine_sync_db_get_drp_last_update_timestamp().
 * redmine_sync_db_get_rdm_id_sum().
 * redmine_sync_db_get_drp_id_sum().
 * redmine_sync_db_get_drp_get_ranges().
 */
function redmine_sync_db_get_rdm_custom_fields() {
  db_set_active('redmine');
  $query = db_select('custom_fields', 'c');
  $query->fields('c', array('id', 'name'));
  $query->orderBy('id');
  $return_value = $query->execute()->fetchAllKeyed(0, 1);
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_rdm_data($entity = 'time_entries', $conditions = array(), $limit = REDMINE_SYNC_DEF_VAL_ITEMS_IMPORT_PER_BATCH) {
  db_set_active('redmine');
  $query = db_select($entity, 'e')->extend('PagerDefault');
  switch ($entity) {
    case 'time_entries':
      $query->fields('e', array('id', 'project_id', 'user_id', 'activity_id', 'issue_id', 'hours', 'comments'));
      $query->addExpression("unix_timestamp(convert_tz(e.spent_on, '+00:00', @@session.time_zone))", 'spent_on');
      $query->addExpression("unix_timestamp(convert_tz(e.created_on, '+00:00', @@session.time_zone))", 'created_on');
      $query->addExpression("unix_timestamp(convert_tz(e.updated_on, '+00:00', @@session.time_zone))", 'updated_on');
      $nid_field_id = variable_get('redmine_sync_custom_field_for_node', REDMINE_SYNC_DEF_VAL_NID_FIELD_ID);
      if ($nid_field_id) {
        $query->leftJoin('custom_values', 'c', "e.id = c.customized_id and c.customized_type = 'TimeEntry' and c.custom_field_id = $nid_field_id");
        $query->addExpression('if(c.value=\'\',null,c.value)', 'nid');
      }
      break;
    case 'users':
      $query->fields('e', array('id', 'mail'));
      break;
    case 'enumerations':
      $query->fields('e', array('id', 'name'));
      break;
    default: $query->fields('e');
  }
  foreach ($conditions as $c_condition) {
    $query->condition($c_condition[0], $c_condition[1], isset($c_condition[2]) ? $c_condition[2] : '=');
  }
  $query->orderBy('e.id');
  $query->limit($limit);
  $return_value = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_drp_data($entity = 'time_entries', $conditions = array(), $limit = REDMINE_SYNC_DEF_VAL_ITEMS_IMPORT_PER_BATCH) {
  $query = db_select($entity, 'e')->extend('PagerDefault');
  switch ($entity) {
    case 'time_entries':
      $query->fields('e', array('id', 'project_id', 'user_id', 'activity_id', 'issue_id', 'hours', 'comments', 'spent_on', 'created_on', 'updated_on', 'nid'));
      break;
    default:
      $query->fields('e');
  }
  foreach ($conditions as $c_condition) {
    $query->condition($c_condition[0], $c_condition[1], isset($c_condition[2]) ? $c_condition[2] : '=');
  }
  $query->orderBy('e.id');
  $query->limit($limit);
  return $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);
}


function redmine_sync_db_get_rdm_ids($entity = 'time_entries', $conditions = array()) {
  db_set_active('redmine');
  $query = db_select($entity, 'e');
  $query->fields('e', array('id'));
  foreach ($conditions as $c_condition) {
    $query->condition($c_condition[0], $c_condition[1], isset($c_condition[2]) ? $c_condition[2] : '=');
  }
  $query->orderBy('e.id');
  $return_value = $query->execute()->fetchAllKeyed(0, 0);
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_drp_ids($entity = 'time_entries', $conditions = array()) {
  $query = db_select($entity, 'e');
  $query->fields('e', array('id'));
  foreach ($conditions as $c_condition) {
    $query->condition($c_condition[0], $c_condition[1], isset($c_condition[2]) ? $c_condition[2] : '=');
  }
  $query->orderBy('e.id');
  return $query->execute()->fetchAllKeyed(0, 0);
}


function redmine_sync_db_get_rdm_total_non_imported_count($entity = 'time_entries', $start_id = 0) {
  db_set_active('redmine');
  $query = db_select($entity, 'e');
  $query->addExpression('count(*)', 'total');
  $query->condition('id', $start_id, '>=');
  $return_value = (int)$query->execute()->fetchField();
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_rdm_total_non_updated_count($entity = 'time_entries', $end_id = 0, $start_updated_on_timestamp = 0) {
  db_set_active('redmine');
  $query = db_select($entity, 'e');
  $query->addExpression('count(*)', 'total');
  $query->condition('id', $end_id, '<=');
  $query->condition('updated_on', gmdate(REDMINE_SYNC_DEF_DATE_TIME_FORMAT, $start_updated_on_timestamp), '>=');
  $return_value = (int)$query->execute()->fetchField();
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_drp_total_imported_count($entity = 'time_entries') {
  $query = db_select($entity, 'e');
  $query->addExpression('count(e.id)', 'total');
  return (int)$query->execute()->fetchField();
}


function redmine_sync_db_get_drp_last_id($entity = 'time_entries') {
  $query = db_select($entity, 'e');
  $query->addExpression('max(e.id)', 'max_tid');
  return (int)$query->execute()->fetchField();
}


function redmine_sync_db_get_rdm_last_update_timestamp($entity = 'time_entries') {
  db_set_active('redmine');
  $query = db_select($entity, 'e');
  $query->addExpression("unix_timestamp(convert_tz(max(e.updated_on), '+00:00', @@session.time_zone))", 'max_updated_on');
  $return_value = (int)$query->execute()->fetchField();
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_drp_last_update_timestamp($entity = 'time_entries') {
  $query = db_select($entity, 'e');
  $query->addExpression('max(e.updated_on)', 'max_updated_on');
  return (int)$query->execute()->fetchField();
}


function redmine_sync_db_get_rdm_id_sum($entity = 'time_entries', $start_id = 0, $end_id = null) {
  db_set_active('redmine');
  $query = db_select($entity, 'e');
  $query->addExpression('sum(e.id)', 'id_sum');
  $query->condition('id', $start_id, '>=');
  if ($end_id) $query->condition('id', $end_id, '<=');
  $return_value = (int)$query->execute()->fetchField();
  db_set_active();
  return $return_value;
}


function redmine_sync_db_get_drp_id_sum($entity = 'time_entries', $start_id = 0, $end_id = null) {
  $query = db_select($entity, 'e');
  $query->addExpression('sum(e.id)', 'id_sum');
  $query->condition('id', $start_id, '>=');
  if ($end_id) $query->condition('id', $end_id, '<=');
  return (int)$query->execute()->fetchField();
}


function redmine_sync_db_get_drp_get_ranges($entity = 'time_entries') {
  $drp_ids = array();
  $i = 0;
  $query = db_select($entity, 'e');
  $query->fields('e', array('id'));
  $query->orderBy('e.id');
  while ($id = (int)$query->range($i, 1)->execute()->fetchField()) {
    $i+= REDMINE_SYNC_DEF_VAL_ITEMS_PER_RANGE;
    $drp_ids[]= $id;
  }
  return $drp_ids;
}


/**
 * redmine_sync_db_time_entries_clear_all().
 * redmine_sync_db_time_entries_import_new().
 * redmine_sync_db_time_entries_update_exist().
 * redmine_sync_db_time_entries_remove_old().
 * redmine_sync_db_time_entries_sync_all().
 * redmine_sync_db_user_sync_by_email().
 */
function redmine_sync_db_time_entries_clear_all() {
  $transaction = db_transaction();
  try {
    db_query('truncate {time_entries}')->execute();
    return true;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function redmine_sync_db_time_entries_import_new($simulate = false) {
  $transaction = db_transaction();
  try {
    // [RU] Как производится импорт:
    //      1) из таблицы time_entries на стороне Drupal получаем максимальный id записи;
    //      2) из таблицы time_entries на стороне Redmine выбираем N записей чьи id больше id, полученного в п.1;
    //      3) выбранные записи добавляем в таблицу time_entries на стороне Drupal.
    //      P.S. Каждая новая запись в таблице time_entries на стороне Redmine всегда будет иметь id выше
    //      любого id в таблице time_entries на стороне Drupal.
    $ret_ids = array();
    $items_insert_per_batch = variable_get('redmine_sync_items_import_per_batch', REDMINE_SYNC_DEF_VAL_ITEMS_IMPORT_PER_BATCH);
    $drp_last_id = redmine_sync_db_get_drp_last_id();
    $rdm_data = redmine_sync_db_get_rdm_data('time_entries', array(array('e.id', $drp_last_id, '>')), $items_insert_per_batch);
    foreach ($rdm_data as $c_row) {
      if (!$simulate) db_insert('time_entries')->fields($c_row)->execute();
      $ret_ids[]= $c_row['id'];
    }
    return $ret_ids;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function redmine_sync_db_time_entries_update_exist($simulate = false) {
  $transaction = db_transaction();
  try {
    // [RU] Как производится обновление:
    //      1) из таблицы time_entries на стороне Drupal получаем максимальный id записи;
    //      2) из таблицы time_entries на стороне Drupal получаем максимальную дату обновления из поля updated_on;
    //      3) из таблицы time_entries на стороне Redmine выбираем N записей чьи id меньше id полученного в п.1 и чьи даты обновления больше максимальной из п.2.
    //      4) выбранные записи обновляем в таблице time_entries на стороне Drupal.
    //      P.S.1 В данном случае максимальный id записи необходим для ограничения области выборки – таким образом мы получим
    //      одинаковые множества. Любая обновленнная запись на стороне Redmine будет иметь дату обновления, выше
    //      максимальной на стороне Drupal.
    //      P.S.2 Обновление необходимо выполнить единовременно для всех записей в выбранном множестве,
    //      иначе запрос на выборку последней даты будет не соответствовать реальной ситуации в базе.
    $ret_ids = array();
    $drp_last_id = redmine_sync_db_get_drp_last_id();
    $drp_last_update_timestamp = redmine_sync_db_get_drp_last_update_timestamp();
    $rdm_data = redmine_sync_db_get_rdm_data('time_entries', array(
      array('e.id', $drp_last_id, '<='),
      array('e.updated_on', gmdate(REDMINE_SYNC_DEF_DATE_TIME_FORMAT, $drp_last_update_timestamp), '>')));
    foreach ($rdm_data as $c_row) {
      $c_id = $c_row['id'];
      unset($c_row['id']);
      if (!$simulate) db_update('time_entries')->fields($c_row)->condition('id', $c_id)->execute();
      $ret_ids[]= $c_id;
    }
    return $ret_ids;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function redmine_sync_db_time_entries_remove_old($simulate = false) {
  $transaction = db_transaction();
  try {
    // [RU] Как производится удаление:
    //      1) делим данные в таблице time_entries на стороне Drupal на области – получаем стартовые id для каждой
    //         области: id_начала_области_i, id_начала_области_i+1, id_начала_области_i+2 и т.д.;
    //      2) по полю id вычисляем сумму записей в таблице time_entries на стороне Drupal,
    //         ограниченных условием "WHERE id >= id_начала_области_i AND id < id_начала_области_i+1";
    //      3) по полю id вычисляем сумму записей в таблице time_entries на стороне Redmine,
    //         ограниченных условием "WHERE id >= id_начала_области_i AND id < id_начала_области_i+1";
    //      4) если сумма из п.2 не совпадает с суммой из п.3 (сумма записей на стороне Drupal больше суммы записей
    //         на стороне Drupal), значит в области есть удаленные поля;
    //      5) если есть удаленные поля, тогда полностью загружаем область данных на стороне Drupal
    //         и на стороне Redmine и выполяем сравнение по каждой записи - либо делаем array_diff, либо выполняем
    //         запрос "SELECT id FROM drupal.time_entries WHERE id not in (SELECT id FROM redmine.time_entries)";
    //      6) если в результате сравнения в п.5 id записи не оказалось на стороне данных Redmine,
    //         тогда удаляем запись на стороне Drupal.
    $ret_ids = array();
    $drp_ids = redmine_sync_db_get_drp_get_ranges();
    $drp_last_id = redmine_sync_db_get_drp_last_id();
    if (count($drp_ids)) {
      for ($i = 0; $i < count($drp_ids); $i++) {
        $c_drp_sum = redmine_sync_db_get_drp_id_sum('time_entries', $drp_ids[$i], isset($drp_ids[$i + 1]) ? $drp_ids[$i + 1] : $drp_last_id);
        $c_rdm_sum = redmine_sync_db_get_rdm_id_sum('time_entries', $drp_ids[$i], isset($drp_ids[$i + 1]) ? $drp_ids[$i + 1] : $drp_last_id);
        if ($c_drp_sum != $c_rdm_sum) {
          if (isset($drp_ids[$i + 1])) {
            $c_rdm_ids = redmine_sync_db_get_rdm_ids('time_entries', array(array('id', $drp_ids[$i], '>='), array('id', $drp_ids[$i + 1], '<=')));
            $c_drp_ids = redmine_sync_db_get_drp_ids('time_entries', array(array('id', $drp_ids[$i], '>='), array('id', $drp_ids[$i + 1], '<=')));
          } else {
            $c_rdm_ids = redmine_sync_db_get_rdm_ids('time_entries', array(array('id', $drp_ids[$i], '>='), array('id', $drp_last_id, '<=')));
            $c_drp_ids = redmine_sync_db_get_drp_ids('time_entries', array(array('id', $drp_ids[$i], '>='), array('id', $drp_last_id, '<=')));
          }
          $c_removed_ids = array_diff($c_drp_ids, $c_rdm_ids);
          foreach ($c_removed_ids as $c_removed_id) {
            if (!$simulate) db_delete('time_entries')->condition('id', $c_removed_id)->execute();
            $ret_ids[]= $c_removed_id;
          }
        }
      }
    }
    return $ret_ids;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function redmine_sync_db_time_entries_sync_all() {
  // Fast check and synchronization.
  $drp_sum = redmine_sync_db_get_drp_id_sum();
  $rdm_sum = redmine_sync_db_get_rdm_id_sum();
  $drp_last_update = redmine_sync_db_get_drp_last_update_timestamp();
  $rdm_last_update = redmine_sync_db_get_rdm_last_update_timestamp();
  if ($drp_sum != $rdm_sum || $drp_last_update != $rdm_last_update) {
    $timer_0 = microtime(true);
    $ids_old = redmine_sync_db_time_entries_remove_old();   // 0.2245960 sec
    $ids_upd = redmine_sync_db_time_entries_update_exist(); // 0.1451499 sec.
    $ids_new = redmine_sync_db_time_entries_import_new();   // 0.0029211 sec.
    $timer_1 = microtime(true);
  }
  // Reports.
  if (isset($ids_new) && count($ids_new)) {
    if (user_access('administer site configuration')) drupal_set_message(t('Imported !count records.', array('!count' => count($ids_new))));
    watchdog('redmine_sync', 'Imported !count records. The next id\'s was imported: !ids', array('!count' => count($ids_new), '!ids' => implode(', ', $ids_new)));
  }
  if (isset($ids_upd) && count($ids_upd)) {
    if (user_access('administer site configuration')) drupal_set_message(t('Updated !count records.', array('!count' => count($ids_upd))));
    watchdog('redmine_sync', 'Updated !count records. The next id\'s was updated: !ids', array('!count' => count($ids_upd), '!ids' => implode(', ', $ids_upd)));
  }
  if (isset($ids_old) && count($ids_old)) {
    if (user_access('administer site configuration')) drupal_set_message(t('Removed !count records.', array('!count' => count($ids_old))));
    watchdog('redmine_sync', 'Removed !count records. The next id\'s was removed: !ids', array('!count' => count($ids_old), '!ids' => implode(', ', $ids_old)));
  }
  if (isset($timer_0) && isset($timer_1)) {
    if (user_access('administer site configuration')) drupal_set_message(t('Time of live synchronization = !time sec.', array('!time' => number_format($timer_1 - $timer_0, 7))));
  }
}


function redmine_sync_db_user_sync_by_email($mail = null) {
  $db_init = redmine_sync_init_db_rdm();
  if ($db_init == REDMINE_SYNC_DB_INIT_OK) {
    if ($mail) $users = redmine_sync_db_get_rdm_data('users', array(array('mail', $mail)));
    else       $users = redmine_sync_db_get_rdm_data('users');
    if (count($users)) {
      foreach ($users as $c_user) {
        $query = db_update('users');
        $query->fields(array('user_id' => $c_user['id']));
        $query->condition('mail', $c_user['mail']);
        $query->condition('mail', '', '<>');
        $query->isNotNull('mail');
        $is_updated = $query->execute();
        if ($is_updated) {
          drupal_set_message(t('Field %field_name in table %table_name with email = %mail was updated.', array('%field_name' => 'user_id', '%table_name' => 'users', '%mail' => $c_user['mail'])));
          watchdog('redmine_sync', 'Field %field_name in table %table_name with email = %mail was updated.', array('%field_name' => 'user_id', '%table_name' => 'users', '%mail' => $c_user['mail']));
        }
      }
    }
  }
}

