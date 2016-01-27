<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Create true slideshows using any image over internet with many other features.
 */

/**
 * Get all images on a given directory
 *
 * @param $path
 *  String. The absolute path to start the scanner
 * @param $path
 *  Boolean. TRUE for recursive scanning
 * @return
 *  Array. All images' paths
 */
function _slideshow_creator_explore_dir($path = '', $recursive = FALSE) {
  // The path should be
  $path = variable_get('file_directory_path', conf_path() . '/files') . $path;

  // A list of image extensions. It should be on lower and
  // upper cases to work on non GNU systems, like Solaris and Windows
  $exts = array(
    'png',
    'gif',
    'jpeg',
    'jpg',
    'bmp',
    'PNG',
    'GIF',
    'JPEG',
    'JPG',
    'BMP',
  );

  // Get all files from a given path
  $files = array();
  foreach ($exts as $ext) {
    if ($new_files = glob("$path*.$ext")) {
      $files = array_merge($files, $new_files);
    }
  }

  // If its a recursive scan, enter in all directories
  // and scan for more image files
  if ($dirs = glob("$path*", GLOB_ONLYDIR) and !empty($recursive)) {
    foreach ($dirs as $dir) {
      $files = array_merge($files, _slideshow_creator_explore_dir("$dir/"));
    }
  }

  return $files;
}

/**
 * Look for and process a control file for an image in a directory to modify default values.
 *
 * @param $file
 *  String. The path to the image file from the site base.
 * @param $f_link
 *  String. The url for the link.
 * @param $f_title
 *  String. The image title
 * @param $f_caption
 *  String. The image caption.
 * @param $f_target
 *  String. The image target.
 */
function _slideshow_creator_get_file_ctlfile(
  $file, &$f_link, &$f_title, &$f_caption, &$f_target
) {
  $ctlfile = substr($file, 0, strrpos($file, '.')) . '.sscctl';
  if (is_readable($ctlfile)) {
    $ctf = fopen($ctlfile, 'r');
    if ($ctf) {
      $line = '';
      while (($line = fgets($ctf)) !== FALSE) {
	if (strncasecmp('Title:', $line, 6) == 0) {
	  $f_title = trim(substr($line, 6));
	} elseif (strncasecmp('Caption:', $line, 8) == 0) {
	  $f_caption = trim(substr($line, 8));
	} elseif (strncasecmp('Link:', $line, 5) == 0) {
	  $f_link = trim(substr($line, 5));
	  if (strcasecmp($f_link, 'yes') == 0) {
	    $f_link = $file;
	  }
	} elseif (strncasecmp('Target:', $line, 7) == 0) {
	  $f_target = trim(substr($line, 7));
	}
      }
      fclose($ctf);
    }
  }
}

/*
 $ssid_unique_id holds the unique id for the slideshow.

 The id for each slideshow ends with a number that starts with 1
 for the first slideshow on the page. Under normal circumstances
 this guarantees a unique number per slideshow on each page.
 This is augmented by a hash because slideshows might be displayed
 in different configurations from the one in which they were built
 and then cached (e.g. built as a page load then displayed as a
 teaser). This strategy is not absolutely foolproof but doing more
 would be far more expensive than the infinitesimal risk justifies.
*/
$ssid_unique_id = 0;

/**
 * The main user hook to display a slideshow.
 *
 * @param $output
 *   String. All the page text with the slideshow code
 * @return
 *   String. All the page text with the slideshow HTML/Javascript
 */
function _slideshow_creator_process_text($output) {
  // Search for Slideshow Creator tags on the text.
  global $ssid_unique_id;
  static $ssid_local = 0;

  // Slideshows end on a non-backslashed "]"
  if (preg_match_all(
	'/\[slideshow:(([\\\\][]]|[^]])+)\]/i', $output, $matches, PREG_SET_ORDER
  )) {
    $max_ss = count($matches);

    // For each of the instances, replace the tag for
    // the real HTML code
    foreach ($matches as $match) {
      $match[1] = str_replace('\]', ']', $match[1]); // Get rid of any \es before ]s
      $attributes = preg_split('|(?<!\\\),|', $match[1]);
      if (!empty($attributes)) {
	++$ssid_local;
	$ssid_unique_id = hash('crc32b', $match[1]) . $ssid_local;
        $output = preg_replace('/\[slideshow:(([\\\\][]]|[^]])+)\]/i',
          _slideshow_creator_tagconversion($max_ss, $attributes), $output, 1);
      }
    }
  }

  // Return the original text with all slideshows correctly
  // inserted
  return $output;
}

