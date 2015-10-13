<?php
/**
 * @file
 * Main Prometheus Class Implementation
 *
 * All the magick happens here
 */

define( 'TEMPLATES_DIR', 'templates' );
define( 'TEMPLATES_EXT', 'tpl.php' );
define( 'DEFAULTS', 'DEFAULTS' );
define( 'PROMETHEUS_AJAX',  '_PROMETHEUS_AJAX' );
define( 'PROMETHEUS_TITLE', '_PROMETHEUS_TITLE');

class Prometheus {
  // supplement the existing filter type map, provide nicer names for filter types
  private static $_FILTER_TYPE_MAP_EXTENSIONS =
  array(
    'validate_boolean' => FILTER_VALIDATE_BOOLEAN,
    'validate_float' => FILTER_VALIDATE_FLOAT,
    'validate_int' => FILTER_VALIDATE_INT,
    'validate_required' => 'validate_required', //special, specific to Prometheus only
    'filter_email' => FILTER_SANITIZE_EMAIL,
    'filter_encoded' => FILTER_SANITIZE_ENCODED,
    'filter_magic_quotes' => FILTER_SANITIZE_MAGIC_QUOTES,
    'filter_number_float' => FILTER_SANITIZE_NUMBER_FLOAT,
    'filter_number_int' => FILTER_SANITIZE_NUMBER_INT,
    'filter_special_chars' => FILTER_SANITIZE_SPECIAL_CHARS,
    'filter_full_special_chars' => FILTER_SANITIZE_FULL_SPECIAL_CHARS,
    'filter_string' => FILTER_SANITIZE_STRING,
    'filter_stripped' => FILTER_SANITIZE_STRIPPED,
    'filter_url' => FILTER_SANITIZE_URL,
    'filter_unsafe_raw' => FILTER_UNSAFE_RAW
  );

  private static $_root_controller_index_has_arguments = FALSE;

  /** Controller Instance Cache */
  private static $_instances = array();

  /** Registered Menu Hooks */
  private static $_menu_hooks = array();

  /** Per class::path argument names */
  public static $_page_arg_names = array();

  /** Per class::path argument default values */
  private static $_page_arg_defaults = array();

  /** GET & POST parameters cache for ::get_parameters() */
  private static $_parameters = array();

  /** Config Options for every path */
  private static $_register_options = array();
  private static $_input_results = array();

  /** Theme registry options */
  private static $_theme_registry = array();

  /** Convenient registration config defaults */
  private static $_DEFAULTS = array(
    // default settings are for all output to be echoed into the content region
    'template' => array(
      'ob_enable'      => TRUE,       // enables output buffering
      'load_as_region' => 'content',
    ),
    // enable ajax fragments. note that you also need to add ajax="TRUE" to <form>
    'ajax_fragments' => TRUE,
    // make sure that if index is defined at root controller to use it as site frontpage
    'site_frontpage_index' => TRUE
  );

  /** Main register function, called from modules */
  public static function register( $register_options ) {
    // TODO: Refactor so we don't need to turn of notices
    $saved_error_level = error_reporting();
    error_reporting($saved_error_level ^ E_NOTICE);
    // user can specify defaults for every class that is being registered at ths time
    // and override prometheus defaults
    $defaults = self::merge_config(self::$_DEFAULTS, (array)$register_options[DEFAULTS]);
    unset($register_options[DEFAULTS]);

    foreach ( $register_options as $class_name => $class_options ) {
      // each class can have it's own defaults
      // these override any global defaults already defined
      // (array) typecast ensures merge_config doesn't complain if defaults are missing
      $class_defaults = self::merge_config($defaults, (array) $class_options[DEFAULTS]);
      unset($class_options[DEFAULTS]);
      self::register_class( $class_name, $class_options, $class_defaults );
    }
    error_reporting($saved_error_level);
  }

  /** Remembers the registration options for this class */
  public static function set_register_options( $class_name, $path_name, $options ) {
    self::$_register_options[$class_name][$path_name] = $options;
  }

