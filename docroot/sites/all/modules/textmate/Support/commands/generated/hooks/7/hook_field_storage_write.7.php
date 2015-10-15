/**
 * Implements hook_field_storage_write().
 */
function <?php print $basename; ?>_field_storage_write(\$entity_type, \$entity, \$op, \$fields) {
  ${1:list(\$id, \$vid, \$bundle) = entity_extract_ids(\$entity_type, \$entity);
  if (!isset(\$vid)) {
    \$vid = \$id;
  \}

  foreach (\$fields as \$field_id) {
    \$field = field_info_field_by_id(\$field_id);
    \$field_name = \$field['field_name'];
    \$table_name = _field_sql_storage_tablename(\$field);
    \$revision_name = _field_sql_storage_revision_tablename(\$field);

    \$all_languages = field_available_languages(\$entity_type, \$field);
    \$field_languages = array_intersect(\$all_languages, array_keys((array) \$entity->\$field_name));

    // Delete and insert, rather than update, in case a value was added.
    if (\$op == FIELD_STORAGE_UPDATE) {
      // Delete languages present in the incoming \$entity->\$field_name.
      // Delete all languages if \$entity->\$field_name is empty.
      \$languages = !empty(\$entity->\$field_name) ? \$field_languages : \$all_languages;
      if (\$languages) {
        db_delete(\$table_name)
          ->condition('entity_type', \$entity_type)
          ->condition('entity_id', \$id)
          ->condition('language', \$languages, 'IN')
          ->execute();
        db_delete(\$revision_name)
          ->condition('entity_type', \$entity_type)
          ->condition('entity_id', \$id)
          ->condition('revision_id', \$vid)
          ->condition('language', \$languages, 'IN')
          ->execute();
      \}
    \}

    // Prepare the multi-insert query.
    \$do_insert = FALSE;
    \$columns = array('entity_type', 'entity_id', 'revision_id', 'bundle', 'delta', 'language');
    foreach (\$field['columns'] as \$column => \$attributes) {
      \$columns[] = _field_sql_storage_columnname(\$field_name, \$column);
    \}
    \$query = db_insert(\$table_name)->fields(\$columns);
    \$revision_query = db_insert(\$revision_name)->fields(\$columns);

    foreach (\$field_languages as \$langcode) {
      \$items = (array) \$entity->{\$field_name\}[\$langcode];
      \$delta_count = 0;
      foreach (\$items as \$delta => \$item) {
        // We now know we have someting to insert.
        \$do_insert = TRUE;
        \$record = array(
          'entity_type' => \$entity_type,
          'entity_id' => \$id,
          'revision_id' => \$vid,
          'bundle' => \$bundle,
          'delta' => \$delta,
          'language' => \$langcode,
        );
        foreach (\$field['columns'] as \$column => \$attributes) {
          \$record[_field_sql_storage_columnname(\$field_name, \$column)] = isset(\$item[\$column]) ? \$item[\$column] : NULL;
        \}
        \$query->values(\$record);
        if (isset(\$vid)) {
          \$revision_query->values(\$record);
        \}

        if (\$field['cardinality'] != FIELD_CARDINALITY_UNLIMITED && ++\$delta_count == \$field['cardinality']) {
          break;
        \}
      \}
    \}

    // Execute the query if we have values to insert.
    if (\$do_insert) {
      \$query->execute();
      \$revision_query->execute();
    \}
  \}}
}

$2