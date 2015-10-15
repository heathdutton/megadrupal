/**
 * Generate the HTML for a menu tree.
 *
 * @param \$pid
 *   The parent id of the menu.
 *
 * @ingroup themeable
 */
function <?php print $basename; ?>_menu_tree(\$pid = 1) {
  if (\$tree = menu_tree(\$pid)) {
    return "\n<ul class=\"menu\">\n". \$tree ."\n</ul>\n";
  }
}

$1