  /** Gets registration options for this path, including per class and global defaults */
  public static function get_register_options( $class_name, $path_name ) {
    return self::$_register_options[$class_name][$path_name];
  }

  /** Remembers the validated/filtered input results */
  public static function set_input_results( $results ) {
    self::$_input_results = $results;
  }

  /** Gets the validated/filtered input results */
  public static function get_input_results() {
    return self::$_input_results;
  }

  /** Needed to override normal parameters with filtered versions */
  public static function set_parameters( $request_parameters ) {
    self::$_parameters = $request_parameters;
  }

  /** Convenient way to get to query string and posted parameters */
  public static function get_parameters() {
    if ( empty( self::$_parameters ) ) {
      $request_parameters = array_merge( $_GET, $_POST );
      self::$_parameters = $request_parameters ;
    }
    return self::$_parameters;
  }

  /** Returns a singleton istance of a class with the specified $class_name */
  public static function get_instance( $class_name ) {
    if ( empty( self::$_instances[$class_name] ) ) {
      self::$_instances[$class_name] = new $class_name;
    }
    return self::$_instances[$class_name];
  }

  /** Returns all registered menu_hooks */
  public static function get_menu_hooks() {
    return self::$_menu_hooks;
  }

  /** Helper function to load node data from a view */
  public static function load_view_node( $view_name, $display_id = NULL, $args = NULL ) {
    $result = array();
    //determine if we are passing array of arguments to the view or a single one
    if ( is_array( $args ) ) {
      array_unshift( $args, $view_name, $display_id );
      call_user_func_array( 'views_get_view_result', $args );
    }
    else {
      $view_result = views_get_view_result( $view_name, $display_id, $args );
    }
    //load node data for each view result
    foreach ( $view_result as $value ) {
      //reset static node cache on every load in order to conserve memory usage
      $node = node_load( $value->nid, NULL, TRUE );
      array_push(
        $result,
        array(
          'node' => $node,
          'view' => $view_result
        )
      );
    }
    return $result;
  }

  /** Helper function to load specified template within controller context */
  public static function load_template( $template_file, &$variables ) {
    $template_path = sprintf(
      '%s/%s',
      path_to_theme(),
      TEMPLATES_DIR
    );
    $template_file = sprintf(
      '%s/%s.%s',
      $template_path,
      $template_file,
      TEMPLATES_EXT
    );
    $content = theme_render_template( $template_file, $variables );
    return $content;
  }

  /** Helper function to load specified node template within controller context */
  public static function load_template_node( $nid, $variables ) {
    extract( $variables, EXTR_SKIP );
    $node = node_load( $nid, NULL, TRUE );
    ob_start();
    eval( '?> ' . $node->body );
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
  }

  /** Indicates wheter prometheus will forward all unknown paths to root controller's index() */
  public static function is_root_controller_catchall() {
    return self::$_root_controller_index_has_arguments;
  }

