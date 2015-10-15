/**
 * Implements hook_field_storage_update_field().
 */
function <?php print $basename; ?>_field_storage_update_field(\$field, \$prior_field, \$has_data) {
  ${1:if (!\$has_data) {
    // There is no data. Re-create the tables completely.
    \$prior_schema = _field_sql_storage_schema(\$prior_field);
    foreach (\$prior_schema as \$name => \$table) {
      db_drop_table(\$name, \$table);
    \}
    \$schema = _field_sql_storage_schema(\$field);
    foreach (\$schema as \$name => \$table) {
      db_create_table(\$name, \$table);
    \}
  \}
  else {
    // There is data. See field_sql_storage_field_storage_update_field() for
    // an example of what to do to modify the schema in place, preserving the
    // old data as much as possible.
  \}
  drupal_get_schema(NULL, TRUE);}
}

$2