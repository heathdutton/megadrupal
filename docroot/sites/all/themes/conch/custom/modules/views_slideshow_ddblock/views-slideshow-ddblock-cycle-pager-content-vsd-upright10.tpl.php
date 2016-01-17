<?php
// $Id$ 

/*!
 * Dynamic display block module template: vsd-upright10 - pager template
 * Copyright (c) 2008 - 2009 P. Blaauw All rights reserved.
 * Version 1.0 (19-AUG-2009)
 * Licenced under GPL license
 * http://www.gnu.org/licenses/gpl.html
 */

/**
 * @file
 * Dynamic display block module template: vsd-upright10 - pager template
 * - Number pager
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
?>
<?php if ($settings['pager_position'] == 'bottom'): ?>
 <div class="spacer-horizontal"><b></b></div>
<?php endif; ?>
<!-- number pager -->
<div id="views-slideshow-ddblock-<?php print $settings['pager'] ."-". $settings['delta'] ?>" class="<?php print $settings['pager'] ?> views-slideshow-ddblock-pager clear-block">
 <?php $item_counter=1; ?>
 <ul>
  <?php if ($settings['pager2'] == 1 && $settings['pager2_position']['pager'] === 'pager'): ?>
   <li class="number-pager-prev pager-prev">
    <a class="prev" href="#"><?php print $settings['pager2_pager_prev']?></a>
   </li>
  <?php endif; ?> 
  <?php foreach ($views_slideshow_ddblock_pager_items as $item): ?>
   <li class="number-pager-item">
    <a href="#" class="pager-link" title="click to navigate to topic">
     <?php print $item_counter; ?>
    </a>
   </li>
   <?php $item_counter++;?>
  <?php endforeach; ?>
  <?php if ($settings['pager2'] == 1 && $settings['pager2_position']['pager'] === 'pager'): ?>
   <li class="number-pager-next pager-next">
    <a class="next" href="#"><?php print $settings['pager2_pager_next']?></a>
   </li>
  <?php endif; ?> 
 </ul>
</div> 
<div class="number-pager-pre-<?php print $settings['pager_position']; ?> "></div>
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
