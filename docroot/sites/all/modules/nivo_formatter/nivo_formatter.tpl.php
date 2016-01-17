<?php
/**
 * @file nivo_formatter.tpl.php
 * Main Nivo Slider template
 * 
 * Variables avaiable:
 * - $nivo_slider_theme: A name of current theme.
 * - $images: An array of ready to print image.
 * 
 */
?>
<div class="slider-wrapper theme-<?php print $nivo_slider_theme; ?> controlnav-thumbs">
  <div class="ribbon"></div>
  <div id="slider<?php print '-' . $field_name; ?>" class="nivoSlider <?php $isThumb ? print 'nivoSlider-thumb' : FALSE; ?>">
  <?php if($images): ?>
  <?php foreach($images as $id => $image): ?>
  	<?php if(isset($raws[$id]) && !empty($raws[$id]['url'])): ?>
  		
  		<a href="<?php echo $raws[$id]['url']; ?>" target="<?php echo isset($raws[$id]['target']) ? $raws[$id]['target'] : '_self'; ?>">
  			<?php print $image; ?>
  		</a>
  		
  	<?php else: ?>
  	
    	<?php print $image; ?>
    	
    <?php endif; ?>
  <?php endforeach; ?>
  <?php endif; ?>
  </div>
</div>