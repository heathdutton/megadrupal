<?php
/**
 * @file
 * OM Extended Regions Info
 *
 * All these will be added during preprocess page
 *
 * Properties:
 *  id     - Region ID (lower case and words separated with dash)
 *           ex. 'sidebar-first',
 *
 *  class  - Region Classes (lower case and words separated with dash)
 *           Automatically added: 'region region-region-name'
 *           ex. 'row row-1 first', 'column column-1 last',

 *  inner  - Automatically added: (0 - without, 1 - with)
 *             <div id ="region-name-inner" class="region-inner">...
 *             </div><!-- /#region-name-inner -->
 *
 *  top    - Automatically added: (0 - without, 1 - with)
 *           <div class="region-top">
 *             <div class="region-top-left"></div>
 *             <div class="region-top-right"></div>
 *           </div>
 *
 *  bottom - Automatically added: (0 - without, 1 - with)
 *           <div class="region-bottom">
 *             <div class="region-bottom-left"></div>
 *             <div class="region-bottom-right"></div>
 *           </div>
 *
 * JS and CSS for each region
 *   If you have a particular js and css for each region
 *   just place them in /js and /css folders,
 *   and name them like the region name, 
 *   ex. your_theme/js/sidebar_first.js and 
 *       your_theme/css/sidebar_first.css
 *
 */

function om_regions_get_info() {

	$regions = array();
  $regions['content'] = array(
    'id'     => 'content',
    'class'  => '',
    'inner'  => 1,
    'top'    => 0,
    'bottom' => 0,
  ); 
  return $regions;
}



