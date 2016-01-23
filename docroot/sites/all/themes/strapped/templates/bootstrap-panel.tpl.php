<?php
/**
 * @file
 * bootstrap-panel.tpl.php
 *
 * Markup for Bootstrap panels ([collapsible] fieldsets).
 */
?>

<?php
// Print the panels prefix.
// print ($prefix ? $prefix : '');
?>

<div <?php print $attributes; ?>>
  <?php if ($title && $title_display == 'before') { ?>
    <div class="panel-heading">
      <?php if ($collapsible): ?>
        <a class="title" href="#<?php print $id; ?>" data-toggle="collapse">
          <?php print $title . $title_tooltip . $title_popover; ?>
        </a>
      <?php else: ?>
          <?php print $title . $title_tooltip . $title_popover; ?>
      <?php endif; ?>
      </div>
  <?php } ?>

  <?php if ($collapsible) { ?>
     <div id="<?php print $id; ?>" class="panel-content panel-collapse collapse fade<?php print (!$collapsed ? ' in' : ''); ?>">
  <?php } ?>

        <div class="panel-body">
          <?php if ($description) { ?>
            <p class="help-block">
              <?php print $description; ?>
            </p>
          <?php } ?>
          <?php print $content; ?>
        </div>

   <?php if ($collapsible) { ?>
     </div>
   <?php } ?>
</div>


