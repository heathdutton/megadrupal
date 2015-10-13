<?php
/**
 * @file
 * This file contains the functions for reuilding various tables.
 */

/**
 * Start batch job for rebuild of nested intervals.
 *
 * @param array $vids
 *   Vocabulary IDs to rebuild.
 * @param bool $safe
 *   (optional) Rebuild safely. Defaults to NULL (use system setting).
 */
function taxonomy_nested_intervals_rebuild_batch($vids, $safe = NULL) {
  $batch = array(
    'finished' => 'taxonomy_nested_intervals_rebuild_finished',
    'file' => drupal_get_path('module', 'taxonomy_nested_intervals') . '/taxonomy_nested_intervals.rebuild.inc',
    'title' => t('Rebuilding taxonomy nested intervals'),
    'operations' => array(),
    'init_message' => t('Rebuilding taxonomy nested intervals'),
  );
  $safe = isset($safe) ? $safe : variable_get('taxonomy_nested_intervals_rebuild_safe_sibling', TAXONOMY_NESTED_INTERVALS_REBUILD_SAFE_SIBLING);
  foreach ($vids as $vid) {
    $batch['operations'][] = array('taxonomy_nested_intervals_rebuild_count_depth', array($vid));
    $batch['operations'][] = array(
      $safe ? 'taxonomy_nested_intervals_rebuild_enumerate_siblings_safe' : 'taxonomy_nested_intervals_rebuild_enumerate_siblings',
      array($vid)
    );
    $batch['operations'][] = array('taxonomy_nested_intervals_rebuild_matrices', array($vid));
    $batch['operations'][] = array('taxonomy_nested_intervals_rebuild_cleanup', array($vid));
  }
  batch_set($batch);
}

/**
 * Start batch job for rebuild of left/right values.
 */
function taxonomy_nested_intervals_rebuild_left_right_batch($vids) {
  $batch = array(
    'finished' => 'taxonomy_nested_intervals_rebuild_finished',
    'file' => drupal_get_path('module', 'taxonomy_nested_intervals') . '/taxonomy_nested_intervals.rebuild.inc',
    'title' => t('Rebuilding taxonomy nested intervals'),
    'init_message' => t('Rebuilding taxonomy nested intervals'),
  );
  foreach ($vids as $vid) {
    $batch['operations'][] = array('taxonomy_nested_intervals_rebuild_left_right', array($vid));
  }
  batch_set($batch);
}
/**
 * Finished function for rebuild tree batch operation.
 *
 * @param bool $success
 *   State of operations.
 * @param mixed $results
 *   Result data.
 * @param array $operations
 *   Operations.
 */
function taxonomy_nested_intervals_rebuild_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = theme('item_list', array('items' => $results));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE)
    ));
  }
  drupal_set_message($message, 'status');
}

/**
 * Count depth of tree.
 */
