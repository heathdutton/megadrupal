/**
 * Implements hook_field_formatter_prepare_view().
 */
function <?php print $basename; ?>_field_formatter_prepare_view(\$entity_type, \$entities, \$field, \$instances, \$langcode, &\$items, \$displays) {
  ${1:\$tids = array();

  // Collect every possible term attached to any of the fieldable entities.
  foreach (\$entities as \$id => \$entity) {
    foreach (\$items[\$id] as \$delta => \$item) {
      // Force the array key to prevent duplicates.
      \$tids[\$item['tid']] = \$item['tid'];
    \}
  \}

  if (\$tids) {
    \$terms = taxonomy_term_load_multiple(\$tids);

    // Iterate through the fieldable entities again to attach the loaded term
    // data.
    foreach (\$entities as \$id => \$entity) {
      \$rekey = FALSE;

      foreach (\$items[\$id] as \$delta => \$item) {
        // Check whether the taxonomy term field instance value could be loaded.
        if (isset(\$terms[\$item['tid']])) {
          // Replace the instance value with the term data.
          \$items[\$id][\$delta]['taxonomy_term'] = \$terms[\$item['tid']];
        \}
        // Otherwise, unset the instance value, since the term does not exist.
        else {
          unset(\$items[\$id][\$delta]);
          \$rekey = TRUE;
        \}
      \}

      if (\$rekey) {
        // Rekey the items array.
        \$items[\$id] = array_values(\$items[\$id]);
      \}
    \}
  \}}
}

$2