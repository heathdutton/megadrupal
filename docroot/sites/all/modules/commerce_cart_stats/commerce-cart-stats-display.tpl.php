<?php

/**
* @file
* Template for the Commerce Cart Stats module.
*
* Available variables:
* - $header: The table's headers.
* - $rows: The table's data.
* - $caption: The table's caption.
*/
?>

<h2><?php print $caption; ?></h2>


<table style="width: 60%;">
  <tr>

<?php
foreach ($header as $key => $value) {
  print '<th>' . $value . '</th>';
}
?>
  </tr>


<?php
print '<tr>';
foreach ($rows as $key => $value) {
  if (is_array($value)) {
    foreach ($value as $subkey => $subvalue) {
      print '<td>' . $subvalue . '</td>';
    }
    print '</tr><tr>';
  } else {
    print '<td>' . $value . '</td>';
  }
}
print "</tr>\n";
?>


</table>