function taxonomy_nested_intervals_rebuild_count_depth($vid, &$context) {
  $class = taxonomy_nested_intervals_get_option($vid, 'class');

  if (!isset($context['sandbox']['depth'])) {
    variable_set("taxonomy_nested_intervals_invalid_$vid", FALSE);
    // First time: Clear out rebuild table, and initialize it tid:0

    _taxonomy_nested_intervals_db_delete_chunked(
      'taxonomy_nested_intervals_rebuild',
      $vid,
      variable_get('taxonomy_nested_intervals_delete_chunksize', TAXONOMY_NESTED_INTERVALS_DELETE_CHUNKSIZE)
    );
    $matrix = $class::getRoot();
    db_insert('taxonomy_nested_intervals_rebuild')
      ->fields(array(
        'vid' => $vid,
        'tid' => 0,
        'parent' => 0,
        'nv' => $matrix->nv,
        'dv' => $matrix->dv,
        'snv' => $matrix->snv,
        'sdv' => $matrix->sdv,
        'distance' => -1
      ))
      ->execute();
    $context['sandbox']['depth'] = -1;
    $context['finished'] = 0.25;
    $context['message'] = t("Initialized rebuild buffer");
    $context['sandbox']['time'] = microtime(TRUE);
    return;
  }
  $depth = &$context['sandbox']['depth'];

  $query = db_select('taxonomy_term_hierarchy', 'h');
  $query->join('taxonomy_term_data', 'd', 'd.tid = h.tid');
  $query->join('taxonomy_nested_intervals_rebuild', 'r', 'r.tid = h.parent');
  $query->addExpression('r.id', 'parent_id');
  $query->fields('d', array('vid'));
  $query->fields('h', array('tid', 'parent'));
  $query->addExpression('1', 'nv');
  $query->addExpression('0', 'dv');
  $query->addExpression('0', 'snv');
  $query->addExpression('1', 'sdv');
  $query->addExpression('r.distance + 1', 'distance');
  $query->condition('r.distance', $depth);
  $query->condition('r.vid', $vid);

  if (taxonomy_nested_intervals_get_option($vid, 'sort')) {
    // Depth is off-by-one at this time (one too short).
    $order = $class::getPolarityByDistance($depth + 1) == 1 ? 'ASC' : 'DESC';
    switch (Database::getConnection()->databaseType()) {
      case 'mysql':
        $query->orderBy('h.parent', $order);
        $query->orderBy('d.weight', $order);
        $query->orderBy('d.name', $order);
        break;

      default:
        $query->fields('d', array('weight', 'name'));
        $query->orderBy('h.parent', $order);
        $query->orderBy('d.weight', $order);
        $query->orderBy('d.name', $order);
        $subquery = $query;
        $query = db_select($subquery, 's')
          ->fields('s', array(
            'vid',
            'parent_id',
            'tid',
            'parent',
            'nv',
            'dv',
            'snv',
            'sdv',
            'distance'
          ));
        break;

    }
  }

  // Walk through depth and count rows to determine when we're done.
  $rows = db_insert('taxonomy_nested_intervals_rebuild', array('return' => Database::RETURN_AFFECTED))
    ->from($query)
    ->execute();

  if ($rows == 0) {
    $context['results'][] = t('Counted depth %depth - took %time seconds', array(
                                '%depth' => $depth,
                                '%time' => sprintf("%.08f", microtime(TRUE) - $context['sandbox']['time'])
                            ));
    $context['sandbox']['max_depth'] = $depth;
    $context['sandbox']['depth'] = $depth--;
    $context['finished'] = 1;
  }
  else {
    $depth++;
    $context['message'] = t("Counted depth %depth", array('%depth' => $depth));
    // Since we don't know the depth level beforehand, we use a reciprocal value for progress.
    $context['finished'] = 1 - 1 / ($depth + 2);
  }
}

/**
 * Enumerate the siblings for each child per parent in a safe way.
 *
 * Ensures single step sibling even on multiple parents.
 */
function taxonomy_nested_intervals_rebuild_enumerate_siblings_safe($vid, &$context) {
  $context['sandbox']['time'] = microtime(TRUE);
  $parent = db_query("SELECT id FROM {taxonomy_nested_intervals_rebuild} WHERE vid = :vid AND tid = 0 AND parent = 0", array(':vid' => $vid))->fetchField();
  update_sibling($vid, $parent);
  $context['message'] = t('Enumerated siblings');
  $context['results'][] = t('Enumerated siblings - took %time seconds', array(
                              '%time' => sprintf("%.08f", microtime(TRUE) - $context['sandbox']['time'])
                          ));
}

/**
 * Update sibling.
 *
 * @param int $vid
 *   Vocabulary ID
 */
