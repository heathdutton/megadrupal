<?php foreach ($photos as $photo): ?>
<div class="photo-wrapper">
  <a class="photo-thumb ''" target="_blank" title="" href="">
   <span class="photo-thumb-wrapper">
     <i data-photo-id="<?php print $photo['id']; ?>" style="width: <?php print $settings['photoThumbWidth'] . 'px'; ?>; height:<?php print $settings['photoThumbHeight'] . 'px'; ?>;"></i>
   </span>
  </a>
</div>
<?php endforeach; ?>
