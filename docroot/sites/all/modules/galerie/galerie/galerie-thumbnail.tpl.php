<a href="<?php echo $image['thumb-link'] ?>" class="<?php echo $class ?>" rel="<?php echo $rel ?>" title="<a href='<?php echo $image['link'] ?>'><?php echo $image['title'] ?></a>"><img
  title="<?php echo $image['title'] ?>"
  alt="<?php echo ($image['title'] ? $image['title'] : $image['id']) ?>"
  id="galerie-<?php echo $image['id'] ?>"
  src="<?php echo $image['thumb-src'] ?>"
  height="75"
  width="75"
/></a><?php echo $actions ?>
