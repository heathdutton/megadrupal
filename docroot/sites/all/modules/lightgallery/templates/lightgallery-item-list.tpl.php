<?php
/**
 * @file
 * Template to display a light gallery item list.
 */
$items = $variables['items'];
$id = $variables['unque_id'];
?>
<ul class="lightgallery" id="lightgallery-<?php print $id; ?>">
    <?php foreach ($items as $item): ?>
      <li data-src="<?php print $item['lightgallery_image_style']; ?>" data-sub-html="">
          <a href="">
              <img class="img-responsive" src="<?php print $item['lightgallery_image_thumb_style']; ?>" />
          </a>
      </li>
    <?php endforeach; ?>
</ul>