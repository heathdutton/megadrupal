<?php
// $Id$

/*!
 * Dynamic display block module template: vsd-upright30 - content template
 * Copyright (c) 2008 - 2009 P. Blaauw All rights reserved.
 * Version 1.1 (01-SEP-2009)
 * Licenced under GPL license
 * http://www.gnu.org/licenses/gpl.html
 */

/**
 * @file
 * Dynamic display block module template: vsd-upright30 - content template
 *
 * Available variables:
 * - $views_slideshow_ddblock_slider_settings['origin']: From which module comes the block.
 * - $views_slideshow_ddblock_slider_settings['delta']: Block number of the block.
 *
 * - $views_slideshow_ddblock_slider_settings['template']: template name
 * - $views_slideshow_ddblock_slider_settings['output_type']: type of content
 *
 * - $views_slideshow_ddblock_slider_settings['slider_items']: array with slidecontent
 * - $views_slideshow_ddblock_slider_settings['slide_text_position']: of the text in the slider (top | right | bottom | left)
 * - $views_slideshow_ddblock_slider_settings['slide_direction']: direction of the text in the slider (horizontal | vertical )
 * - 
 * - $views_slideshow_ddblock_slider_settings['pager_content']: Themed pager content
 * - $views_slideshow_ddblock_slider_settings['pager_position']: position of the pager (top | bottom)
 *
 * notes: don't change the ID names, they are used by the jQuery script.
 */
$settings = $views_slideshow_ddblock_slider_settings;
// add Cascading style sheet
drupal_add_css($directory .'/custom/modules/views_slideshow_ddblock/' . $settings['template'] . '/views-slideshow-ddblock-cycle-'. $settings['template'] . '.css', 'template', 'all', FALSE);
?>
<!-- dynamic display block slideshow -->
<div id="views-slideshow-ddblock-<?php print $settings['delta'] ?>" class="views-slideshow-ddblock-cycle-<?php print $settings['template'] ?> clear-block">
 <div class="container clear-block border">
  <div class="container-inner clear-block border">
   <?php if ($settings['pager_position'] == "top") : ?>
    <!-- number pager --> 
    <?php print $views_slideshow_ddblock_pager_content ?>
   <?php else : ?>
    <?php if ($settings['pager2'] == 1 && $settings['pager2_position']['pager'] === 'pager'): ?>  
     <!-- prev next pager. -->
     <div id="views-slideshow-ddblock-prev-next-pager-<?php print $settings['delta'] ?>" class="prev-next-pager views-slideshow-ddblock-pager clear-block">
      <a class="prev" href="#"><?php print $settings['pager2_pager_prev']?></a>
      <a class="count"></a>
      <a class="next" href="#"><?php print $settings['pager2_pager_next']?></a>
     </div>
    <?php endif; ?>  
   <?php endif; ?> 
   <!-- slider content -->
   <div class="slider clear-block border">
    <div class="slider-inner clear-block border">
     <?php if ($settings['output_type'] == 'view_fields') : ?>
      <?php foreach ($views_slideshow_ddblock_slider_items as $slider_item): ?>
       <div class="slide clear-block border">
        <div class="slide-inner clear-block border">
         <?php print $slider_item['slide_image']; ?>
          <div class="slide-text slide-text-<?php print $settings['slide_direction'] ?> slide-text-<?php print $settings['slide_text_position'] ?> clear-block border">
           <div class="slide-text-inner clear-block border">
           <?php if ($settings['slide_text'] == 1) :?>
            <div class="slide-title slide-title-<?php print $settings['slide_direction'] ?> clear-block border">
             <div class="slide-title-inner clear-block border">
              <h2><?php print $slider_item['slide_title'] ?></h2>
             </div> <!-- slide-title-inner-->
            </div>  <!-- slide-title-->
            <div class="slide-body-<?php print $settings['slide_direction'] ?> clear-block border">
             <div class="slide-body-inner clear-block border">
              <p><?php print $slider_item['slide_text'] ?></p>
             </div> <!-- slide-body-inner-->
            </div>  <!-- slide-body-->
           <?php endif; ?>  
           <div class="slide-read-more slide-read-more-<?php print $settings['slide_direction'] ?> clear-block border">
            <p><?php print $slider_item['slide_read_more'] ?></p>
	         </div><!-- slide-read-more-->
          </div> <!-- slide-text-inner-->
         </div>  <!-- slide-text-->
        </div> <!-- slide-inner-->
       </div>  <!-- slide-->
      <?php endforeach; ?>
     <?php endif; ?>
    </div> <!-- slider-inner-->
   </div>  <!-- slider-->
   <?php if ($settings['pager_position'] == "bottom") : ?>
    <!-- number pager --> 
    <?php print $views_slideshow_ddblock_pager_content ?>
   <?php else : ?>
    <?php if ($settings['pager2'] == 1 && $settings['pager2_position']['pager'] === 'pager'): ?>  
     <!-- prev next pager. -->
     <div id="views-slideshow-ddblock-prev-next-pager-<?php print $settings['delta'] ?>" class="prev-next-pager views-slideshow-ddblock-pager clear-block">
      <a class="prev" href="#"><?php print $settings['pager2_pager_prev']?></a>
      <a class="count"></a>
      <a class="next" href="#"><?php print $settings['pager2_pager_next']?></a>
     </div>
    <?php endif; ?>  
   <?php endif; ?> 
  </div> <!-- container-inner-->
 </div> <!--container-->
</div> <!--  template -->
