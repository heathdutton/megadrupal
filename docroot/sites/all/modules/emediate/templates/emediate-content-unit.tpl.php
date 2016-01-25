<?php
/**
 * @file
 * Template file for presenting an Emediate content unit
 */
?>

<!-- Emediate Content Unit #begin: <?php print "$content_unit->name - [$content_unit->cuid] - $content_unit->label"; ?> -->
<script type="text/javascript">
  if (Drupal.emediate.bannersProcessed == 0) {
    Drupal.emediate.initialize();
  }
</script>
<script type="text/javascript">
  if (Drupal.emediate.bannersProcessed == 0) {
    Drupal.emediate.startMoveGlobalData();
  }
  Drupal.emediate.bannersProcessed++;
</script>

<script type="text/javascript">
  Drupal.emediate.banner(<?php print $index; ?>)
</script>
<!-- Emediate Content Unit #end: <?php print "$content_unit->name - [$content_unit->cuid] - $content_unit->label"; ?> -->
