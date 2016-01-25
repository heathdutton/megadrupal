<?php
/*

 MagazineLayout.class.php

 Introduction
 ============
 A class for creating magazine-like layouts for images. A Magazine-like layout arranges the images at
 different sizes so that all images fit within a defined "square box". This can be an attractive way
 of arranging images when you are dealing with user-uploaded images, or don't have a graphic designer
 handy to arrange and resize them in photoshop.

 Purpose
 =======
 The obvious use for this script is anywhere where more than one user submitted image needs to be presented
 in a HTML page. I'm thinking product databases, forum image uploads, random image rotations, etc etc.
 Once you have 10 or so images, you are better off using an AJAX based image gallery, but this script will
 fill the gap nicely up till that point.

 Layouts
 =======
 The layouts that are used depend on the number of landscape and portrait images. For example, if we are
 given a portrait and 2 landscapes, the layout will appear as follows (different numbers represent different
 images)...

 11113333
 11113333
 22223333
 22223333

 With 3 landscapes, the layout may appear as such...

 11112222
 11112222
 33333333
 33333333

 With 2 portraits, 1 landscape we use

 11122222222333  or  111222
 11122222222333      111222
 11122222222333      333333
                     333333

 With 3 portraits, we could use either

 111222333  or   11333
 111222333       11333
 111222333       22333
                 22333

 If you have 4 images to display, this class will use any of the following...

 111222   112233   111444
 111222   112233   222444
 333444   444444   333444
 333444   444444

 Logic
 =====
 The logic behind these calculations are based on algebra - yes, x + y = z (but a little more complicated).
 I have attempted to clearly document all calculations, however you will find the tools at http://www.quickmath.com/
 very useful if you aren't a mathematics expert (I'm certainly not one).

 Requirements
 ============
 -A PHP 4.3.x server with GD2.x extension enabled - most PHP shared hosting is suitable
 -An image resizing script - I have included a very simple one with this bundle

 Usage
 =====

 //include the class file
 require_once('magazinelayout.class.php');

 //Define the width for the output area (pixels)
 $width = 600;

 //Define padding around each image - this *must* be included in your stylesheet (pixels)
 $padding = 3;

 //Define your template for outputting images
 $template = "<img src=\"image.php?size=[size]&amp;file=[image]\" alt=\"\" />"; //Don't forget to escape the &

 //create a new instance of the class
 $mag = new magazinelayout($width,$padding,$template);

 //Add the images in any order
 $mag->addImage('landscape1.jpg');
 $mag->addImage('portrait1.jpg');
 $mag->addImage('landscape2.jpg');

 //display the output
 echo $mag->getHtml();

 Template
 ========
 A different <img> tag will be required for different installations, depending mainly on the image script used.
 Variables:
 [size] = The size of the image, either 500, h500 or w500
 [file] = The filename of the image, eg images/image1.jpg

 The default is...
 <img src="image.php?size=[size]&file=[image]" alt="" />

 Using Apache's mod_rewrite, a better format might be...
 <image src="images/[size]/[file]" alt="" />
 Note your script and server must be configured for this, details of this are out of the scope of this script

 A static looking image URL is better because Google will usually ignore dynamic looking images (they look
 like PHP scripts, not images)


 CSS
 ===
 The following CSS is required for padding to work correctly...

 .magazine-image {
  background: #fff;
  border: 1px #eee solid;
 }
 .magazine-image img {
  padding: 0px;
  background: #fff;
  margin: 2px;
  border: 1px #eee solid;
 }

 Padding
 =======
 Including padding between images was the most complicated part of this script. On the more complex layouts,
 the equations double in complexity once padding is added. Padding is implemented as x pixels gap between the
 images - x is defined when the class is created.
 The padding you specify *must* be reflected in the stylesheet you use, or the layout will not look right.
 Because IE deals with padding incorrectly, padding has been implemented as "margin" instead. If a padding of
 3 is specified in the PHP class, then the CSS class for ".magazine-image img" should reflect a margin of 3px
 or a margin o 2px + border of 1px. Do not specify padding on the image unless you are prepared to hack the PHP
 code.

 Rounding
 ========
 Almost all of the calculations are done using floating point numbers. Because HTML required whole numbers, the
 numbers need to be rounded (down) before outputting. On some examples, this makes no difference. On others, the
 1 or 2 pixels worth of rounding is noticeable. Would welcome any suggestions on this issue for those who want
 pixel perfect layouts.

 Restrictions
 ============
 -There is a current limit of 8 images. This can easily be extended.
 -Images must be a reasonable quality. Images that are too small are stretched which doesn't look good.
 -The included image resizing script is very basic. I recommend using a script which caches output. The
  image resizing script that is right for you will depend on your server configuration and is out of
  the scope of this class.

 To Do
 =====
 There are several obvious improvements that can be made to this script. These include...
 -Adding code for more than 8 images
 -Configuring so that low-res images are always shown in the small spots
 -Allow positioning of images (though this does defeat the purpose)
 -Include an all-purpose image resizing script with static looking URLs and Image caching
 -Full testing for version 1 release
 -Rounding issue explained above

 Copyright
 =========
 This file may be used and distributed subject to the LGPL available from http://www.fsf.org/copyleft/lgpl.html
 If you find this script useful, I would appreciate a link back to http://www.ragepank.com or simply an email
 to let me know you like it :)
 You are free (and encouraged) to modify or enhance this class - if you do so, I would appreciate a copy
 of the changes if it's not too much trouble. Please keep this copyright statement intact, and give fair
 credit on any derivative work.

 About the Author
 ================
 Harvey Kane is a PHP Web developer living and working in Auckland New Zealand. He is interested in developing
 "best practice" websites, and especially interested in using PHP to automate this process as much as
 possible. Harvey works as a freelance developer doing CMS websites and SEO under the umbrella of www.harveykane.com,
 and publishes SEO Articles and tools at www.ragepank.com

 Ragepank.com
 ============
 www.ragepank.com is a source of original SEO Articles and tools - PHP based techniques for improving
 search engine positions, and good practice for web development.

 Support
 =======
 I am happy to help with support and installation, so long as all documentation and forum threads are read
 first. You can contact me at info@ragepank.com

 Thanks
 ======
 Thanks to Alexander Burkhardt (www.alex3d.de) for the use of the demo images. The images were taken on
 the lovely Hokianga Harbour in Northland New Zealand.

 @version 0.9
 @copyright 2006 Harvey Kane
 @author Harvey Kane info@ragepank.com

 */