/**
 * Convert the slideshow tag into the data array that will
 * describe the Slideshow.
 *
 * @param &$max_ss
 *   Number. How many slideshows there are on the page
 * @param &$attributes
 *   Array. Each slideshow attributes, like 'img', 'dir', 'rotate'
 * @return
 *   String, The HTML code for the slideshow
 */
function _slideshow_creator_tagconversion(&$max_ss, &$attributes) {
  $output = '';

  // Using version system, its possible to expand the slideshow functionality
  // and support old versions
  switch ((int) $attributes[0]) {
    case 1:
    case 2:
    default:
      $slides = array(
        '#total'   => 0,
      );
      $slides['js'] = array();

      // Expand default tags and set as $slides['js'] values
      static $settings;
      if (empty($settings)) {
	$settings = variable_get('slideshow_creator_settings', array());
      }
      if (isset($settings['#default_tags'])) {
	$defaults = preg_split('|(?<!\\\),|', $settings['#default_tags']);
	foreach ($defaults as $default) {
	  $tagval = explode('=', $default, 2);
	  if (isset($tagval[1]) && strlen($tagval[1]) > 0) {
            if ( ($tag = trim($tagval[0])) == 'name' or $tag == 'class' ) {
              // not a javascript tag
              $slides['#class'] = trim($tagval[1]);
            } else {
              $slides['js'][$tag] = trim($tagval[1]);
            }
	  }
	}
      }

      // For the 'set for all' link, title, caption, and target attributes:
      $default_link    = '';
      $default_title   = '';
      $default_caption = '';
      $default_target  = '';

      // Now do the specified attributes:
      $attributes_count = count($attributes);
      foreach ($attributes as $attribute) {

	// First look for escaped commas (used earlier to allow commas
	// in arguments)
	$attribute = strtr($attribute, array('\\,' => ','));

	// Now divide key from value
        $attribute = explode('=', $attribute, 2);

        // If the attribute has no value, skip the validation
        if (($attribute[0] = trim($attribute[0])) == '' or
	    !isset($attribute[1])
	) {
          continue;
        }

	// We allow "attr=" syntax to set attribute to empty, but not "attr" with no "="
	$attribute[1] = trim($attribute[1]);

        if (drupal_strtolower($attribute[0]) == 'img') {
	  // img tag: URL|link|title|caption|target format
          $img_attributes = explode('|', $attribute[1]);
	  foreach ($img_attributes as &$attr) {
	    $attr = trim($attr);
	  }
	  unset($attr);  // remove the last reference to an array element.

          if (isset($img_attributes[1]) and $img_attributes[1] != '') {
            $slides['#total']++;
            if (!isset($img_attributes[2])) {
	      $img_attributes[2] = '';
	    }
            $slides[$slides['#total']] = array(
              '#src'         => _slideshow_creator_url(check_url(trim(
				  $img_attributes[1]
				))),
              '#link'        => $img_attributes[2] == '' ?
	                          $default_link :
	                          _slideshow_creator_url(url(
				    $img_attributes[2]
				  )),
              '#title'       => (!isset($img_attributes[3]) or
				 $img_attributes[3] == '') ?
                                   $default_title :
                                   preg_replace(
				     '|\\\,|',
				     ',',
				     $img_attributes[3]
				   ),
              '#caption'     => (!isset($img_attributes[4]) or
				 $img_attributes[4] == '') ?
                                   $default_caption :
                                   preg_replace(
				     '|\\\,|',
				     ',',
				     $img_attributes[4]
				   ),
              '#target'      => (!isset($img_attributes[5]) or
				 $img_attributes[5] == '')?
                                  $default_target :
	                          $img_attributes[5],
            );
          }
        }
        elseif (drupal_strtolower($attribute[0]) == 'dir') {
	  // dir tag: scan all images from a given directory
          $dir_attributes = explode('|', $attribute[1]);
	  foreach ($dir_attributes as &$attr) {
	    $attr = trim($attr);
	  }
	  unset($attr);  // remove the last reference to an array element.

	  // Get to a path with predictable '/' on both ends
	  $dir_attributes[1] = isset($dir_attributes[1]) ?
	                         trim($dir_attributes[1], ' /') : '';
	  // Add a separator '/' if not already present
	  if ($dir_attributes[1] != '') {
	    $dir_attributes[1] = '/' . $dir_attributes[1] . '/';
	  }
	  else {
	    $dir_attributes[1] = '/';
	  }

	  $recursive = isset($dir_attributes[2]) ? $dir_attributes[2] : '';
          if (
	    !empty($dir_attributes[1]) and
	    $files = _slideshow_creator_explore_dir(
	      $dir_attributes[1], $recursive
	    )
	  ) {
	    $given_link    = empty($dir_attributes[3]) ? '' : $dir_attributes[3];
	    $given_title   = empty($dir_attributes[4]) ? '' :
		               preg_replace(
			         '|\\\,|', ',', $dir_attributes[4]
		               );
	    $given_caption = empty($dir_attributes[5]) ? '' :
		               preg_replace(
			         '|\\\,|', ',', $dir_attributes[5]
		               );
	    $given_target  = !empty($dir_attributes[6]) ? $dir_attributes[6] : '';
            foreach ($files as $file) {
              $file = preg_replace('/^\.\//', '', $file);
              $slides['#total']++;
	      $f_link    = (strcasecmp($given_link, 'yes') == 0) ?
		             $file : $given_link;
	      $f_title   = $given_title;
	      $f_caption = $given_caption;
	      $f_target =  $given_target;
	      _slideshow_creator_get_file_ctlfile(
		$file,
		$f_link,
		$f_title,
		$f_caption,
		$f_target
	      );

              $slides[$slides['#total']] = array(
                '#src'         => $file,
                '#link'        => !empty($f_link) ? $f_link : $default_link,
                '#title'       => !empty($f_title) ? $f_title : $default_title,
                '#caption'     => !empty($f_caption) ? $f_caption : $default_caption,
                '#target'      => !empty($f_target) ? $f_target : $default_target,
              );
            }
          }
        }
        else { // all other tags
          $list = array(
            'layout'    => TRUE,
            'order'     => TRUE,
            'width'     => TRUE,
	    'height'    => TRUE,  // grab later
            'name'      => TRUE,
            'class'      => TRUE, // class and name are synonyms - name is deprecated
          );
          // For retro compatibility, we must filter some results
          // to simulate the previous behaviour
          if (array_key_exists($attribute[0], $list)) {
            if ($attribute[0] == 'name') {
              $attribute[0] = 'class';
            }
            if (($attribute[0] == 'class') and isset($slides['#class'])) {
              // This one is a list of css classes, not a single value, so we have to
              // combine with previously-specified names.
              if ($attribute[1] != '') {
                // Only keep prior class names if we are given a nonempty class list here
                $slides['#class'] .= ' ' . $attribute[1];
              } else {
                // Special empty value means clear existing class list
                unset($slides['#class']);
              }
            } else {
              $slides['#' . $attribute[0]] = $attribute[1];
            }
          }
          else {
            $value = $attribute[1];
            if (is_numeric($value)) {
              $value = (int) $value;
            }
            if ($attribute[0] == 'blend') {
              $slides['js']['speed'] = $value * 1000;
            }
            elseif ($attribute[0] == 'rotate') {
              $slides['js']['timeout'] = $value * 1000;
            }
	    elseif ($attribute[0] == 'link') {
	      $default_link = _slideshow_creator_url(check_url($value));
	    }
	    elseif ($attribute[0] == 'title') {
	      $default_title = $value;
	    }
	    elseif ($attribute[0] == 'caption') {
	      $default_caption = $value;
	    }
	    elseif ($attribute[0] == 'target') {
	      $default_target = $value;
	    }
	    else {
	      $slides['js'][$attribute[0]] = $value;
	    }
	  }
        }
      }

      // If at least one slide is valid, build the Slideshow
      if ($slides['#total'] > 0) {

        // The slideshow should be displayed using a theme function
	// to allow users to change its general look
        $output = theme('slideshow_creator', array('ss' => $slides));
      }
      break;
  }
  return $output;
}