  /** Registers individual class, called by register */
  private static function register_class( $class_name, $class_options = array(), $default_class_options ) {
    // convert all Namespace Separators into forward-slashes and any underscores into dashes
    $path_prefix = str_replace( array("\\", "_"), array("/", "-"), strtolower( $class_name ) );
    $module = new ReflectionClass( $class_name );
    $paths = $module->getMethods();

    foreach ( $paths as $path ) {
      // ignore private, protected functions and constructors --
      if ( $path->isPrivate() )           continue;
      if ( $path->isProtected() )         continue;
      if ( $path->name == "__construct" ) continue;

      // asssemble and save all the options that apply to this path
      $path_options = self::merge_config($default_class_options, (array) $class_options['paths'][$path->name]);
      self::set_register_options($class_name, $path->name, $path_options);

      // check for path prefix override in the options --
      if ( isset($path_options['prefix']) ) {
        // custom prefix overides go unaltered to be appended to
        $this_path_prefix = $path_options['prefix'];
      }
      else {
        // auto-deduced prefix (from class_name) needs a trailing slash
        $this_path_prefix = $path_prefix . '/';
      }

      // index function handles root path for its class --
      if ( $path->name == "index" ) {
        // we strip any trailing slashes, and use that as the path
        $this_path_prefix = trim($this_path_prefix, '/');
        if ( !empty($this_path_prefix) ) {
          $menu_hook_path = $this_path_prefix;
        }
        else {
          // drupal doesn't support empty paths
          // in this case we just register as index
          // and the site operator can use admin settings to make
          // index the front page
          $menu_hook_path = "index";
        }
      }
      else {
        // we replace any underlines with dashes for prettier looking paths
        $menu_hook_path = $this_path_prefix . str_replace("_", "-", $path->name);
      }
      // create a proxy function that drupal can access for the menu callback --

      // name the proxy function prometheus_proxy_menu_hook_path
      $prometheus_proxy_fn_name = "prometheus_proxy_" . str_replace( array("/", "-"), array("_", "_"), $menu_hook_path );

      $prometheus_proxy_fn = "function $prometheus_proxy_fn_name() {
        // for PHP < 5.3 we need to get arguments first, then pass them as func_get_args relies on context
        \$args = func_get_args();
        Prometheus::invoke('$menu_hook_path', '$class_name', '$path->name', \$args);
      }";

      eval( $prometheus_proxy_fn );

      // prepare "page arguments" --
      // for drupal to include path components as arguments to the callback
      // we need to supply and array of indexes into the exported path
      //
      // if we are registering       \Namespace\Class\func( $param1, $param2 )
      // then our path is             namespace/class/func
      //                                      0/    1/   2
      // and arguments are                           array(       3,       4 )
      $page_arguments = array();
      $page_arg_names = array();
      $path_part_count = count( explode( '/', $menu_hook_path ) );
      $param_count = $path_part_count;

      $params = $path->getParameters();
      foreach ( $params as $param ) {
        $page_arguments[] = $param_count;
        $param_count++;
        // update argument name and default value arrays

        self::$_page_arg_names[$class_name][$path->name][] = $param->name;
        if ( $param->isOptional() ) {
          self::$_page_arg_defaults[$class_name][$path->name][] = $param->getDefaultValue();
        }
        else {
          self::$_page_arg_defaults[$class_name][$path->name][] = NULL;
        }
      }

      // check to see whether index has been defined at root controller level
      if ($menu_hook_path == 'index') {
        // check to determine whether index has parameters in order to route non-existant paths
        self::$_root_controller_index_has_arguments = count($params) > 0;

        // next set front page to point to index unles user provides explicit override
        // note that this behaviour is core default if index is detected at root level
        if ( isset( $path_options['site_frontpage_index'] ) && $path_options['site_frontpage_index'] ) {
          // to get optimal performance use cache_set instead of variable_set
          if ( $cached = cache_get( 'variables', 'cache' ) ) {
            $variables = $cached->data;
            $variables['site_frontpage'] = 'index';
            cache_set( 'variables', $variables );
          }
        }
      }

      // prepare menu_hook config to be passed to drupal --
      $menu_hook = array(
        'page callback'   => $prometheus_proxy_fn_name,
        'type'            => MENU_CALLBACK,
        'access callback' => TRUE,
        'page arguments'  => $page_arguments,
        'access arguments' => array('access content')
      );

      // merge any custom parameters, user can shoot themselves in the foot if they change 'page callback'
      if ( !empty($path_options['hook_menu']) ) {
        $menu_hook = self::merge_config( $menu_hook, $path_options['hook_menu'] );
      }

      // stash in the Prometheus menu hooks
      self::$_menu_hooks[$menu_hook_path] = $menu_hook;
    }
  }

  /** Invokes the path handler with specified arguments
   *
   * This function is called by the auto generated menu callback proxy function
   * and should not be manually invoked by the user.
   */
  public static function invoke( $menu_hook_path, $class_name, $path_name, $args ) {
    // TODO: Refactor so we don't need to turn of notices and errors
    $saved_error_level = error_reporting();
    error_reporting($saved_error_level ^ E_NOTICE ^ E_WARNING);

    $path_options = self::get_register_options( $class_name, $path_name );

    // start buffering the output if enabled
    $ob_enabled = $path_options['template']['ob_enable'];
    if ($ob_enabled) ob_start();

    // include $args into rest of $params
    self::merge_args_into_params( $class_name, $path_name, $args );

    // validate the request input and store results so they are available to user
    $input_results = self::process_input_main( $path_options );
    self::set_input_results( $input_results );

    // merge filtered data back into args
    self::merge_params_into_args( $class_name, $path_name, $args );

    // Call the path handler
    // Ensure we cast the return to array in case function returns something funky
    $variables = (array) call_user_func_array( array( self::get_instance($class_name), $path_name ), $args );

    // set the validation/filter results for the template/region
    $variables['input'] = $input_results;

    // save any output in a variable and stop buffering
    if ( $ob_enabled ) {
      $ob_content = ob_get_contents();
      ob_end_clean();
    }

    // determine if this is JSON view, in which case we need to assemble/send json data and return
    $json_view_options = is_array($path_options['view']['json']) ? $path_options['view']['json'] : NULL;
    if ( $path_options['view'] == 'json' || is_array($json_view_options) ) {
      if ( $json_view_options['root'] ) {
        $json_content = json_encode(
          $variables[$json_view_options['root']],
          $json_view_options['encode_options']
        );
      }
      else {
        $json_content = json_encode(
          $variables,
          $json_view_options['encode_options']
        );
      }
      header( 'Content-type: application/json' );
      print $json_content;
      return;
    }

    // base template path is theme/templates/our/uri/path
    $template_path = sprintf(
      '%s/%s/%s',
      path_to_theme(),
      TEMPLATES_DIR,
      $menu_hook_path
    );

    $params = self::get_parameters();

    // AJAX fragments support
    if ( isset($params[PROMETHEUS_AJAX]) ) {
      $ajax_response = array(
        PROMETHEUS_AJAX => $params[PROMETHEUS_AJAX]
      );
    }
    elseif ( $path_options['ajax_fragments'] ) {
      // History State Support
      if ($history_js_path = libraries_get_path("history.js")) {
        // Include the uncompressed version in case we need to debug the history library.
        // Site developer is free to enable JS+CSS compression and combining in their final product
        $script_path = $history_js_path . '/scripts/uncompressed';

        drupal_add_js($script_path . '/history.adapter.jquery.js');
        drupal_add_js($script_path . '/history.js');

        // IE 6 makes horrible paths, so disable support for history there
        //drupal_add_js($script_path . '/history.html4.js');
      }

      // Prometheus Ajax Fragmets Library
      drupal_add_js(drupal_get_path('module', 'prometheus') . '/js/prometheus.js');
    }

    // merge any template options that user returned back into options
    $path_options['template'] = self::merge_config( (array) $path_options['template'], (array) $variables['template'] );

    //    Determine if we are
    // 1. rendering multiple regions available to existing page context; or
    // 2. the new page as single region; or
    // 3. new standalone page

    if ( $path_options['template']['regions'] ) {
      $regions = (array) $path_options['template']['regions'];
      // pick the region to append the output buffer into
      if ( $ob_enabled === TRUE ) {
        // we pick the first region if ob_enable doesn't explicitly say which one
        $ob_region = $regions[0];
      }
      else {
        // use the region specified in ob_enable
        $ob_region = $ob_enabled;
      }
      foreach ( $regions as $region_name ) {
        // 1. for each region we look for theme/templates/our/uri/path/region_name.tpl.php
        $template_file_region = sprintf(
          '%s/%s.%s',
          $template_path,
          $region_name,
          TEMPLATES_EXT
        );

        // replace specified region with our content, check to make sure template file exists
        if ( file_exists( $template_file_region ) ) {
          $variables[$region_name] = theme_render_template( $template_file_region, $variables );
        }

        if ( $region_name == $ob_region ) {
          // prepend any buffered prints to the region
          $variables[$region_name] = $ob_content . $variables[$region_name];
        }

        if ( !empty($ajax_response) ) {
          $ajax_response[$region_name] = $variables[$region_name];
        }
      }
    }
    elseif ( $path_options['template']['load_as_region'] ) {
      // 2. we look for theme/templates/our/uri/path.tpl.php
      $region_name = $path_options['template']['load_as_region'];
      $template_file_region = sprintf(
        '%s.%s',
        $template_path,
        TEMPLATES_EXT
      );
      // replace specified region with our content, check to make sure template file exists
      if ( file_exists( $template_file_region ) ) {
        $variables[$region_name] = theme_render_template( $template_file_region, $variables );
      }

      if ( $ob_enabled === TRUE ) {
        // prepend our region with any buffered output
        $variables[$region_name] = $ob_content . $variables[$region_name];

        // AJAX fragment support
        if ( !empty($ajax_response) ) {
          $ajax_response[$region_name] = $variables[$region_name];
        }
      }
      elseif ( $ob_enabled ) {
        // use the region specified in ob_enable
        $variables[$ob_enabled] .= $ob_content;

        // AJAX fragment support
        if ( !empty($ajax_response) ) {
          $ajax_response[$ob_enabled] .= $variables[$ob_enabled];
        }
      }

    }
    else {
      if ( $ob_enabled === TRUE ) {
        // if output buffering was enabled but a region was not specified
        // now is as good a time as any to dump the buffered contents out
        print $ob_content;
      }
      elseif ( $ob_enabled ) {
        // if a specific region was named, append our content to it
        $variables[$ob_enabled] .= $ob_content;
      }

      // split the path into dir and file name components --
      $template_path_dir  = dirname(  $template_path );
      $template_path_file = basename( $template_path );
      // controller can override the file name
      if ( $path_options['template']['page'] ) {
        $template_path_file = $path_options['template']['page'];
      }

      // 3. render the whole page with our template
      array_unshift( $theme_registry['page']['theme path'], $template_path_dir );
      $theme_registry['page']['template'] = $template_path_file;
    }

    // AJAX fragments
    if ( !empty($ajax_response) ) {
      // ensure we send the new page title
      $ajax_response[PROMETHEUS_TITLE] = drupal_get_title();
      // encode and send to the browser
      $json_content = json_encode( $ajax_response );
      header( 'Content-type: application/json' );
      print $json_content;
      error_reporting($saved_error_level);
      return;
    }

    //$theme_registry['page']['arguments'] = self::merge_config( $theme_registry['page']['arguments'], $variables );

    //set modified theme registry internally and call the alter hook to manipulate it
    self::theme_registry( $theme_registry );
    theme_get_registry();

    $variables['page']['content'] = $variables['content'];
    $variables['page']['#children'] = theme( 'page', $variables );
    print drupal_render_page( $variables );


    error_reporting($saved_error_level);
  }

  /** Private function that updates path arguments from GET and POST data */
  private static function merge_args_into_params( $class_name, $path_name, $args ) {
    $arg_params = array();

    foreach ( (array)self::$_page_arg_names[$class_name][$path_name] as $index => $param_name ) {
      if ( array_key_exists($index, $args) ) {
        $arg_params[$param_name] = $args[$index];
      }
      else {
        $arg_params[$param_name] = self::$_page_arg_defaults[$index];
      }
    }
    // merge with GET, but let existing GETs override the uri args
    // get_parameters will then override any GETs with POSTs
    $_GET = array_merge($arg_params, $_GET);
  }

  /** Private function that updates path arguments from GET and POST data */
  private static function merge_params_into_args( $class_name, $path_name, &$args ) {
    $params = self::get_parameters();
    foreach ( (array)self::$_page_arg_names[$class_name][$path_name] as $index => $param_name ) {
      $args[$index] = $params[$param_name];
    }
  }

  /** Public function to validate and filter/sanitize request data dynamically
   *
   *  This is a wrapper around process_input_main which can be called by user
   */

  public static function process_input( $path_options, $parameters = NULL ) {
    $results = process_input_main( $path_options, $parameters );
    return $results;
  }

  /** Private function to validate and filter/sanitize request data
   *
   *  Uses native php filters
   */

  private static function process_input_main( &$path_options, $parameters = NULL ) {
    $input_options = $path_options['input'];

    if ( ! $parameters ) {
      $parameters = self::get_parameters();
    }

    $results = array();

    // process filter rules first
    $results['filter'] = self::process_input_options(
      $parameters,
      $input_options['filter'],
      'filter'
    );
    // override with filtered version and reset parameters
    foreach ( $results['filter'] as $field_name => $result_filtered ) {
      $parameters[$field_name] = $result_filtered['result'];
    }
    self::set_parameters( $parameters );

    // process validation rules next
    $results['validate'] = self::process_input_options(
      $parameters,
      $input_options['validate'],
      'validate'
    );

    return $results;
  }

  private static function process_input_options( &$parameters, $options, $process_type ) {
    $results = array();
    // process given options
    foreach ( (array) $options as $field_name => $field_option_rows ) {

      //determine if field is required first before running any other validation type of rules
      if ( $process_type == 'validate' ) {
        $validate_required = $field_option_rows['validate_required'];
        if ( $validate_required ) {
          if ( empty( $parameters[$field_name] ) ) {
            $results[$field_name] = array(
              'result' => FALSE,
              'last_known_rule' => 'validate_required'
            );
            continue;
          }
        }
      }

      foreach ( (array) $field_option_rows as $field_option_key => $field_option_value ) {
        if ( is_array( $field_option_value ) ) {
          $filter_type = $field_option_key;
          $filter_type_options = $field_option_value;
        }
        else {
          //determine if we are dealing with rule type only or if flags are passed in as well
          if ( is_string( $field_option_value ) ) {
            $filter_type = $field_option_value;
            $filter_type_options = array();
          }
          else {
            $filter_type = $field_option_key;
            $filter_type_options = $field_option_value;
          }
        }
        //skip validate_required filter since this is always run first
        if ( $process_type == 'validate' && $filter_type == 'validate_required' ) continue;
        $filter_id = self::get_filter_id( $filter_type );
        $final_result = filter_var(
          $parameters[$field_name],
          $filter_id,
          $filter_type_options
        );
        // return filtered values and any failed validations
        if ( ($process_type == "validate"  && $final_result === FALSE ) || ($process_type == "filter") ) {
          $results[$field_name] = array(
            'result' => $final_result,
            'last_known_rule' => $filter_type
          );
        }
        if ( $filter_type == FILTER_CALLBACK ) {
          $results[$field_name]['callback_function'] = $filter_type_options['options'];
        }
        // break the validation type of chain if any rule returns FALSE
        if ( $process_type == 'validate' && $final_result == FALSE ) break;
      }
    }
    return $results;
  }

  private static function get_filter_id( $filter_name ) {
    $filter_id_map = self::$_FILTER_TYPE_MAP_EXTENSIONS;
    $filter_id = $filter_id_map[$filter_name];
    if ( ! $filter_id ) {
      //try to find filter directly by querying built-ins instead
      $filter_id = filter_id( $filter_name );
    }
    return $filter_id;
  }

  /** Utility function to recursively merge configuration arrays
   *
   *  This differs from array_merge in that it will recursively merge
   *  the config tree rather than overwrite full branches.
   *  It will also not exibit the weird behaviour of array_merge_recursive where
   *  merging ( key => 'val1' ) and ( key => 'val2' ) produces ( key=> ('val1', 'val2') )
   *  rather than ( key => 'val2' )
   *
   *  Adapted from <andyidol at gmail dot com> MergeArray comment on http://php.net/manual/en/function.array-merge-recursive.php
   */
  private static function merge_config($config1, $config2) {
    foreach ( $config2 as $key => $value ) {
      if ( array_key_exists($key, $config1) && is_array($value) ) {
        $config1[$key] = self::merge_config( $config1[$key], $config2[$key] );
      }
      else {
        $config1[$key] = $value;
      }
    }
    return $config1;
  }

  /** Utility function to set/return theme registry
   *
   */
  public static function theme_registry( &$theme_registry = NULL ) {
    if ( $theme_registry ) {
      self::$_theme_registry = $theme_registry;
    }
    return self::$_theme_registry;
  }
}
