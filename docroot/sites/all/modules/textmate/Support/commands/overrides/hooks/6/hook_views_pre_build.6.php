/**
 * Implements hook_views_pre_build().
 */
function <?php print $basename; ?>_views_pre_build(&\$view) {
  switch (\$view->name) {
    case '${1:view_name}':
      ${2:// View modification code goes here.}
      break;
  }
}

$3