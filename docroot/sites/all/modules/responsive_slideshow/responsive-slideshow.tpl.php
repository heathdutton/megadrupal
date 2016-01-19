<?php

/**
 * @file
 * Theme file to handle carousel display.
 *
 * Variables passed:
 * - $responsive_slideshow_item_count:The variable indicating
 *   the number of contents available to be diplayed in the block.
 * - $responsive_slideshow_items: An array of slider items.
 *   Each row is an array of content.
 */
?>

<div class="col-md-12 column">
  <div class="carousel slide" data-ride="carousel" data-interval="<?php print check_plain(variable_get('responsive_slideshow_interval')); ?>" id="carousel-main">
    <ol class="carousel-indicators">
    <?php for ($i = 0; $i < $responsive_slideshow_item_count; $i++): ?>
      <li <?php if ($i == 0): ?> class="active" <?php endif; ?> data-slide-to="<?php print $i; ?>" data-target="#carousel-main"></li>
    <?php endfor; ?>
    </ol>
		<div class="carousel-inner">
    <?php  for ($i = 0; $i < $responsive_slideshow_item_count; $i++):
      $item_class = ($i == 0) ? 'item active' : 'item';
    ?>
		  <div class="<?php print $item_class; ?>">
			  <img class="home-img-slideshow" title="<?php print $responsive_slideshow_items[$i]['image']['title']; ?>" alt="<?php print $responsive_slideshow_items[$i]['image']['alt']; ?>" src="<?php print $responsive_slideshow_items[$i]['image']['pic']; ?>" />
			  <div class="carousel-caption">
          <h4><?php print l($responsive_slideshow_items[$i]['title'], 'node/' . $responsive_slideshow_items[$i]['nid']); ?></h4>
          <p>
            <?php
              print (isset($responsive_slideshow_items[$i]['description']) ? $responsive_slideshow_items[$i]['description'] : '') . ' ' . (isset($responsive_slideshow_items[$i]['link']) ? $responsive_slideshow_items[$i]['link'] : '');
            ?>
          </p>
			  </div>
		  </div>
    <?php endfor; ?>
		</div>
    <a class="left carousel-control" href="#carousel-main" data-slide="prev"><span class="glyphicon glyphicon-chevron-left"></span></a>
    <a class="right carousel-control" href="#carousel-main" data-slide="next"><span class="glyphicon glyphicon-chevron-right"></span></a>
	</div>
</div>
