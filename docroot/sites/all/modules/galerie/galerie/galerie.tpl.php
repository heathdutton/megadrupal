<div class="galerie-wrapper galerie-<?php echo $galerie_type ?> galerie-style-<?php echo $galerie_style ?> galerie-<?php echo $view_mode ?>" data-galerie-nid="<?php echo $node->nid ?>">
  <div class="galerie-browser-wrapper">
    <div class="galerie-browser">
      <?php echo $thumbnails ?>
    </div>
    <div class="galerie-browser-more">
      <?php echo $more_link ?>
    </div>
  </div>
  <?php if ($galerie_style != 'colorbox'): ?>
  <div class="galerie-viewer-wrapper">
    <div class="galerie-viewer">
      <?php echo $image ?>
    </div>
  </div>
  <?php endif; ?>
</div>
