<?php
// $Id$

/**
 * @file
 * Theme implementation to display getfeatureinfo result
 *
 */
print '<div class="layers">';
if (isset($tables)) {
  foreach ($tables as $name => $table) {
    $name = str_replace(":", " ", $name);
    print '<div class="' . $name . '">';
    print theme_table(
      Array(
        'header' => $table['header'],
        'rows' => $table['rows'],
        'attributes' => Array('class' => Array($name)),
        'colgroups' => Array(),
        'caption' => $name,
        'sticky' => TRUE,
        'empty' => t("No results found")
      )
    );
    print '</div>';
  }
}
else {
  print t("No results found");
}
print '</div>';