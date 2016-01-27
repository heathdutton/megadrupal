<?php
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<table id="cm_airing_schedule_table">
<?php foreach ($rows as $id => $row): ?>

    <?php print $row; ?>

<?php endforeach; ?>
</table>
 