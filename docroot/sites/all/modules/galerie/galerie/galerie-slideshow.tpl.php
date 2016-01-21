<div id="node-<?php echo $node->nid ?>" class="galerie-wrapper galerie-slideshow-wrapper galerie-<?php echo $galerie_type ?>">
  <div class="galerie-slideshow-thumbnails-wrapper">
    <div class="galerie-slideshow-thumbnails">
      <span id="controls"><span id="back">«</span><span id="play">▶</span><span id="pause">▍▍</span></span>
      <?php echo $thumbnails ?>
    </div>
  </div>
  <div class="galerie-slideshow-viewer-wrapper">
    <div class="galerie-slideshow-viewer">
      <?php echo $image ?>
    </div>
  </div>
</div>