function update_sibling($vid, $parent = 0, $path = '', $depth = 0) {
  $scale = taxonomy_nested_intervals_get_option($vid, 'scale');

  $result = db_select('taxonomy_nested_intervals_rebuild', 'r')
    ->fields('r')
    ->condition('r.parent_id', $parent)
    ->orderBy('r.id', 'ASC')
    ->execute();
  $sibling = 1;
  foreach ($result as $term) {
    $term_path = $path ? "$path.$sibling" : $sibling;
    db_update('taxonomy_nested_intervals_rebuild')
      ->fields(array(
        'sibling' => $sibling
      ))
      ->condition('id', $term->id)
      ->execute();
    update_sibling($vid, $term->id, $term_path, $depth + 1);
    $sibling += $scale;
  }
}

/**
 * Enumerate the siblings for each child per parent.
 * @todo Fix sibling reset for multiple parents?
 */
function taxonomy_nested_intervals_rebuild_enumerate_siblings($vid, &$context) {
  $context['sandbox']['time'] = microtime(TRUE);

  $scale = taxonomy_nested_intervals_get_option($vid, 'scale');

  // Sanitize for db query.
  $vid = (int) $vid;
  $scale = (int) $scale;

  // Proper temporary table hack!
  $table = _taxonomy_nested_intervals_db_create_temporary_table(array(
    'fields' => array(
      'rebuild_id' => array(
        'type' => 'int',
        'size' => 'big',
        'default' => 0,
        'not null' => TRUE,
      ),
      'sibling' => array(
        'type' => 'int',
        'size' => 'big',
        'default' => 0,
        'not null' => TRUE,
      ),
    ),
    'primary key' => array('rebuild_id'),
  ));

  // Based on the auto incerement id column in taxonomy_nested_intervals_rebuild,
  // We generate the enumerated siblings in a temp table table.
  $parents = db_select('taxonomy_nested_intervals_rebuild', 'r')
    ->fields('r', array('vid', 'parent_id'))
    // Quoted $vid can (will?) result in "using temporary; using filesort" on MySQL.
    ->where("r.vid = $vid")
    ->condition('r.parent_id', 0, '>')
    ->groupBy('vid')
    ->groupBy('parent_id');
  $parents->addExpression('MIN(r.id)', 'start_id');

  $query = db_select('taxonomy_nested_intervals_rebuild', 'i');
  $query->join($parents, 'p', 'p.parent_id = i.parent_id');
  $query->addExpression('i.id', 'rebuild_id');
  $query->addExpression('(i.id - p.start_id) * :scale1 + :scale2', 'sibling', array(':scale1' => $scale, ':scale2' => $scale));
  $query->condition('i.vid', $vid);
  $query->condition('i.parent_id', 0, '>');
  db_insert($table)
    ->from($query)
    ->execute();

  // And then we update the original table with the correct values.
  db_update('taxonomy_nested_intervals_rebuild')
    ->expression('sibling', '(SELECT sibling FROM {' . $table . '} WHERE rebuild_id = id)')
    ->condition('vid', $vid)
    ->condition('parent_id', 0, '>')
    ->execute();

  $context['message'] = t('Enumerated siblings');
  $context['results'][] = t('Enumerated siblings - took %time seconds', array(
                              '%time' => sprintf("%.08f", microtime(TRUE) - $context['sandbox']['time'])
                          ));
  $context['finished'] = 1;
}

/**
 * Generate matrices.
 */
