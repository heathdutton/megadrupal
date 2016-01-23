<?php
/**
 * @file
 * Field Group: Easy Responsive Tabs to Accordion.
 *
 * Available variables:
 * - $is_empty: Boolean: any content to render at all?
 * - $wrapper_classes: The classes set on the wrapping div.
 * - $nav_classes: The classes set on the nav ul.
 * - $navs: An array of nav elements
 * - $pane_classes: The classes set on the panes containing content.
 * - $panes: An array of panes containing content.
 *
 * @ingroup themeable
 */
?>

<?php if (!$is_empty) : ?>
  <div class="<?php print $wrapper_classes; ?>">
    <div id="<?php print $identifier; ?>">
      <ul class="resp-tabs-list <?php print $nav_classes; ?> <?php print $tabidentify; ?>">
        <?php foreach ($navs as $index => $nav) : ?>
          <li><?php print $nav['content']; ?></li>
        <?php endforeach; ?>
      </ul>

      <div class="resp-tabs-container <?php print $pane_classes; ?> <?php print $tabidentify; ?>">
        <?php foreach ($panes as $index => $pane) : ?>
          <div><?php print $pane['content']; ?></div>
        <?php endforeach; ?>
      </div>
    </div>
  </div>
<?php endif; ?>
