/**
 * Implements hook_preprocess().
 */
function <?php print $basename; ?>_preprocess(&\$variables, \$hook) {
  ${1:static \$hooks;

  // Add contextual links to the variables, if the user has permission.

  if (!user_access('access contextual links')) {
    return;
  \}

  if (!isset(\$hooks)) {
    \$hooks = theme_get_registry();
  \}

  // Determine the primary theme function argument.
  if (isset(\$hooks[\$hook]['variables'])) {
    \$keys = array_keys(\$hooks[\$hook]['variables']);
    \$key = \$keys[0];
  \}
  else {
    \$key = \$hooks[\$hook]['render element'];
  \}

  if (isset(\$variables[\$key])) {
    \$element = \$variables[\$key];
  \}

  if (isset(\$element) && is_array(\$element) && !empty(\$element['#contextual_links'])) {
    \$variables['title_suffix']['contextual_links'] = contextual_links_view(\$element);
    if (!empty(\$variables['title_suffix']['contextual_links'])) {
      \$variables['classes_array'][] = 'contextual-links-region';
    \}
  \}}
}

$2