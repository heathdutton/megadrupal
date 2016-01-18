<?php

/**
 * @file
 * Generates breadcrumb by walking backwards on the current path.
 */

class Smart_breadcrumb {
  var $lang, $path, $cid, $cached = FALSE, $settings = array(), $crumbs = array(), $breadcrumb = array();

  function __construct() {
    global $language;

    // set current language prefix
    $this->lang = $language->language;

    // get the path from the requested URL path
    $this->path = request_path();

    // TODO: Update for D7
    // remove language prefix from path if language negotioation is set to use path prefix
    // if (LANGUAGE_NEGOTIATION_PATH_DEFAULT) {
    //   $this->path = preg_replace("/^$this->lang(\/?)/", '', $this->path);
    // }

    // get breadcrumb cache id by calculating md5 of path with prepended language
    $this->cid = $this->lang . '/' . $this->path;

    // get the cached data
    $cache = cache_get($this->cid, 'cache_smart_breadcrumb');

    // set the breadcrumb using cached data
    if (is_object($cache)) {
      $this->breadcrumb = (array)$cache->data['breadcrumb'];
    }
    else {
      $this->breadcrumb = array();
    }
  }

  function title_raw($path) {
    // get arguments from path
    $args = explode('/', $path);

    // invoke implementations of hook_crumb_title_raw()
    $titles = module_invoke_all('crumb_title_raw', $path, $this->lang);

    // filter empty titles
    $titles = array_filter($titles);

    // set title as last title returned
    $title = end($titles);

    // format raw title if none returned by hook
    if (empty($title)) {
      // get current page title if path is current path
      if ($path == $this->path) {
        $title = drupal_get_title();
      }

      if (empty($title)) {
        $title = ucwords(htmlspecialchars(str_replace(array('_', '-', '+', '%20'), ' ', end($args))));
      }
    }

    return $title;
  }

  function filter_titles($titles, $path) {
    // filter empty titles
    $titles = array_filter($titles);

    // iterate through each item
    foreach ($titles as $module => $val) {
      // filter array if is already array
      if (is_array($val)) {
        // remove item if title is empty or not set
        if (!isset($val['title']) || empty($val['title'])) {
          unset($titles[$module]);
        }
        else {
          // set path if not provided in array
          if (!isset($val['path'])) {
            $titles[$module]['path'] = $path;
          }
        }
      }
      else {
        // create array from plain text title
        $titles[$module] = array(
          'title' => t($val),
          'path' => $path
        );
      }

      // check that the title for the given module has been set
      if (isset($titles[$module]) && isset($titles[$module]['title'])) {
        // strip html tags
        $titles[$module]['title'] = strip_tags($titles[$module]['title']);
      }
    }

    // reset modified array
    reset($titles);

    return $titles;
  }

  // hook_smart_breadcrumb_settings
  function custom_settings() {
    // set hook
    $hook = 'smart_breadcrumb_settings';

    // duplicate module settings
    $custom_settings = $this->settings;

    // iterate through each hook implementation
    foreach (module_implements($hook) as $module) {
      // define function name
      $function = $module . '_' . $hook;

      // get settings from hook
      $settings = $function($custom_settings);

      // mere settings from hook with module settings
      if (isset($settings) && is_array($settings)) {
        $custom_settings = array_merge($custom_settings, $settings);
      }
    }

    return $custom_settings;
  }

  // hook_crumb_source_alter
  function crumb_source_alter($source) {
    // set hook
    $hook = 'crumb_source_alter';

    // iterate through each hook implementation
    foreach (module_implements($hook) as $module) {
      // define function name
      $function = $module . '_' . $hook;

      // get settings from hook
      $function($source);
    }

    return $source;
  }

  // hook_crumb_alter
  function crumb_alter($crumb, $title, $count) {
    // set hook
    $hook = 'crumb_alter';

    // iterate through each hook implementation
    foreach (module_implements($hook) as $module) {
      // define function name
      $function = $module . '_' . $hook;

      // get settings from hook
      $function($crumb, $title, $count);
    }

    return $crumb;
  }

  // hook_breadcrumb_alter
  function breadcrumb_alter() {
    // set hook
    $hook = 'breadcrumb_alter';

    // iterate through each hook implementation
    foreach (module_implements($hook) as $module) {
      // define function name
      $function = $module . '_' . $hook;

      // get settings from hook
      $function($this->breadcrumb, $this->path, $this->settings);
    }
  }