/**
 * Repair internal urls using the pathfilter module, if available.
 *
 * @param $url
 *   String, The url to complete (if internal)
 * @return
 *   String, The full url, filtered from internal
 */
function _slideshow_creator_url($url) {
  if (module_exists('pathfilter')) {
    // Wrap url in double quotes, filter and then remove double quotes
    return preg_replace("(^\"|\"$)", '', pathfilter_filter('process', 0, -1, '"' . $url . '"'));
  }
  else {
    // Return URL as-is
    return $url;
  }
}

function _slideshow_creator_send_default_to_js(&$ss, $tagname, $hashtagname) {
  if (array_key_exists($hashtagname, $ss)) {
    if (is_numeric($ss[$hashtagname])) {
      $ss[$hashtagname] .= 'px';
    }
    $ss[$hashtagname] = '\'' . $ss[$hashtagname] . '\'';
    $ss['js'][$tagname] = $ss[$hashtagname];
  }
}

/**
 * Format the slideshow
 * @ingroup themeable
 */
function theme_slideshow_creator($variables) {
  $ss = $variables['ss'];

  // To ensure that all slideshows have a unique ID, the
  // ssid variable will hold a automatic increasing number
  global $ssid_unique_id;
  $ssid = $ssid_unique_id;

  static $settings;
  if (empty($settings)) {
    $settings = variable_get('slideshow_creator_settings', array());
  }

  // Merge the current slideshow data with previously saved default settings
  $ss = array_merge($settings, $ss);

  // Default settings for jQuery Cycle plugin
  // The proper JavaScript commands of next and previous
  if (!array_key_exists('pause', $ss['js'])) {
    $ss['js']['pause'] = TRUE;
  }
  $ss['js']['next']  = '#ssc-next-' . $ssid;
  $ss['js']['prev']  = '#ssc-previous-' . $ssid;

  // This will put the pieces into a given order,
  // base on which "layout" the user chose.

  // The 'order' is invented as a tag to overcome a long-standing bug
  // by which the 'layout' didn't function properly for many years.
  // Rather than fix it and thereby break everyone's workarounds, we
  // introduce a new tag, 'order'.
  // Order specifies the order of the main pieces of a slideshow:
  //   Title, Image, and Caption.
  // The old layout option specifies whether the image is above (top),
  //   or below (bottom) the controls, or if the controls are absent (none).
  // If 'order' is absent, 'layout' alone gives the old behaviour.

  $showcontrols = array('#previous', '#status', '#next', );

  if ( !empty($ss['#order'])) {
    switch ($ss['#order']) {
      case 'middle':
	$showorder = array(array('#title', '#main', '#caption', ));
	break;
      case 'top':
	$showorder = array(array('#main', '#title', '#caption', ));
	break;
      case 'reversebottom':
	$showorder = array(array('#caption', '#title', '#main', ));
	break;
      case 'reversemiddle':
	$showorder = array(array('#caption', '#main', '#title', ));
	break;
      case  'reversetop':
	$showorder = array(array('#main', '#caption', '#title', ));
	break;
      case 'default':
      case 'bottom':
      default:
	$showorder = array(array('#title', '#caption', '#main', ));
	break;
    }
  }
  else {
    $showorder = array(array('#title', '#caption', '#main', ));
  }

  if (
    empty($ss['#layout']) or
    $ss['#layout'] == 'default' or $ss['#layout'] == 'bottom'
  ) {
    $layout = array_merge($showcontrols, $showorder);
  }
  elseif ($ss['#layout'] == 'top' or $ss['#layout'] == 'reverse') {
    $layout = array_merge($showorder, $showcontrols);
  }
  elseif ($ss['#layout'] == 'none') {
    $layout = $showorder;
  }
  else {
    $layout = array_merge($showcontrols, $showorder);
  }

  _theme_slideshow_creator_nextprevious($ssid, $ssc, $ss);

  // Building the slideshow parts: title, name, slideshow weight and height
  $css_class = !empty($ss['#class']) ? $ss['#class'] : '';

  // Setting the image width
  // Height, but not width, is passed to jQuery to handle for best
  // results if javascript is turned off in the browser. It allows
  // the specified height to be used by jQuery, or else if no js,
  // the height, being omitted from the static page, expands and
  // shows all the images statically.
  $css_width  = $ss['#width'] . 'px';

  _slideshow_creator_send_default_to_js($ss, 'height', '#height');

  // Create each image. The order should be:
  // 1* the current picture to the end
  // 2* the first picture to the current
  for ($i = $ss['#current']; $i <= $ss['#total']; ++$i) {
    _theme_slideshow_creator_content($ssid, $ssc, $ss[$i - 1]);
  }
  for ($i = 1; $i < $ss['#current']; ++$i) {
    _theme_slideshow_creator_content($ssid, $ssc, $ss[$i - 1]);
  }

  // Put all pieces together, according to layout
  $output = '';
  foreach ($layout as $piece) {
    if (is_array($piece)) {
      $output .= "<span id='ssc-content-$ssid' class='ssc-content' style=\"width:$css_width;\">";
      foreach (array_keys($ssc['#content']) as $slide) {
        $output .= "<span id='ssc-slide-$ssid-$slide' class='ssc-slide' style=\"width:$css_width;\">";
	foreach ($piece as $item) {
	  if (!empty($ssc['#content'][$slide][$item])) {
	    $output .= $ssc['#content'][$slide][$item];
	  }
        }
        $output .= "</span>";
      }
      $output .= "</span>";
    }
    else {
      if (isset($ssc[$piece])) {
	$output .= $ssc[$piece];
      }
    }
  }

  // Filter the JS object to avoid JS injection
  // NOTE: this list should be periodically updated with cycle jQuery plugin
  // NB: ssc-specific js attributes are omitted.
  $cycle_attributes = array_flip(array(
    'activePagerClass',
    'after',
    'allowPagerClickBubble',
    'animIn',
    'animOut',
    'autostop',
    'autostopCount',
    'backwards',
    'before',
    'cleartype',
    'cleartypeNoBg',
    'containerResize',
    'continuous',
    'cssBefore',
    'cssAfter',
    'delay',
    'easeIn',
    'easeOut',
    'easing',
    'end',
    'fastOnEvent',
    'fit',
    'fx',
    'fxFn',
    'height',
    'manualTrump',
    'metaAttr',
    'next',
    'nowrap',
    'onPagerEvent',
    'onPrevNextEvent',
    'pager',
    'pagerAnchorBuilder',
    'pagerEvent',
    'pause',
    'pauseOnPagerHover',
    'prev',
    'prevNextEvent',
    'random',
    'randomizeEffects',
    'requeueOnImageNotLoaded',
    'requeueTimeout',
    'rev',
    'shuffle',
    'slideExpr',
    'slideResize',
    'speed',
    'speedIn',
    'speedOut',
    'startingSlide',
    'sync',
    'timeout',
    'timeoutFn',
    'updateActivePagerLink',
    'width',
  ));
  $have_ssc_keys = false;
  foreach ($ss['js'] as $key => $attribute) {
    $is_ssc_key = (strncmp($key, 'ssc_', 4) == 0 && strlen($key) > 4);
    if ( (!isset($ss['js']['ssc_pager']) || ! $is_ssc_key ) &&
	 ! isset($cycle_attributes[$key])
    ) {
      unset($ss['js'][$key]);
    } elseif ($is_ssc_key) {
      $have_ssc_keys = true;
    }
  }

  $cycle_fx_valid_args = array_flip(array(
    'blindX',
    'blindY',
    'blindZ',
    'cover',
    'curtainX',
    'curtainY',
    'fade',
    'fadeZoom',
    'growX',
    'growY',
    'none',
    'scrollUp',
    'scrollDown',
    'scrollLeft',
    'scrollRight',
    'scrollHorz',
    'scrollVert',
    'shuffle',
    'slideX',
    'slideY',
    'toss',
    'turnUp',
    'turnDown',
    'turnLeft',
    'turnRight',
    'uncover',
    'wipe',
    'zoom',
  ));

  // Compose slideshow settings as a script tag in the html. NB: We cannot
  // use the following 'setting' line because Drupal.settings has proven
  // unreliable when pages are cached:
  //   drupal_add_js(array('ssc' => array($ssid => $ss['js'])), array('type' => 'setting', 'scope' => JS_DEFAULT));
  $sscvars = "<script type=\"text/javascript\">\n";
  $sscvars .= "if(typeof(window['ssc_settings'])=='undefined') { ssc_settings = new Object(); }\n";
  $sscvars .= "ssc_settings['$ssid'] = new Object();\n";
  if ($have_ssc_keys) {
    $sscvars .= "ssc_settings['$ssid'].ssc = new Object();\n";
  }
  foreach ($ss['js'] as $key => $attribute) {
    // Make a field for each attribute in the object for this slideshow
    if (strncmp($key, 'ssc_', 4) == 0) {
      $sscvars .= "ssc_settings['$ssid'].ssc." . substr($key, 4) . " = ";
    } else {
      $sscvars .= "ssc_settings['$ssid'].$key = ";
    }
    if (is_bool($attribute)) {
      $sscvars .= ($attribute ? 'true' : 'false');
    } elseif ($attribute[0] == '#') {
      $sscvars .= "'$attribute'";
    } elseif ($key == 'fx' && isset($cycle_fx_valid_args[$attribute])) {
      $sscvars .= "'$attribute'";
    } else {
      // NB: Don't try to quote all non-numerics because too many cycle
      // attributes are actually javascript (functions, etc.).
      $sscvars .= "$attribute";
    }
    $sscvars .= ";\n";
  }
  $sscvars .= "</script>";

  // Put everything inside a SPAN tag
  return "<span class='ssc $css_class' id='ssc-$ssid' style='width:$css_width;'>$output$sscvars</span>";
}

