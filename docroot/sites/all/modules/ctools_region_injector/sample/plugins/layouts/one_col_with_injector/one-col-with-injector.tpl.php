<?php
/**
 * @file
 * Layout template.
 */

/**
 * The following region is the one that will not be displayed in the panel page,
 * but rather extracted into the site template, via the region_injector plugin.
 * In order to not display it in the panel page, we wrap it into a conditional
 * which displays the content only on the panel administration page.
 * So every injected region should be wrapped in this conditional.
 */
?>
<?php if (isset($renderer) && isset($renderer->admin) && $renderer->admin == TRUE): ?>
  <div class="panel-panel banner-area">
    <?php if ($content['banner_area']): ?>
      <?php print $content['banner_area']; ?>
    <?php endif ?>
  </div>
<?php endif; ?>

<?php
/**
 * A simple content region, that is always displayed.
 */
?>
<?php if ($content['content']): ?>
    <?php print $content['content']; ?>
<?php endif; ?>