function taxonomy_nested_intervals_rebuild_matrices($vid, &$context) {
  $class = taxonomy_nested_intervals_get_option($vid, 'class');
  $factor = taxonomy_nested_intervals_get_option($vid, 'factor');

  // Ensure strings with decimals.
  $factor = BigNum::add($factor, 0, TAXONOMY_NESTED_INTERVALS_DECIMALS);
  $next = BigNum::add(1, 0, TAXONOMY_NESTED_INTERVALS_DECIMALS);

  if (!isset($context['sandbox']['depth'])) {
    // First time: Clear out table, and generate matrices one depth at a time.
    _taxonomy_nested_intervals_db_delete_chunked(
      'taxonomy_nested_intervals',
      $vid,
      variable_get('taxonomy_nested_intervals_delete_chunksize', TAXONOMY_NESTED_INTERVALS_DELETE_CHUNKSIZE)
    );

    $context['sandbox']['depth'] = 0;
    $context['sandbox']['terms'] = 0;
    $context['sandbox']['max_depth'] = db_query("SELECT MAX(distance) FROM {taxonomy_nested_intervals_rebuild} WHERE vid = :vid", array(':vid' => $vid))->fetchField() + 2;
    $context['finished'] = ($context['sandbox']['depth'] + 1) / $context['sandbox']['max_depth'];
    if ($context['finished'] < 1) {
      $context['message'] = t("Initialized matrix generation");
      $context['sandbox']['time'] = microtime(TRUE);
      $query = db_select('taxonomy_nested_intervals_rebuild', 'r')
        ->fields('r', array(
          'vid',
          'tid',
          'parent',
          'distance',
          'nv',
          'dv',
          'snv',
          'sdv'
        ));
      $query->addExpression('r.id', 'rebuild_id');
      $query->condition('r.vid', $vid);
      $query->condition('r.tid', 0);
      $query->condition('r.distance', -1);
      db_insert('taxonomy_nested_intervals')
        ->from($query)
        ->execute();
    }
    return;
  }
  $depth = &$context['sandbox']['depth'];


  // Walk through depth, calculating matrix per child based on childs sibling and parents matrix.

  // @fixme ... smelly code ... well, smellier than the rest, OK!
  // @todo Move this to the class file somehow.
  if ($class == 'MatrixTreeHazel') {
    $sql_nv = '(i.nv + i.snv * r.sibling)';
    $sql_dv = '(i.dv + i.sdv * r.sibling)';
    $sql_snv = "(i.nv + i.snv * (r.sibling + $next))";
    $sql_sdv = "(i.dv + i.sdv * (r.sibling + $next))";
    $sql_lft = "ROUND($sql_nv * $factor / $sql_dv, " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
    $sql_rgt = "ROUND($sql_snv * $factor / $sql_sdv, " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
  }
  elseif ($class == 'MatrixTreeTropashko') {
    $sql_nv = '(i.nv * r.sibling + i.snv)';
    $sql_dv = '(i.dv * r.sibling + i.sdv)';
    $sql_snv = '(i.nv)';
    $sql_sdv = '(i.dv)';
    $sql_lft = "ROUND($sql_nv * $factor / $sql_dv, " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
    $sql_rgt = "ROUND(($sql_nv + $sql_snv) * $factor / ($sql_dv + $sql_sdv), " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";

    if ($depth % 2 == 1) {
      list($sql_lft, $sql_rgt) = array($sql_rgt, $sql_lft);
    }
    else {
      $sql_lft .= ' + (CASE r.sibling WHEN 1 THEN 0.000000000000000000000000000001 ELSE 0 END)';
    }
  }

  $query = db_select('taxonomy_nested_intervals_rebuild', 'r');
  $query->join('taxonomy_term_data', 'd', 'd.tid = r.tid');
  $query->join('taxonomy_nested_intervals', 'i', 'i.rebuild_id = r.parent_id');
  $query->fields('r', array('vid', 'tid', 'parent', 'distance'));
  $query->addExpression('r.id', 'rebuild_id');
  $query->addExpression($sql_nv, 'nv');
  $query->addExpression($sql_dv, 'dv');
  $query->addExpression($sql_snv, 'snv');
  $query->addExpression($sql_sdv, 'sdv');
  $query->addExpression($sql_lft, 'lft');
  $query->addExpression($sql_rgt, 'rgt');
  $query->condition('r.vid', $vid);
  $query->condition('r.distance', $depth);


  $rows = db_insert('taxonomy_nested_intervals', array('return' => Database::RETURN_AFFECTED))
    ->from($query)
    ->execute();

  $context['sandbox']['terms'] += $rows;

  $context['message'] = t("Generated matrices for %terms terms", array('%terms' => $context['sandbox']['terms']));;

  $depth++;
  $context['finished'] = ($depth + 1) / $context['sandbox']['max_depth'];

  if ($context['finished'] >= 1) {
    db_delete('taxonomy_nested_intervals')
      ->condition('vid', $vid)
      ->condition('tid', 0)
      ->execute();
    $context['results'][] = t('Generated matrices for %terms terms - took %time seconds', array(
                                '%terms' => $context['sandbox']['terms'],
                                '%time' => sprintf("%.08f", microtime(TRUE) - $context['sandbox']['time'])
                            ));
    $context['finished'] = 1;
    return;
  }
}

