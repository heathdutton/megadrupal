<?php

/**
 * @file
 * Picasa node album template file
 *
 * $nid is the node these albums belong to.
 * $albums is an array containing all image data for the node.
 */
?>
<?php foreach ($albums as $key => $album): ?>

<div class="album">
  <div class="posted-by"><?php print $album['title']; ?> <?php print $album['postedby']; ?></div>

  <?php if (count($album['images']) == 0): ?>
    <p>No Images</p>
  <?php else: ?>

    <?php foreach($album['images'] as $image): ?>
      <a rel="lightbox[<?php print $key; ?>]" href="<?php print $image['image']; ?>"><img src="<?php print $image['thumbnail']; ?>" hspace="2" /></a>
    <?php endforeach; // end $album['images'] foreach loop ?>
    <?php if (isset($album['preview_mode'])): ?>
      <?php print $album['link']; ?> (<?php print $album['total_images']; ?>)
    <?php endif; ?>
  <?php endif; ?>

</div><!-- End album class -->

<?php endforeach; // end $albums foreach loop ?>
