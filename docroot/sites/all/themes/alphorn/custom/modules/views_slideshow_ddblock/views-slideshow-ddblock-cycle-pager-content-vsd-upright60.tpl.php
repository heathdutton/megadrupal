<?php
// $Id$ 

/*!
 * Dynamic display block module template: vsd-upright60 - pager template
 * Copyright (c) 2008 - 2009 P. Blaauw All rights reserved.
 * Version 1.0 (19-AUG-2009)
 * Licenced under GPL license
 * http://www.gnu.org/licenses/gpl.html
 */

/**
 * @file
 * Dynamic display block module template: vsd-upright60 - pager template
 * - Scrollable pager with images
 *
 * Available variables:
 * - $views_slideshow_ddblock_pager_settings['delta']: Block number of the block.
 * - $views_slideshow_ddblock_pager_settings['pager']: Type of pager to add
 * - $views_slideshow_ddblock_pager_settings['pager2']: Add prev/next pager
 * - $views_slideshow_ddblock_pager_settings['pager_position']: position of the slider (top | bottom) 
 * - $views_slideshow_ddblock_pager_items: array with pager_items
 *
 * notes: don't change the ID names, they are used by the jQuery script.
 */
 $settings = $views_slideshow_ddblock_pager_settings;
 
// jquery_plugin_add('scrollable');
 $base = drupal_get_path('module', 'views_slideshow_ddblock');
 drupal_add_js($base . '/js/tools.scrollable-1.0.5.min.js', 'theme');
?>

<?php if ($settings['pager_position'] == 'bottom'): ?>
 <div class="spacer-horizontal"><b></b></div>
<?php endif; ?>

<!-- navigator --> 
<div class="navi"></div> 

<!-- custom "prev" link --> 
<div class="prev"></div> 

<!-- scrollable pager -->
<div id="views-slideshow-ddblock-scrollable-pager-<?php print $settings['delta'] ?>" class="<?php print $settings['pager'] ?> vsd-scrollable-pager clear-block border">
 <div class="items <?php print $settings['pager'] ?>-inner clear-block border">
  <?php if ($views_slideshow_ddblock_pager_items): ?>
   <?php $item_counter=1; ?>
   <?php foreach ($views_slideshow_ddblock_pager_items as $pager_item): ?>
    <div class="<?php print $settings['pager'] ?>-item <?php print $settings['pager'] ?>-item-<?php print $item_counter ?>">
      <a href="#" title="navigate to topic" class="pager-link"><?php print $pager_item['image'];?></a>
    </div> <!-- /custom-pager-item -->
    <?php $item_counter++; ?>
   <?php endforeach; ?>
  <?php endif; ?>
 </div> <!-- /pager-inner-->
</div>  <!-- /scrollable pager-->

<!-- custom "next" link --> 
<div class="next"></div>

<!-- prev/next page on slide -->
<?php if ($settings['pager2'] == 1 && $settings['pager2_position']['slide'] === 'slide'): ?>
 <div class="views-slideshow-ddblock-prev-next-slide">
  <div class="prev-container">
   <a class="prev" href="#"><?php print $settings['pager2_slide_prev']?></a>
  </div>
  <div class="next-container">
   <a class="next" href="#"><?php print $settings['pager2_slide_next'] ?></a>
  </div>
 </div>
<?php endif; ?> 

<?php if ($settings['pager_position'] == 'top'): ?>
 <div class="spacer-horizontal"><b></b></div>
<?php endif; ?>
