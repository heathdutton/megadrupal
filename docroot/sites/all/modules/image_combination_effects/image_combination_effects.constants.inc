<?php
/**
 * @file
 *   <p>Constants for the Image Combination Effects module.</p>
 *   
 *   <p>© 2012 RedBottle Design, LLC. All rights reserved.</p>
 *   
 *   <p>All module code is released under the GNU General Public License.
 *   See COPYRIGHT.txt and LICENSE.txt.</p>
 * 
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 * @license GNU GPLv2
 */

/**
 * The machine name of this module.
 * 
 * @var string
 */
define('IMCOEF_MODULE_NAME', 'image_combination_effects');

/**
 * The ICE Image effect option for rendering the images horizontally,
 * left-to-right.
 * 
 * @var string
 */
define('IMCOEF_OPTION_HORIZONTAL_LEFT_TO_RIGHT', 'horiz-ltr');

/**
 * The ICE Image effect option for rendering the images horizontally,
 * right-to-right.
 * 
 * @var string
 */
define('IMCOEF_OPTION_HORIZONTAL_RIGHT_TO_LEFT', 'horiz-rtl');

/**
 * The ICE Image effect option for rendering the images vertically,
 * top-to-bottom.
 * 
 * @var string
 */
define('IMCOEF_OPTION_VERTICAL_TOP_TO_BOTTOM', 'vert-ttb');

/**
 * The ICE Image effect option for rendering the images vertically,
 * bottom-to-top.
 * 
 * @var string
 */
define('IMCOEF_OPTION_VERTICAL_BOTTOM_TO_TOP', 'vert-btt');

/**
 * The ICE target option for the expected width of each image in the output
 * image.
 * 
 * @var string
 */
define('IMCOEF_TARGET_WIDTH', 'width');

/**
 * The ICE target option for the expected height of each image in the output
 * image.
 * 
 * @var string
 */
define('IMCOEF_TARGET_HEIGHT', 'height');

/**
 * The ICE target option for the zero-based index/position of the image being
 * placed in the output image.
 * 
 * @var string
 */
define('IMCOEF_TARGET_DELTA', 'delta');

/**
 * The ICE target option for the direction in which images should be laid out
 * in the output image.
 * 
 * @var string
 */
define('IMCOEF_TARGET_ORIENTATION', 'orientation');