<?php
/**
 * @file
 * Template file for the galleryformatter default formatter
 */

/**
 * $slides - Array containing all slide images, its sanatized title & alt ready to print, its hash id and the full image URL if you need it
 * $thumbs - Array containing all thumbnail images ready to print and their hash
 */
?>

<!-- Place somewhere in the <body> of your page -->
<?php if(!empty($slides)): ?>
<div id="slider" class="flexslider">
  <ul class="slides">
  <?php foreach ($slides as $id => $data): ?>
      <li>
      <?php print $data['image']; ?>
      </li>
    <?php endforeach; ?>
    <!-- items mirrored twice, total of 12 -->
  </ul>
</div>
<?php endif; ?>
<?php if(!empty($thumbs)): ?>
<div id="carousel" class="flexslider">
  <ul class="slides">
      <?php foreach ($thumbs as $id => $data): ?>
        <li>
          <?php print $data['image']; ?>
        </li>
      <?php endforeach; ?>
    </ul>
</div>
<?php endif; ?>
