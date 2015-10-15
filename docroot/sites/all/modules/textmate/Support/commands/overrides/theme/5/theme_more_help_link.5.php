/**
 * Returns code that emits the 'more help'-link.
 */
function <?php print $basename; ?>_more_help_link(\$url) {
  return '<div class="more-help-link">' . t('[<a href="@link">more help...</a>]', array('@link' => check_url(\$url))) . '</div>';
}

$1