<div class="galerie-image <?php echo $class ?>">
  <div class="galerie-image-wrapper"><a href="<?php echo $link ?>" target="_blank"><img
    title="<?php echo $title ?>"
    alt="<?php echo ($title ? $title : $id) ?>"
    id="galerie-<?php echo $id ?>"
    src="<?php echo $src ?>"
  /></a><div class="galerie-image-actions"><?php echo $actions ?></div></div>
  <div class="galerie-image-info">
    <h2><?php echo $title; ?></h2>
    <div class="galerie-image-description"><?php echo $description ?></div>
    <div class="galerie-image-extra"><?php echo $extra ?></div>
  </div>
</div>