/**
 * Build the main content of the slideshow. It needs a different
 * process because the content is the union of separated parts
 * that are build in a loop process.
 *
 * @param $ssid
 *   Number. The slideshow unique ID
 * @param &$ssc
 *   Array. The slideshow final data array
 * @param &$ss
 *   Array. The partial data array, extracted from the ssc tag
 */
function _theme_slideshow_creator_content($ssid, &$ssc, &$ss) {
  $title = isset($ss['#title']) ? $ss['#title'] : '';
  $caption = isset($ss['#caption']) ? $ss['#caption'] : '';
  $alt_attr = $caption != '' ? $caption : $title;

  if (isset($ss['#src']) and $ss['#src'] != '') {
    // Build the image
    $image = theme(
               'image',
	       array('path' => $ss['#src'], 'alt' => $alt_attr, 'title' => $alt_attr)
    );

    $image = preg_replace("/\"\/\//", '"/', $image);

    // Add a new image link
    if ($ss['#link'] == '') {
      $main = $image;
    }
    else {
      $ss['#link'] = preg_replace("/^\/\/?/", '', $ss['#link']);
      if ($ss['#target'] == '') {
	$main = l($image, $ss['#link'], array('html' => TRUE));
      }
      else {
	$main = l(
	  $image,
	  $ss['#link'],
	  array(
	    'html' => TRUE,
	    'attributes' => array('target' => $ss['#target'])
	  )
	);
      }
    }
  }
  else {
    $main = $ss['#main'];
  }

  $ssc_item = array();
  if ($title != '') {
    $ssc_item['#title'] = "<span class='ssc-title'>$title</span>";
  }
  if ($caption != '') {
    $ssc_item['#caption'] = "<span class='ssc-caption ssc-description'>$caption</span>";
  }
  $ssc_item['#main'] = "<span class='ssc-main'>$main</span>";
  $ssc['#content'][] = $ssc_item;
}