class MagazineLayout {

  /**
   * Configuration options provided by constructor.
   *
   * @var array
   */
  private $options = array(
    'block_width' => 600,
    'block_height' => NULL,
    'block_images_count' => 7,
    'image_padding' => 3,
    'randomize_layout' => TRUE,
  );

  /**
   * Array of image attributes received from external function.
   *
   * @var array
   */
  var $images = array();

  private $_blocks = array();

  /**
   * Array of already processed blocks.
   *
   * @var array
   */
  var $_processed_blocks;

  /**
   * Array of already processed images.
   *
   * @var array
   */
  var $_processed_images;

  /**
   * Initial total height of single rendered block, before any further
   * processing, based on heights of all images added to the block. This
   * could undergo further changes if a specific final height of the block
   * was requested through $_block_height value.
   *
   * @var int
   */
  var $_rendered_images_height;

  /**
   * Total height of rendered vertical padding around images.
   * This will not change anymore, even if specific final block height
   * was requested and image heights are still to be changed.
   *
   * @var int
   */
  var $_rendered_padding_height;

  /**
   * Image layout definitions.
   *
   * @var array
   */
  var $_layouts = array(
    // One image.
    1 => array(
      'default' => array(
        0 => array(
          array('_get1a', 0),
        ),
      ),
    ),
    // Two images.
    2 => array(
      'default' => array(
        0 => array(
          array('_get2a', 0, 1),
        ),
      ),
    ),
    // Three images.
    3 => array(
      'LLL' => array(
        0 => array(
          array('_get3b', 0, 1, 2),
        ),
        1 => array(
          array('_get2a', 1, 2),
          array('_get1a', 0),
        ),
      ),
      'default' => array(
        0 => array(
          array('_get3b', 0, 1, 2),
        ),
      ),
    ),
    // Four images.
    4 => array(
      'LLLP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 3),
        ),
      ),
      'LPPP' => array(
        0 => array(
          array('_get3a', 1, 2, 3),
          array('_get1a', 0),
        ),
      ),
      'default' => array(
        0 => array(
          array('_get2a', 2, 0),
          array('_get2a', 1, 3),
        ),
      ),
    ),
    // Five images.
    5 => array(
      'LLLLL' => array(
        0 => array(
          array('_get3a', 0, 1, 2),
          array('_get2a', 3, 4),
        ),
      ),
      'LLLLP' => array(
        0 => array(
          array('_get3b', 0, 1, 4),
          array('_get2a', 2, 3),
        ),
      ),
      'LLLPP' => array(
        0 => array(
          array('_get3b', 0, 1, 4),
          array('_get2a', 2, 3),
        ),
      ),
      'LLPPP' => array(
        0 => array(
          array('_get3b', 2, 3, 4),
          array('_get2a', 0, 1),
        ),
      ),
      'LPPPP' => array(
        0 => array(
          array('_get3b', 2, 3, 4),
          array('_get2a', 0, 1),
        ),
      ),
      'PPPPP' => array(
        0 => array(
          array('_get2a', 4, 0),
          array('_get3a', 1, 2, 3),
        ),
      ),
    ),
    // Six images.
    6 => array(
      'LLLLLL' => array(
        0 => array(
          array('_get2a', 0, 1),
          array('_get2a', 2, 3),
          array('_get2a', 4, 5),
        ),
      ),
      'LLLLLP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 5),
          array('_get2a', 3, 4),
        ),
      ),
      'LLLLPP' => array(
        0 => array(
          array('_get3b', 0, 1, 4),
          array('_get3b', 2, 3, 5),
        ),
      ),
      'LLLPPP' => array(
        0 => array(
          array('_get3b', 0, 1, 5),
          array('_get3b', 2, 3, 4),
        ),
      ),
      'LLPPPP' => array(
        0 => array(
          array('_get3b', 0, 2, 4),
          array('_get3b', 1, 3, 5),
        ),
      ),
      'LPPPPP' => array(
        0 => array(
          array('_get3b', 0, 1, 5),
          array('_get3a', 2, 3, 4),
        ),
      ),
      'PPPPPP' => array(
        0 => array(
          array('_get3a', 3, 4, 5),
          array('_get3a', 0, 1, 2),
        ),
      ),
    ),
    // Seven images.
    7 => array(
      'LLLLLLL' => array(
        0 => array(
          array('_get3a', 0, 1, 2),
          array('_get2a', 3, 4),
          array('_get2a', 5, 6),
        ),
      ),
      'LLLLLLP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 6),
          array('_get3a', 3, 4, 5),
        ),
      ),
      'LLLLLPP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 5),
          array('_get3b', 3, 4, 6),
        ),
      ),
      'LLLLPPP' => array(
        0 => array(
          array('_get3b', 0, 1, 5),
          array('_get4b', 2, 3, 4, 6),
        ),
      ),
      'LLLPPPP' => array(
        0 => array(
          array('_get3b', 0, 1, 5),
          array('_get4b', 2, 3, 4, 6),
        ),
      ),
      'LLPPPPP' => array(
        0 => array(
          array('_get3a', 4, 5, 6),
          array('_get2a', 0, 1),
          array('_get2a', 2, 3),
        ),
      ),
      'LPPPPPP' => array(
        0 => array(
          array('_get3a', 0, 1, 2),
          array('_get4b', 3, 4, 5, 6),
        ),
      ),
      'PPPPPPP' => array(
        0 => array(
          array('_get4a', 0, 1, 2, 3),
          array('_get3b', 4, 5, 6),
        ),
      ),
    ),
    // Eight images.
    8 => array(
      'LLLLLLLL' => array(
        0 => array(
          array('_get3a', 0, 1, 2),
          array('_get2a', 3, 4),
          array('_get3a', 5, 6, 7),
        ),
      ),
      'LLLLLLLP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 7),
          array('_get2a', 3, 4),
          array('_get2a', 5, 6),
        ),
      ),
      'LLLLLLPP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 6),
          array('_get4b', 3, 4, 5, 7),
        ),
      ),
      'LLLLLPPP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 6),
          array('_get4b', 3, 4, 5, 7),
        ),
      ),
      'LLLLPPPP' => array(
        0 => array(
          array('_get4b', 0, 1, 2, 6),
          array('_get4b', 3, 4, 5, 7),
        ),
      ),
      'LLLPPPPP' => array(
        0 => array(
          array('_get3a', 4, 5, 6),
          array('_get2a', 0, 1),
          array('_get3a', 2, 3, 7),
        ),
      ),
      'LLPPPPPP' => array(
        0 => array(
          array('_get3b', 5, 6, 7),
          array('_get2a', 0, 1),
          array('_get3b', 2, 3, 4),
        ),
      ),
      'LPPPPPPP' => array(
        0 => array(
          array('_get3b', 5, 6, 7),
          array('_get2a', 0, 1),
          array('_get3b', 2, 3, 4),
        ),
      ),
      'PPPPPPP' => array(
        0 => array(
          array('_get4a', 0, 1, 2, 3),
          array('_get4a', 4, 5, 6, 7),
        ),
      ),
      'default' => array(
        0 => array(
          array('_get3b', 5, 4, 7),
          array('_get2a', 1, 0),
          array('_get3b', 2, 3, 6),
        ),
      ),
    ),
  );


  /**
   * Constructor.
   *
   * $options may be:
   * - block_width: fixed width of single image block (required, default 600px)
   * - block_height: fixed height of single image block (optional)
   * - padding: default padding in pixels around each image
   * - block_template: HTML template for single image block
   * - image_template: HTML template for single image
   *
   * @param array $options
   */
  public function __construct($options = NULL) {

    // Make sure that we can process received options.
    if (!is_array($options) && !$options instanceof Traversable) {
      throw new Exception(__FUNCTION__ . ' expects an array or Traversable');
    }

    // Add received __construct parameters to class options,
    // but allow only those already predefined by this class.
    foreach ($options as $name => $value) {
      if (in_array($name, array_keys($this->options))) {
        $this->options[$name] = $value;
      }
    }
  }

  /**
   * Returns file extension from provided file name / file path.
   *
   * @param string $file
   * @return string
   */
  private function _getFileExt($file) {
    $exploded = explode(".", $file);
    return strtolower(array_pop($exploded));
  }

  /**
   * Converts a 2D array from $arr[a][b] to $arr[b][a].
   * Used for sorting the array.
   *
   * @param array $arr
   * @return array
   */
  private function _transpose($arr) {
    foreach($arr as $key_x => $valx) {
      foreach($valx as $key_y => $valy) {
        $newarr[$key_y][$key_x] = $valy;
      }
    }
    return $newarr;
  }

  /**
   * Adds single image to the processing queue.
   *
   * @param array $attributes
   * @return array
   */
  public function addImage($attributes) {

    // No path, no processing.
    if (empty($attributes['path'])) {
      return FALSE;
    }

    // Ensure the file is an image.
    if (!in_array($this->_getFileExt($attributes['path']), array('jpg', 'jpeg', 'gif', 'png'))) {
      return FALSE;
    }

    // Get image dimensions if not provided.
    if (empty($attributes['width']) || empty($attributes['height'])) {
      $imagesize = getimagesize($attributes['path']);
      $attributes['width'] = $imagesize[0];
      $attributes['height'] = $imagesize[1];
    }

    // Don't include zero-sized images.
    if ($attributes['width'] == 0 || $attributes['height'] == 0) {
      return FALSE;
    }

    // Find the ration of width:height.
    $attributes['ratio'] = $attributes['width'] / $attributes['height'];

    // Set format based on the dimensions.
    $attributes['format'] = ($attributes['width'] > $attributes['height']) ? 'landscape' : 'portrait';

    $this->images[] = $attributes;

    // Provide fluent interface.
    return $this;
  }

  /**
   * Makes sure that processed block height is the same as height
   * specified in options when the class was initiated.
   */
  private function _checkBlockHeight() {
    if ($this->_rendered_images_height + $this->_rendered_padding_height != $this->options['block_height']) {
      $ratio = $this->_rendered_images_height / ($this->options['block_height'] - $this->_rendered_padding_height);
      foreach ($this->_processed_blocks[$this->_current_block_id] as $image_id => $image) {
        $this->_processed_blocks[$this->_current_block_id][$image_id]['height'] = round($image['height'] / $ratio);
      }
    }
  }

  /**
   * Makes sure that all images have correct heights, so we don't have
   * any of those 1 (or more) pixel imperfections.
   */
  private function _checkImageHeights($pointer = 0) {
    // Get the layout from the first image of the subblock we're checking.
    $subblock_layout = $this->_processed_blocks[$this->_current_block_id][$pointer]['layout'];

    // Let's make the code nicer and shorter.
    $images = $this->_processed_blocks[$this->_current_block_id];

    // We do not have to check all layout, only few of them.
    switch ($subblock_layout) {
      case '3b':

        $this->_processed_blocks[$this->_current_block_id][$pointer + 2]['height'] = $images[$pointer]['height'] - $images[$pointer + 1]['height'] - 2 * $this->options['image_padding'];
        break;
      case '4b':
        $this->_processed_blocks[$this->_current_block_id][$pointer + 3]['height'] = $images[$pointer]['height'] - $images[$pointer + 1]['height'] - $images[$pointer + 2]['height'] - 4 * $this->options['image_padding'];
        break;
    }

    // If number of images in current block is higher than number of images
    // in sub-block we have just checked, it means there is another sub-block
    // which should be checked as well.
    $subblock_element_count = substr($subblock_layout, 0, 1);
    if (count($this->_processed_blocks[$this->_current_block_id]) > $subblock_element_count + $pointer) {
      $this->_checkImageHeights($pointer + $subblock_element_count);
    }
  }

  /*
  IMAGE LAYOUTS
  =============
  These layouts are coded based on the number of images.
  Some fairly heavy mathematics is used to calculate the image sizes and the excellent calculators at
  http://www.quickmath.com/ were very useful.
  Each of these layouts outputs a small piece of HTML code with the images and a containing div
  around each.
  */


  private function _get1a($i1) {
    /*
    111 or 1
           1
    */

    $w1 = floor($this->options['block_width'] - ($this->options['image_padding'] * 2));
    $h1 = floor($w1 / $this->_blocks[$this->_current_block_id][$i1]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i1], array(
      'width'     => $w1,
      'height'    => $h1,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    $this->_rendered_images_height += $h1;
    $this->_rendered_padding_height += $this->options['image_padding'] * 2;

    // Provide fluent interface.
    return $this;
  }

  private function _get2a($i1, $i2) {
    /*
    1122

    Equation: t = 4p + ha + hb Variable: h

    */

    $a = $this->_blocks[$this->_current_block_id][$i1]['ratio'];
    $b = $this->_blocks[$this->_current_block_id][$i2]['ratio'];
    $t = $this->options['block_width'];
    $p = $this->options['image_padding'];

    $h1 = floor( (4*$p - $t) / (-$a - $b) );

    // Left image.
    $w1 = floor($h1 * $this->_blocks[$this->_current_block_id][$i1]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i1], array(
      'height'    => $h1,
      'width'     => $w1,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Right image.
    // $w2 = $h1 * $this->_blocks[$this->_current_block_id][$i2]['ratio'];
    $w2 = $this->options['block_width'] - $w1 - 4 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i2], array(
      'height'    => $h1,
      'width'     => $w2,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    $this->_rendered_images_height += floor($h1);
    $this->_rendered_padding_height += $this->options['image_padding'] * 2;

    // Provide fluent interface and allow for method chaining.
    return $this;
  }

  private function _get3a($i1, $i2, $i3) {
    /*
    1223
    */

    /* To save space in the equation */
    $a = $this->_blocks[$this->_current_block_id][$i3]['ratio'];
    $b = $this->_blocks[$this->_current_block_id][$i1]['ratio'];
    $c = $this->_blocks[$this->_current_block_id][$i2]['ratio'];
    $t = $this->options['block_width'];
    $p = $this->options['image_padding'];

    /*
    Enter the following data at http://www.hostsrv.com/webmab/app1/MSP/quickmath/02/pageGenerate?site=quickmath&s1=equations&s2=solve&s3=advanced#reply
    EQUATIONS
    t = 6p + ah + bh + ch
    VARIABLES
    h
    */

    $h1 = floor(
      (6 * $p - $t)
      /
      (-$a -$b -$c)
    );

    // Left image.
    $w1 = round($h1 * $this->_blocks[$this->_current_block_id][$i1]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i1], array(
      'height'    => $h1,
      'width'     => $w1,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Middle image.
    $w3 = round($h1 * $this->_blocks[$this->_current_block_id][$i3]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i3], array(
      'height'    => $h1,
      'width'     => $w3,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Right image.
    // Image height calculated from its new width and original ratio...
    // $w2 = round($h1 * $this->_blocks[$this->_current_block_id][$i2]['ratio']);
    // ...but instead we'll use number of pixels needed to fill up the block.
    $w2 = $this->options['block_width'] - $w1 - $w3 - 6 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i2], array(
      'height'    => $h1,
      'width'     => $w2,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));

    $this->_rendered_images_height += floor($h1);
    $this->_rendered_padding_height += $this->options['image_padding'] * 2;

    // Provide fluent interface and allow for method chaining.
    return $this;
  }

  private function _get3b($i1, $i2, $i3) {
    /*
    1133
    2233
    */

    /* To save space in the equation */
    $a = $this->_blocks[$this->_current_block_id][$i3]['ratio'];
    $b = $this->_blocks[$this->_current_block_id][$i1]['ratio'];
    $c = $this->_blocks[$this->_current_block_id][$i2]['ratio'];
    $t = $this->options['block_width'];
    $p = $this->options['image_padding'];

    /*
    Enter the following data at http://www.hostsrv.com/webmab/app1/MSP/quickmath/02/pageGenerate?site=quickmath&s1=equations&s2=solve&s3=advanced#reply
    EQUATIONS
    x/a = w/b + w/c + 2p
    w+x+4p = t
    VARIABLES
    w
    x
    */

    /* width of left column with 2 small images */
    $w1 = floor(
    -(
    (2 * $a * $b * $c * $p + 4 * $b * $c * $p - $b * $c * $t)
    /
    ($a * $b + $c * $b + $a * $c)
    )
    );

    /* width of right column with 1 large image */
    $w2 = floor(
    ($a * (-4 * $b * $p + 2 * $b * $c * $p - 4 * $c * $p + $b * $t + $c * $t))
    /
    ($a * $b + $c * $b + $a * $c)
    );

    // Right big image.
    $h3 = floor($w2 / $this->_blocks[$this->_current_block_id][$i3]['ratio']);
    $float = 'right';
    if (!empty($this->options['randomize_layout'])) {
      $float = array_rand(array_flip(array('left', 'right')));
    }
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i3], array(
      'width'     => $w2,
      'height'    => $h3,
      'float'     => $float,
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Left top image.
    $h1 = floor($w1 / $this->_blocks[$this->_current_block_id][$i1]['ratio']);
    $w1 = $this->options['block_width'] - $w2 - 4 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i1], array(
      'width'     => $w1,
      'height'    => $h1,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Left bottom image.
    // $h2 = floor($w1 / $this->_blocks[$this->_current_block_id][$i2]['ratio']);
    $h2 = $h3 - $h1 - 2 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i2], array(
      'width'     => $w1,
      'height'    => $h2,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    $this->_rendered_images_height += floor($w2 / $this->_blocks[$this->_current_block_id][$i3]['ratio']);
    $this->_rendered_padding_height += $this->options['image_padding'] * 2;

    // Provide fluent interface and allow for method chaining.
    return $this;
  }



  private function _get4a($i1, $i2, $i3, $i4) {
    /*
    1234
    */

    /* To save space in the equation */
    $a = $this->_blocks[$this->_current_block_id][$i1]['ratio'];
    $b = $this->_blocks[$this->_current_block_id][$i2]['ratio'];
    $c = $this->_blocks[$this->_current_block_id][$i3]['ratio'];
    $d = $this->_blocks[$this->_current_block_id][$i4]['ratio'];
    $t = $this->options['block_width'];
    $p = $this->options['image_padding'];

    /*
    Enter the following data at http://www.hostsrv.com/webmab/app1/MSP/quickmath/02/pageGenerate?site=quickmath&s1=equations&s2=solve&s3=advanced#reply
    EQUATIONS
    t = 6p + ah + bh + ch + dh
    VARIABLES
    h
    */

    $h1 = floor(
      (8 * $p - $t)
      /
      (-$a -$b -$c -$d)
    );

    //$h1 = floor($this->options['block_width'] / ($this->_blocks[$this->_current_block_id][$p1]['ratio'] + $this->_blocks[$this->_current_block_id][$p2]['ratio'] + $this->_blocks[$this->_current_block_id][$p3]['ratio'] + $this->_blocks[$this->_current_block_id][$p4]['ratio']));

    // Left image.
    $w1 = floor($h1 * $this->_blocks[$this->_current_block_id][$i1]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i1], array(
      'height'    => $h1,
      'width'     => $w1,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Middle-left image.
    $w2 = floor($h1 * $this->_blocks[$this->_current_block_id][$i2]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i2], array(
      'height'    => $h1,
      'width'     => $w2,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Middle-right image.
    $w3 = floor($h1 * $this->_blocks[$this->_current_block_id][$i3]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i3], array(
      'height'    => $h1,
      'width'     => $w3,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Right image.
    // $w4 = floor($h1 * $this->_blocks[$this->_current_block_id][$i4]['ratio']);
    $w4 = $this->options['block_width'] - $w1 - $w2 - $w3 - 8 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i4], array(
      'height'    => $h1,
      'width'     => $w4,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    $this->_rendered_images_height += floor($h1);
    $this->_rendered_padding_height += $this->options['image_padding'] * 2;

    // Provide fluent interface and allow for method chaining.
    return $this;
  }

  private function _get4b($i1, $i2, $i3, $i4) {
    /*
    11444
    22444
    33444
    */

    /* To save space in the equation */
    $a = $this->_blocks[$this->_current_block_id][$i4]['ratio'];
    $b = $this->_blocks[$this->_current_block_id][$i1]['ratio'];
    $c = $this->_blocks[$this->_current_block_id][$i2]['ratio'];
    $d = $this->_blocks[$this->_current_block_id][$i3]['ratio'];
    $t = $this->options['block_width'];
    $p = $this->options['image_padding'];

    /*
    Enter the following data at http://www.hostsrv.com/webmab/app1/MSP/quickmath/02/pageGenerate?site=quickmath&s1=equations&s2=solve&s3=advanced#reply
    EQUATIONS
    x/a = w/b + w/c + 2p
    w+x+4p = t
    VARIABLES
    w
    x
    */

    /* width of left column with 3 small images */
    $w1 = floor(
    -(
    (4 * $a * $b * $c * $d * $p + 4 * $b * $c * $d * $p - $b * $c * $d * $t)
    /
    ($a * $b * $c + $a * $d * $c + $b * $d * $c + $a * $b * $d)
    )
    );

    /* width of right column with 1 large image */
    $w2 = floor(
    -(
    (-4 * $p - (-(1/$c) -(1/$d) -(1/$b)) * (4 * $p - $t) )
    /
    ( (1/$b) + (1/$c) + (1/$d) + (1/$a) )
    )
    );

    // Right big image.
    $h4 = floor($w2 / $this->_blocks[$this->_current_block_id][$i4]['ratio']);
    $float = 'right';
    if (!empty($this->options['randomize_layout'])) {
      $float = array_rand(array_flip(array('left', 'right')));
    }
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i4], array(
      'width'     => $w2,
      'height'    => $h4,
      'float'     => $float,
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Left-top image.
    $h1 = floor($w1 / $this->_blocks[$this->_current_block_id][$i1]['ratio']);
    $w1 = $this->options['block_width'] - $w2 - 4 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i1], array(
      'width'     => $w1,
      'height'    => $h1,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Left middle image.
    $h2 = floor($w1 / $this->_blocks[$this->_current_block_id][$i2]['ratio']);
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i2], array(
      'width'     => $w1,
      'height'    => $h2,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    // Left bottom image.
    // $h3 = floor($w1 / $this->_blocks[$this->_current_block_id][$i3]['ratio']);
    $h3 = $h4 - $h1 - $h2 - 4 * $p;
    $this->_processed_blocks[$this->_current_block_id][] = array_merge($this->_blocks[$this->_current_block_id][$i3], array(
      'width'     => $w1,
      'height'    => $h3,
      'float'     => 'left',
      'block_id'  => $this->_current_block_id,
      'layout'    => str_replace('_get', '', __FUNCTION__),
    ));
    $this->_rendered_images_height += floor($w2 / $this->_blocks[$this->_current_block_id][$i4]['ratio']);
    $this->_rendered_padding_height += $this->options['image_padding'] * 2;

    // Provide fluent interface and allow for method chaining.
    return $this;
  }

  /**
   * Processes all images within a single block.
   */
  private function _processBlockImages() {

    // Nasty feature of array_multisort() function is re-indexing numeric array
    // keys, which we could happen to have in the transposed version of our array,
    // so let's remember all original values and re-assing them right afterwards.
    // @TODO: Think of more generic method of doing this?
    $originals = array();
    foreach ($this->_blocks[$this->_current_block_id] as $image) {
      $originals[$image['fid']] = $image;
    }

    // Sort the images array: landscape first, then portrait.
    $images = $this->_transpose($this->_blocks[$this->_current_block_id]);
    array_multisort($images['format'], SORT_STRING, SORT_ASC, $images['fid'], $images['ratio']);
    $images = $this->_transpose($images);

    // Re-assign original values to sorted array.
    foreach ($images as $key => $value) {
      $images[$key] = $originals[$value['fid']];
    }

    $this->_blocks[$this->_current_block_id] = $images;

    // Profile explains the makeup of the images (landscape vs portrait)
    // so we can use the best layout eg. LPPP or LLLP.
    $profile = '';
    foreach ($images as $i) {
      $profile .= $i['format'] == 'landscape' ? 'L' : 'P';
    }

    // Array that all processed block images will be added to.
    $this->_processed_blocks[$this->_current_block_id] = array();

    // Rendered images and padding heights will be used to calculate total
    // block height in case we want to render our block with specific height.
    $this->_rendered_images_height = 0;
    $this->_rendered_padding_height = 0;

    // Get layout definitions available for the number of images
    // in the current block.
    if (!empty($this->_layouts[count($images)])) {
      $layout_profiles = $this->_layouts[count($images)];
    }
    else {
      return FALSE;
    }

    // Get layout definition for the current profile (eg. LPPP or LLLP).
    if (!empty($layout_profiles[$profile])) {
      $layout_definitions = $layout_profiles[$profile];
    }
    elseif (!empty($layout_profiles['default'])) {
      $layout_definitions = $layout_profiles['default'];
    }
    else {
      return FALSE;
    }

    // Select one random definition from available definitions.
    $layout_definition_id = 0;
    if (!empty($this->options['randomize_layout'])) {
      $layout_definition_id = mt_rand(0, count($layout_definitions) - 1);
    }
    $layout_definition = $layout_definitions[$layout_definition_id];

    // Randomize order.
    if (!empty($this->options['randomize_layout'])) {
      shuffle($layout_definition);
    }

    // Process images based on the selected definition.
    foreach ($layout_definition as $arguments) {
      $function = array_shift($arguments);
      call_user_func_array(array($this, $function), $arguments);
    }

    // If block height was set in options when class was initiated,
    // make sure that processed block has the same height.
    if (!empty($this->options['block_height'])) {
      $this->_checkBlockHeight();
    }

    // Make sure that all images have correct heights, so we don't have
    // any of those 1 (or more) pixel imperfections.
    $this->_checkImageHeights();
  }

  /**
   * Loops through each block to be processed
   * and calls image processing method for each one.
   */
  private function _processBlocks() {
    foreach (array_keys($this->_blocks) as $block_id) {
      // Processing methods need to know which block we are currently processing.
      $this->_current_block_id = $block_id;
      $this->_processBlockImages();
    }
  }

  /**
   * Splits initial array of all images into blocks based
   * on number of images in one block defined in class settings,
   * and calls blocks processing method.
   */
  private function _processImages() {
    $this->_blocks = array_chunk($this->images, $this->options['block_images_count']);
    $this->_processBlocks();
  }

  /**
   * Returns array of processed blocks.
   */
  public function getProcessedBlocks() {
    $this->_processImages();
    return $this->_processed_blocks;
  }

}
