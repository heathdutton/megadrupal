<?php
// $Id: nitobe.utils.inc,v 1.3 2010/11/27 01:36:29 shannonlucas Exp $
/**
 * @file
 * Various utility functions used by Nitobe.
 */

/** The cache variable to store the header list in. */
define("NITOBE_CACHE_HDR", "nitobe.headers.list");

/** The path to the headers in the current theme, falling back to Nitobe if
 * the headers directory is not present in the sub-theme. */
define("NITOBE_HEADER_PATH",
       (is_dir(path_to_theme() . "/headers") ?
       path_to_theme() : drupal_get_path("theme", "nitobe")) . "/headers");

/** The mask to use when listing the headers directory. */
define("NITOBE_HEADER_IMG_MASK",
       "/\\.jpg$|\\.JPG$|\\.jpeg*|\\.JPEG*|\\.gif$|\\.GIF$|\\.png$|\\.PNG$/");


/**
 * Contextually adds 960 Grid System classes.
 *
 * The first parameter passed is the *default class*. All other parameters must
 * be set in pairs like so: "$variable, 3". The variable can be anything
 * available within a template file and the integer is the width set for the
 * adjacent box containing that variable.
 *
 * @code
 * class="<?php print ns('grid-16', $var_a, 6); ?>"
 * @endcode
 *
 * If $var_a contains data, the next parameter (integer) will be subtracted from
 * the default class. See the README.txt file.
 */
function nitobe_ns() {
  $args    = func_get_args();
  $default = array_shift($args);

  // -- Get the type of class, i.e., 'grid', 'pull', 'push', etc.
  // -- Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // -- Process the conditions.
  $flip_states = array('var' => 'int', 'int' => 'var');
  $state       = 'var';
  $var_state   = FALSE;

  foreach ($args as $arg) {
    if ($state == 'var') {
      $var_state = !empty($arg);
    }
    elseif ($var_state) {
      $return_unit = $return_unit - $arg;
    }
    $state = $flip_states[$state];
  }

  $output = '';
  // -- Anything below a value of 1 is not needed.
  if ($return_unit > 0) {
    $output = $type . '-' . $return_unit;
  }
  return $output;
}


/**
 * Gets the push/pull classes for the various layout options.
 *
 * @return array
 *   A nested array of layout and class information. The array's values are
 *   accesed with $push_pull[$placement][$layout][$region] such that:
 *   - $placement is one of left or center and indicates where the
 *     sidebars are placed relative to the content. A 'right' placement is
 *     not provided since this is the natural layout.
 *   - $layout is one of left, right, or both and indicates which sidebars
 *     are present.
 *   - $region is one of content, sidebar_first, sidebar_second and indicates
 *     which region push/pull classes are wanted for.
 */
function _nitobe_get_push_pull() {
  $push_pull = array(
    "center" => array(
      "left" => array(
        "content"        => "push-4",
        "sidebar_first"  => "pull-12",
      ),
      "both" => array(
        "content"        => "push-4",
        "sidebar_first"  => "pull-8",
      ),
    ),
    "left" => array(
      "left" => array(
        "content"        => "push-4",
        "sidebar_first"  => "pull-12",
      ),
      "right" => array(
        "content"        => "push-4",
        "sidebar_second" => "pull-12",
      ),
      "both" => array(
        "content"        => "push-8",
        "sidebar_first"  => "pull-8",
        "sidebar_second" => "pull-8",
      ),
    ),
  );

  return $push_pull;
}


/**
 * Copies selected items from one array to another array by key.
 *
 * For each item in $keys, if that key exists in $source, copy the key-value
 * pair from $source to &$destination.  In the event of duplicate keys, the
 * value of a key in $source will replace the value in &$destination.
 *
 * @param array $source
 *   A keyed array of values to copy from.
 * @param array &$destination
 *   The array to copy to.
 * @param array $keys
 *   An array of keys to copy from $source to &$destination.
 */
function _nitobe_copy_if_exists($source, &$destination, $keys) {
  foreach ($keys as $key) {
    if (array_key_exists($key, $source)) {
      $destination[$key] = $source[$key];
    }
  }
}


/**
 * Retrieves a list of header images.
 *
 * Scans the headers directory and generate a "pretty" name for each. Pretty
 * names are generated from the image's path within the headers directory using
 * these rules:
 * -# '/' is replaced with ' / '
 * -# '_' is replaced with ' '.
 * -# '.***' extension is removed.
 *
 * @param boolean $refresh
 *   If TRUE, reload the image list and flush the cached version.
 *
 * @return array
 *   A mapping of the headers' pretty names to their actual names.
 */
function _nitobe_get_header_list($refresh = FALSE) {
  // -- If caching is disabled, force a refresh.
  if (!$refresh && (variable_get('cache', 0) == 0)) {
    $refresh = TRUE;
  }

  $cached = cache_get(NITOBE_CACHE_HDR);
  $files  = (!empty($cached)) ? $cached->data : NULL;

  if (($files == NULL) OR ($refresh == TRUE)) {
    $files = file_scan_directory(NITOBE_HEADER_PATH,
                                 NITOBE_HEADER_IMG_MASK,
                                 array('.', '..', 'CVS', '.svn'));
    foreach ($files as $filename => $data) {
      $name = substr($filename, strlen(NITOBE_HEADER_PATH) + 1);
      $name = preg_replace('/\//', ' / ', $name);
      $name = preg_replace('/_/', ' ', $name);
      $name = preg_replace('/\.(\w{3,4}$)/', '', $name);

      $data->pretty_name = $name;
    }

    // -- Cache the list for a week.
    cache_set(NITOBE_CACHE_HDR, $files, 'cache', time() + 604800);
  }

  return $files;
}
