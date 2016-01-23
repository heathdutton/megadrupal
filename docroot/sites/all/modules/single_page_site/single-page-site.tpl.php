<?php
/**
 * Template to display single page site.
 */
$items = $variables['items'];
$wrapper = $variables['wrapper'];
?>
<div id="<?php print $wrapper ?>">
    <?php foreach ($items as $item): ?>
      <div id="<?php print $item['anchor']; ?>" class="single-page-wrapper" data-active-item="<?php print $item['anchor']; ?>" data-menu-item="<?php print $item['hide']; ?>">
          <div class="single-page">
              <<?php print $item['tag']; ?> class="single-page-title"><?php print $item['title']; ?></<?php print $item['tag']; ?>>
              <?php print $item['output']; ?>
          </div>
      </div>
    <?php endforeach; ?>
</div>
