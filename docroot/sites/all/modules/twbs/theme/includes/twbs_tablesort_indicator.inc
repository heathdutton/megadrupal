<?php

/**
 * Returns HTML for a sort icon.
 *
 * @param $variables
 *   An associative array containing:
 *   - style: Set to either 'asc' or 'desc', this determines which icon to
 *     show.
 */
function twbs_tablesort_indicator($variables) {
  if ($variables['style'] == "asc") {
    return ' <i class="fa fa-sort-asc"></i>';
  }
  else {
    return ' <i class="fa fa-sort-desc"></i>';
  }
}