  function generate_breadcrumbs() {
    // force to developer mode if page cache is enabled, otherwise use Smart Breadcrumb setting
    $devel = variable_get('cache', 0) ? 0 : variable_get('smart_breadcrumb_devel', 0);

    // is breadcrumb as cached or devel mode enabled
    if (empty($this->breadcrumb) || $devel) {
      // get additional variables
      $this->settings['home'] = variable_get('smart_breadcrumb_home', NULL);
      $this->settings['home_class'] = variable_get('smart_breadcrumb_home_class', NULL);
      $this->settings['tag'] = variable_get('smart_breadcrumb_tag', 'span');
      $this->settings['link_wrap'] = variable_get('smart_breadcrumb_link_wrap', FALSE);
      $this->settings['current'] = variable_get('smart_breadcrumb_current', NULL);
      $this->settings['exclude'] = variable_get('smart_breadcrumb_exclude', NULL);

      // merge custom settings with default
      $this->settings = array_merge($this->settings, $this->custom_settings());

      // if path is in exclude list, use existing breadcrumb
      if (drupal_match_path($this->path, $this->settings['exclude'])) {
        // set breadcrumb to default
        $this->breadcrumb = drupal_get_breadcrumb();
      }
      else if(!empty($this->path)) {
        // explode the path into arguments
        $args = explode('/', $this->path);

        // count number of arguments
        $this->args = count($args);

        // cycle through arguments, in reverse order
        for ($key = ($this->args - 1); $key >= 0; $key--) {
          // create a path to check based on the current items in the argument's array
          $crumb_path = implode('/', $args);

          // get source of crumb path
          // invoke implementations of hook_crumb_source_alter()
          $crumb_source = $this->crumb_source_alter(drupal_lookup_path('source', $crumb_path, $this->lang));

          // set hook name
          $hook = 'crumb_title';

          // create array to hold titles
          $titles = array();

          // execute each instance of hook_crumb_title
          foreach (module_implements($hook) as $module) {
            $titles[$module] = module_invoke($module, $hook, $crumb_path, $crumb_source, $this->lang);
          }

          // add raw title
          $titles['raw'] = $this->title_raw($crumb_path);

          // filter empty titles
          $titles = $this->filter_titles($titles, $crumb_path);

          // set crumb with first generated title
          $this->set_crumb($titles, $key + 1);

          // unset the current array key, reducing the number of arguments for next iteration
          unset($args[$key]);
        }

        // if breadcrumb text for home page is not empty, create first crumb
        if (!empty($this->settings['home'])) {
          $titles = array(
            'home' => array(
              'title' => $this->settings['home'],
              'path' => NULL,
              'attributes' => array(
                'class' => $this->settings['home_class']
              )
            )
          );

          $this->set_crumb($titles, 0);
        }

        // reverse crumbs array and merge with breadcrumb array
        $this->breadcrumb = array_reverse($this->crumbs);
      }

      $this->breadcrumb_alter();

      // breadcrumb is not cached
      $this->cached = FALSE;
    }
    else {
      // breadcrumb is cached
      $this->cached = TRUE;
    }
  }

  // set crumb
  function set_crumb($titles = array(), $count) {
    // get first item in titles array
    $title = current($titles);

    // continue only if title is array
    if (is_array($title)) {
      // set reference to module
      $title = array_merge(array('module' => key($titles)), $title);

      // deterimine if crumb is last
      $title['last'] = $this->args == $count;

      // increase the count of the crumb if a home crumb will be created
      if (!empty($this->settings['home'])) {
        $count++;
      }

      // set default values
      $link = TRUE;

      // add class array to attributes
      if (!isset($title['attributes'])) {
        $title['attributes'] = array('class' => '');
      }

      // add classes
      $title['attributes']['class'] = trim('crumb crumb-' . $count . ' ' . $title['attributes']['class']);

      // check if last crumb
      if ($title['last']) {
        // do not link the crumb
        $link = FALSE;

        // get current class
        $smart_breadcrumb_current = $this->settings['current'];

        // set class if current class exists
        if (!empty($smart_breadcrumb_current)) {
          $title['attributes']['class'] .= ' ' . $smart_breadcrumb_current;
        }
      }
      elseif (empty($title['path']) || $title['module'] === 'raw') {
        // do not link the crumb
        $link = FALSE;
      }

      $tag_open = '<' . $this->settings['tag'] . ' class="' . $title['attributes']['class'] . '">';
      $tag_close = '</' . $this->settings['tag'] . '>';

      // create crumb
      if ($link || $title['module'] == 'home') {
        // define default link options
        $link_options = array();

        // add class attributes for link
        if (!$this->settings['link_wrap']) {
          $link_options['attributes'] = $title['attributes'];

          $tag_open = NULL;
          $tag_close = NULL;
        }

        // linked crumb
        $crumb = l($title['title'], $title['path'], $link_options);
      }
      else {
        // add crumb
        $crumb = $title['title'];
      }

      // wrap plain-text crumb in span
      if (strlen($crumb) == strlen(strip_tags($crumb))) {
        $crumb = '<span>' . $crumb . '</span>';
      }

      // invoke implementations of hook_crumb_alter()
      $crumb = $this->crumb_alter($crumb, $title, $count);

      // add crumb
      $this->crumbs[] = $tag_open . $crumb . $tag_close;
    }
  }

  // set drupal breadcrumb and update cached data
  function set_breadcrumb() {
    drupal_set_breadcrumb($this->breadcrumb);

    // update cache if not cached already
    if (!$this->cached) {
      $this->cache_update();
    }
  }

  // update cached data with new breadcrumb
  function cache_update() {
    // create data array
    $data = array(
      'path' => $this->path,
      'lang' => $this->lang,
      'breadcrumb' => $this->breadcrumb
    );

    // set data for breadcrumb
    cache_set($this->cid, $data, 'cache_smart_breadcrumb');
  }
}
