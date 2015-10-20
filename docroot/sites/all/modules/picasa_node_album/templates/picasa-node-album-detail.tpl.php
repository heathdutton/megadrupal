<?php

/**
 * @file
 * Picasa node album template file
 *
 * $nid is the node these albums belong to.
 * $node_url - the path to the node the album belongs to.
 * $title - the title of the album
 * $uid - the user id of the album submitter
 * $author - the name/link of the album submitter
 * $postedby - a posted by string
 * $images - array of image data
 */
?>
<div class="album">
  <div class="posted-by"><?php print $postedby; ?></div>

  <?php if (count($images) == 0): ?>
    <p>No Images</p>
  <?php endif; ?>

  <?php foreach($images as $image): ?>
    <a rel="lightbox[album]" href="<?php print $image['image']; ?>"><img src="<?php print $image['thumbnail']; ?>" hspace="2" /></a>
  <?php endforeach; // end $'images foreach loop ?>

</div><!-- end album class -->