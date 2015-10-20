<?php
?>

<h4>Date</h4>
<?php print $date ?>

<h4>ID</h4>
<?php print $id ?>

<?php if ($items): ?>
  <?php print $items ?>
<?php endif?>

<?php if ($notes): ?>
  <h4>Notes</h4>
  <?php print $notes ?>
<?php endif?>

<?php if ($actions): ?>
  <h4>Actions</h4>
  <?php print $actions ?>
<?php endif?>

<?php if ($activity): ?>
  <h4><?php print t('Payment activity')?></h4>
  <?php print $activity ?>
<?php endif?>
