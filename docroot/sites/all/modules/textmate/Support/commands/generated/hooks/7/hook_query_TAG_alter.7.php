/**
 * Implements hook_query_TAG_alter().
 */
function <?php print $basename; ?>_query_TAG_alter(QueryAlterableInterface \$query) {
  ${1:// Skip the extra expensive alterations if site has no node access control modules.
  if (!node_access_view_all_nodes()) {
    // Prevent duplicates records.
    \$query->distinct();
    // The recognized operations are 'view', 'update', 'delete'.
    if (!\$op = \$query->getMetaData('op')) {
      \$op = 'view';
    \}
    // Skip the extra joins and conditions for node admins.
    if (!user_access('bypass node access')) {
      // The node_access table has the access grants for any given node.
      \$access_alias = \$query->join('node_access', 'na', '%alias.nid = n.nid');
      \$or = db_or();
      // If any grant exists for the specified user, then user has access to the node for the specified operation.
      foreach (node_access_grants(\$op, \$query->getMetaData('account')) as \$realm => \$gids) {
        foreach (\$gids as \$gid) {
          \$or->condition(db_and()
            ->condition(\$access_alias . '.gid', \$gid)
            ->condition(\$access_alias . '.realm', \$realm)
          );
        \}
      \}

      if (count(\$or->conditions())) {
        \$query->condition(\$or);
      \}

      \$query->condition(\$access_alias . 'grant_' . \$op, 1, '>=');
    \}
  \}}
}

$2