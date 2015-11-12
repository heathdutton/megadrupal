<?php
// $Id$ 

/*!
 * Dynamic display block module template: vsd-upright20 - pager template
 * Copyright (c) 2008 - 2009 P. Blaauw All rights reserved.
 * Version 1.0 (19-AUG-2009)
 * Licenced under GPL license
 * http://www.gnu.org/licenses/gpl.html
 */

/**
 * @file
 * Dynamic display block module template: vsd-upright20 - pager template
 * - prev/next pager
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
<!-- prev next pager. -->
<?php if ($settings['pager2'] == 1 && $settings['pager2_position']['pager'] === 'pager'): ?>  
 <!-- prev next pager. -->
 <div id="views-slideshow-ddblock-prev-next-pager-<?php print $settings['delta'] ?>" class="prev-next-pager views-slideshow-ddblock-pager clear-block">
  <a class="prev" href="#"><?php print $settings['pager2_pager_prev']?></a>
  <a class="count"></a>
  <a class="next" href="#"><?php print $settings['pager2_pager_next']?></a>
 </div>
<?php endif; ?>  
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




