<?php
/**
 * @file
 * OM Extended Regions Info
 *
 * All these will be added during preprocess page
 *
 * Properties:
 *
 * tag     - html markup tags, with the coming of HTML5, new 
 *           markup tags can replace the good old div tag.
 *
 *           More meaningful tags such as:
 *           section, aside, article, header, footer, nav
 *           See http://www.w3schools.com/html5/html5_reference.asp
 *
 *           IMPORTANT: 
 *           Make sure you use the OM HTML5 Subtheme
 *           for these tags to work properly
 *
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
 *  grids - If you turned on the 960 grid system in the .info file
 *          (12, 16, and 24 standard grids), you can automatically set
 *          the sizes of the regions just by putting the number of columns
 *          on each region (0 for disabled).
 *          Ex. 4, will output grid-4
 *
 *          Settings Override (values must be in '')
 *          Ex. '12-4', will output grid-12-4, overrides .info file when set to 16 or 24
 *              '16-4', will output grid-16-4, overrides .info file when set to 12 or 24
 *              '24-4', will output grid-24-4, overrides .info file when set to 12 or 16
 * 
 *          Depending on the presence of side bars the columns of each region can also be changed.
 *          a) 'grid'       => 16, - no side bar present
 *          b) 'grid-both   => 9,  - both side bars are present
 *          c) 'grid-first  => 12, - side bar first is present
 *          d) 'grid-second => 13, - side bar second is present
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
  $regions['header_block'] = array(
    'tag'    => 'div',
    'id'     => 'header-block',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
  ); 
  $regions['menu_bar'] = array(
    'tag'    => 'div',
    'id'     => 'menu-bar',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
  );
  $regions['highlighted'] = array(
    'tag'    => 'div',
    'id'     => 'highlighted',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
  ); 
  $regions['sidebar_first'] = array(
    'tag'    => 'div',
    'id'     => 'sidebar-first',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
  ); 
  $regions['sidebar_second'] = array(
    'tag'    => 'div',
    'id'     => 'sidebar-second',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
  ); 
  $regions['content'] = array(
    'tag'    => 'div',
    'id'     => 'content',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
    // these can be added to any region, see description above
    //'grid-both'   => 0,
    //'grid-first'  => 0,
    //'grid-second' => 0,
  ); 
  $regions['footer'] = array(
    'tag'    => 'div',
    'id'     => 'footer',
    'class'  => '',
    'inner'  => 0,
    'top'    => 0,
    'bottom' => 0,
    'grid'   => 0, 
  ); 
  return $regions;
}