/**
 * Update left/right values.
 */
function taxonomy_nested_intervals_rebuild_left_right($vid, &$context) {
  $context['sandbox']['time'] = microtime(TRUE);

  variable_set("taxonomy_nested_intervals_invalid_$vid", FALSE);

  $class = taxonomy_nested_intervals_get_option($vid, 'class');
  $factor = taxonomy_nested_intervals_get_option($vid, 'factor');

  // @todo Move this to the class file somehow.
  if ($class = 'MatrixTreeHazel') {
    $sql_lft = "ROUND(nv * $factor / dv, " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
    $sql_rgt = "ROUND(snv * $factor / sdv, " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
  }
  elseif ($class == 'MatrixTreeTropashko') {
    $sql_lft_org = $sql_lft = "ROUND(nv * $factor / dv, " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
    $sql_rgt = "ROUND((nv + snv) * $factor / (dv + sdv), " . TAXONOMY_NESTED_INTERVALS_DECIMALS . ")";
    $sql_lft = "(CASE WHEN distance % 2 = 0 THEN $sql_lft + (CASE WHEN (snv = 1 AND sdv <> 0 AND dv = 1) OR FLOOR(nv / snv) = 1 THEN 0.000000000000000000000000000001 ELSE 0 END) ELSE $sql_rgt END)";
    $sql_rgt = "(CASE WHEN distance % 2 = 0 THEN $sql_lft_org ELSE $sql_rgt END)";
  }

  db_update('taxonomy_nested_intervals')
    ->expression('lft', "($sql_lft)")
    ->expression('rgt', "($sql_rgt)")
    ->condition('vid', $vid)
    ->execute();

  $context['message'] = t('Generated left/right values');
  $context['results'][] = t('Generated left/right values - took %time seconds', array(
                              '%time' => sprintf("%.08f", microtime(TRUE) - $context['sandbox']['time'])
                          ));
  $context['finished'] = 1;
}

/**
 * Clean up rebuild table.
 */
function taxonomy_nested_intervals_rebuild_cleanup($vid, &$context) {
  $context['sandbox']['time'] = microtime(TRUE);
  _taxonomy_nested_intervals_db_delete_chunked(
    'taxonomy_nested_intervals_rebuild',
    $vid,
    variable_get('taxonomy_nested_intervals_delete_chunksize', TAXONOMY_NESTED_INTERVALS_DELETE_CHUNKSIZE)
  );
  $context['message'] = t('Cleaned up rebuild table');
  $context['results'][] = t('Cleaned up rebuild table - took %time seconds', array(
                              '%time' => sprintf("%.08f", microtime(TRUE) - $context['sandbox']['time'])
                          ));
  $context['finished'] = 1;
}

/**
 * Chunked "truncate" for a vocabulary.
 *
 * Can perform much faster than one single delete query.
 */
function _taxonomy_nested_intervals_db_delete_chunked($table, $vid, $size = 0) {
  if ($size <= 0) {
    db_delete($table)
      ->condition('vid', $vid)
      ->execute();
    return;
  }
  do {
    $ids = db_select($table, 'ids')
      ->fields('ids', array('id'))
      ->condition('vid', $vid)
      ->range(0, $size)
      ->execute()
      ->fetchAll(PDO::FETCH_COLUMN);
    if ($ids) {
      db_delete($table)
        ->condition('id', $ids, 'IN')
        ->execute();
    }
  } while ($ids);
}