/**
 * Create Next and Previous links.
 *
 * Next and Previous links work with jQuery no matter what they say, so
 * we make them point back to this page. We no longer try to build
 * special links for non-javascript browsers, as with D7's very aggressive
 * caching we cannot be sure any given code will be executed when needed.
 * Also such links would require page rendering on the server every time
 * a non-js browser user clicked next/previous. Instead, the code earlier
 * renders all images sequentially for non-js browsers.
 *
 * @param $ssid
 *   Number. The slideshow unique ID
 * @param &$ssc
 *   Array. The slideshow final data array
 * @param &$ss
 *   Array. The partial data array, extracted from the ssc tag
 */
function _theme_slideshow_creator_nextprevious($ssid, &$ssc, &$ss) {

  $ss['#current'] = 1;

  // Put a PREVIOUS and NEXT buttons if there are more
  // then one image (generally TRUE for a slideshow)
  if ($ss['#total'] > 1) {
    $ssc['#previous'] = l(
      t('Previous'),
      $_GET['q'],
      array(
	'attributes' => array('id' => "ssc-previous-$ssid",
			      'class' =>  array('ssc-previous'),
	                     ),
      )
    );
    $ssc['#next']     = l(
      t('Next'),
      $_GET['q'],
      array(
	'attributes' => array('id' => "ssc-next-$ssid",
			      'class' => array('ssc-next'),
	                     ),
      )
    );
    $ssc['#status']   =
	" <span class='ssc-index' id='ssc-index-$ssid'>" .
	t($ss['#current_slide_string']) .
	" <span class='ssc-current' id='ssc-current-$ssid'>" .
	$ss['#current'] . '</span>/' . $ss['#total'] . ' </span>';
  }
}
