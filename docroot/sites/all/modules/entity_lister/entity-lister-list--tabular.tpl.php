<?php // An example of a template for tabular output.

if ($above) {
  print $pager;
}
?>

<table>
<?php print $headers ?>
<?php print $list ?>
</table>

<?php
if ($below) {
  print $pager;
}

if ($total) {
  print '<p>' . t('The query yielded @count items', array('@count' => $total)) . '</p>';
}
