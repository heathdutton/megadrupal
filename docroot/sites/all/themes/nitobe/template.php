<?php
// $Id: template.php,v 1.47 2010/11/28 21:38:23 shannonlucas Exp $
/**
 * @file
 * The core functions for the Nitobe theme.
 */

require_once drupal_get_path('theme', 'nitobe') . '/nitobe.utils.inc';


/**
 * Returns the path to the main Nitobe theme directory.
 *
 * @return string
 *   The path to Nitobe.
 */
function nitobe_theme_path() {
  static $theme_path;

  if (!isset($theme_path)) {
    global $theme;

    if ($theme == "nitobe") {
      $theme_path = path_to_theme();
    } else {
      $theme_path = drupal_get_path("theme", "nitobe");
    }
  }

  return $theme_path;
}


/**
 * Overrides theme_pager() and alters the default quantity of pager items.
 *
 * @param array $tags
 *   Labels for the controls in the pager.
 * @param int $limit
 *   The number of query results to display per page.
 * @param int $element
 *   An optional ID to distinguish between multiple pagers on one page.
 * @param array $parameters
 *   An mapping of query string parameters to append to the pager links.
 * @param int $quantity
 *   The number of page items to show in the pager. If this value is zero (0),
 *   the item count specified by the theme setting nitobe_pager_page_count will
 *   be used (5 if not set).
 *
 * @return string
 *   The HTML that generates the query pager.
 */
function nitobe_pager($tags = array(), $limit = 10, $element = 0,
                      $parameters = array(), $quantity = 0) {
  if ($quantity == 0) {
    $quantity = theme_get_setting("nitobe_pager_page_count");
    $quantity = empty($quantity) ? 5 : $quantity;
  }

  return theme_pager($tags, $limit, $element, $parameters, $quantity);
}


/**
 * Decorates theme_username() to strip the " (not verified)" string from the
 * commenter's name.
 *
 * @param object $account
 *   An instance of a user object.
 *
 * @return string
 *   The altered HTML output from theme_username().
 */
function __nitobe_username($account) {
  $output = theme_username($account);

  if ((boolean)theme_get_setting("nitobe_remove_not_verified")) {
    $to_strip = " (" . t("not verified") . ")";
    $output   = str_replace($to_strip, "", $output);
  }

  return $output;
}


/**
 * Preprocesses the user picture.
 *
 * If no default user picture is provided, and pictures are enabled, use the
 * theme's default user picture.
 *
 * @param array &$variables
 *   The template arguments.
 */
function nitobe_preprocess_user_picture(&$variables) {
  $account = $variables["account"];

  if (empty($variables["user_picture"]) && (variable_get("user_pictures", 0) != 0)) {
    $picture = nitobe_theme_path() . "/images/user-icon.jpg";
    $name    = $account->name ? $account->name :
                                variable_get("anonymous", t("Anonymous"));
    $alt     = t("@user's picture", array('@user' => $name));

    $variables["user_picture"] =
      theme("image", array("path" => $picture, "alt" => $alt, "title" => $alt));

    // -- Link the picture if allowed.
    if (!empty($account->uid) && user_access("access user profiles")) {
      $attributes = array(
        "attributes" => array(
      	  "title" => t("View user profile."),
        ),
      	"html" => TRUE
      );

      $variables["user_picture"] =
        l($variables["user_picture"], "user/{$account->uid}", $attributes);
    }
  }
}


/**
 * Determines whether to show the date stamp for the given node.
 *
 * @param string $type
 *   The machine readable name of the type to check.
 *
 * @return boolean
 *   TRUE if the node is of a type that should show the date stamp, FALSE if
 *   not.
 */
function nitobe_show_datestamp($type) {
  $default     = drupal_map_assoc(array("blog", "forum", "poll", "article"));
  $valid_types = theme_get_setting("nitobe_show_datestamp");
  $valid_types = (!empty($valid_types)) ? $valid_types : $default;

  return (array_key_exists($type, $valid_types) && ($valid_types[$type] === $type));
}


/**
 * Produces the title effect.
 *
 * Removes the spaces between words in the given string and returns an HTML
 * string with every other word wrapped in a span with the class "alt-color".
 *
 * @param string $title
 *   The text to render.
 *
 * @return string
 *   The rendered HTML.
 */
function nitobe_title_effect($title = "") {
  $words  = explode(" ", $title);
  $result = "";

  if (is_array($words)) {
    $alt = FALSE;
    foreach ($words as $word) {
      if ($alt) {
        $result .= "<span class=\"alt-color\">{$word}</span>";
      } else {
        $result .= $word;
      }

      $alt = !$alt;
    }
  }

  return $result;
}


/**
 * Adds the JavaScript and CSS required for the masthead image.
 *
 * @param array &$vars
 *   The page template variables.
 */
function _nitobe_add_masthead_image(&$vars) {
  // -- Determine the header image if it is set, or add the JavaScript for
  // -- random header images.
  $header_img = theme_get_setting("nitobe_header_image");
  $header_img = empty($header_img) ? "<random>" : $header_img;
  $css        = _nitobe_fixed_header_css($header_img);
  $css_opts   = array(
    "type" => "inline",
    "group" => CSS_THEME,
  );

  if ($header_img == "<random>") {
    $image   = array_rand(_nitobe_get_header_list());
    $css     = _nitobe_fixed_header_css($image);
    $js      = _nitobe_random_header_js();
    $js_opts = array(
      "type"  => "inline",
      "group" => JS_THEME,
    );

    drupal_add_js($js, $js_opts);
  }

  drupal_add_css($css, $css_opts);
}


/**
 * Preprocess the nodes.
 *
 * @param array &$variables
 *   The template variables. After invoking this function, these keys will be
 *   added to $variables:
 *   - nitobe_node_author: The node's "posted by" text and author link.
 *   - nitobe_perma_title: The localized permalink text for the node.
 *   - nitobe_show_meta: Indicates whether the meta data div should be
 *     rendered.
 *   - nitobe_node_timestamp: The timestamp for this type, if one should be
 *     rendered for this type.
 */
function nitobe_preprocess_node(&$variables) {
  $content = $variables["content"];
  $node    = $variables["node"];

  $variables["nitobe_perma_title"] =
    t("Permanent Link to !title", array("!title" => $variables['title']));

  if ($variables["display_submitted"]) {
    $variables["nitobe_node_author"] =
      t("Posted by !author", array("!author" => $variables["name"]));
  }

  if ($variables["display_submitted"] && isset($node->created)) {
    $date_format = theme_get_setting("nitobe_node_datestamp_format");
    $variables['nitobe_node_timestamp'] = 
      empty($date_format) ? "" : format_date($node->created, "custom", $date_format);
  }

  $variables["nitobe_show_meta"] = !empty($content["field_tags"]) ||
                                   !empty($content["links"]);
}


/**
 * Provides page variables for the maintenance page.
 *
 * @param array &$vars
 *   The template variables. After invoking this function, this array will have
 *   the same added values provided by nitobe_preprocess_page().
 *
 * @see nitobe_preprocess_page
 */
function nitobe_preprocess_maintenance_page(&$variables) {
  nitobe_preprocess_page($variables);
  nitobe_process_page($variables);
}


/**
 * Preprocesses the regions.
 *
 * The function checks for the presence of the keys "#nitobe_classes" and
 * "#nitobe_vars" in $variables["elements"] and uses them to add classes and
 * template variables, respectively.
 *
 * @param array &$variables
 *   The template variables.  After invoking this function, &$variables will
 *   contain the key "nitobe_force_render" indicating that the region should
 *   render even if empty.
 *
 * @see region.tpl.php
 */
function nitobe_preprocess_region(&$variables) {
  $region   = $variables["region"];
  $elements = $variables["elements"];

  // -- Indicate if the theme should render this region even if it is empty.
  $variables["nitobe_force_render"] = isset($elements["#nitobe_force_render"]) ?
                                      $elements["#nitobe_force_render"] : FALSE;

  // -- If any layout classes were added by nitobe_preprocess_page(), those
  // -- are added here.
  if (!empty($elements["#nitobe_classes"])) {
    $variables["classes_array"] =
      array_merge($elements["#nitobe_classes"], $variables["classes_array"]);
  }

  // -- If there are any Nitobe specific variables added by
  // -- nitobe_preprocess_page(), add those to the region's template scope.
  if (!empty($elements["#nitobe_vars"])) {
    foreach ($elements["#nitobe_vars"] as $name => $value) {
      $variables[$name] = $value;
    }
  }
}


/**
 * Builds the title_group region.
 *
 * After this function is invoked, the array &$variables["page"]["title_group"]
 * will be populated with the necessary data to render the region.  This will
 * result in the necessary classes being added to the region and will add
 * the following variables to region--title-group.tpl.php:
 * - $front_page The path of the site's front page.
 * - $logo The URI for the site logo, if present.
 * - $nitobe_title The site title with Nitobe's title effect applied.
 * - $title The page title, if present
 * - $title_prefix An array of title prefix data for this page.
 * - $title_suffix An array of title suffix data for this page.
 * - $site_name The site's name.
 * - $site_slogan The site slogan, if present.
 *
 * @note This is only invoked by nitobe_preprocess_page() in order to pass
 *   necessary layout information to the region.
 *
 * @param array &$variables
 *   The page template variables passed from nitobe_preprocess_page().
 *
 * @see nitobe_preprocess_region()
 * @see region--title-group.tpl.php
 * @see page.tpl.php
 */
function _nitobe_build_title_group(&$variables) {
  $page        = $variables["page"];
  $nitobe_vars = array();

  $class = empty($page["header"]) ? "grid-16" : "grid-10";
  $variables["page"]["title_group"]["#nitobe_classes"] = array($class);

  // -- Handle the title effect
  if (isset($variables["site_name"]) &&
      ((boolean)theme_get_setting("nitobe_title_effect") == TRUE)) {
    $nitobe_vars["nitobe_title"] = nitobe_title_effect(check_plain($variables["site_name"]));
  } else {
    $nitobe_vars["nitobe_title"] = check_plain($variables["site_name"]);
  }
  
  // -- Copy necessary values from the page variables.
  $keys = array("front_page", "logo", "site_name", "site_slogan", "title");
  _nitobe_copy_if_exists($variables, $nitobe_vars, $keys);

  $variables["page"]["title_group"]["#region"]              = "title_group";
  $variables["page"]["title_group"]["#sorted"]              = TRUE;
  $variables["page"]["title_group"]["#nitobe_force_render"] = TRUE;
  $variables["page"]["title_group"]["#theme_wrappers"]      = array("region");
  $variables["page"]["title_group"]["#nitobe_vars"]         = $nitobe_vars;
}


/**
 * Builds the menu bar.
 *
 * After this function is invoked, the array &$variables["page"]["menu_bar"]
 * will be populated with the necessary data to render the region.  This will
 * result in the necessary classes being added to the region and will add
 * the following variables to region--menu-bar.tpl.php if primary and
 * secondary menus are available:
 * - $main_menu The array of primary links.
 * - $nitobe_main_menu The HTML for the rendered primary links.
 * - $nitobe_secondary_menu The HTML for the rendered secondary links.
 * - $secondary_menu The array of secondary links.
 *
 * @note This is only invoked by nitobe_preprocess_page() in order to pass
 *   necessary layout information to the region.
 *
 * @param array &$variables
 *   The page template variables passed from nitobe_preprocess_page().
 *
 * @see nitobe_preprocess_region()
 * @see region--menu-bar.tpl.php
 * @see page.tpl.php
 */
function _nitobe_build_menu_bar(&$variables) {
  $nitobe_vars = array();

  if (isset($variables["secondary_menu"])) {
    $nitobe_vars["nitobe_secondary_menu"] = theme("links", array(
      "links"      => $variables["secondary_menu"],
      "attributes" => array(
        "id"    => "secondary-nav",
        "class" => array("secondary-nav", "inline", "links", "secondary-menu", "grid-16"),
      ),
      "heading" => array(
        "text"  => t("Secondary menu"),
        "level" => "h2",
        "class" => array("element-invisible"),
      )
    ));
  }

  if (!empty($variables["main_menu"])) {
    $classes = array("primary-nav", "inline", "links", "main-menu", "grid-16");

    if (!empty($nitobe_vars["nitobe_secondary_menu"])) {
      $classes[] = "has-secondary";
    }

    $nitobe_vars["nitobe_main_menu"] = theme("links", array(
      "links"      => $variables["main_menu"],
      "attributes" => array(
        "id"    => "primary-nav",
        "class" => $classes,
      ),
      "heading" => array(
        "text"  => t("Main menu"),
        "level" => "h2",
        "class" => array("element-invisible"),
      )
    ));
  }

  // -- Copy necessary values from the page variables.
  $keys = array("main_menu", "secondary_menu");
  _nitobe_copy_if_exists($variables, $nitobe_vars, $keys);

  $variables["page"]["menu_bar"]["#nitobe_classes"] = array("grid-16", "nav-links");
  $variables["page"]["menu_bar"]["#sorted"]         = TRUE;
  $variables["page"]["menu_bar"]["#theme_wrappers"] = array("region");
  $variables["page"]["menu_bar"]["#region"]         = "menu_bar";
  $variables["page"]["menu_bar"]["#nitobe_vars"]    = $nitobe_vars;
}


/**
 * Builds the masthead region.
 *
 * @note This is only invoked by nitobe_preprocess_page() in order to pass
 *   necessary layout information to the region.
 *
 * @param array &$variables
 *   The page template variables passed from nitobe_preprocess_page().
 *
 * @see nitobe_preprocess_region()
 * @see region--masthead.tpl.php
 * @see page.tpl.php
 */
function _nitobe_build_masthead(&$variables) {
  // -- Determine if the masthead image should be displayed.
  $force_header = theme_get_setting('nitobe_header_always_show');
  $masthead     = $variables["page"]["masthead"];

  if ($force_header || empty($variables["page"]["masthead"])) {
    _nitobe_add_masthead_image($variables);
  }

  // -- The Masthead region needs to always render in order to show the image,
  // -- so if it's empty, we need to populate it with something.
  if (empty($masthead)) {
    $variables["page"]["masthead"]["#region"]              = "masthead";
    $variables["page"]["masthead"]["#sorted"]              = TRUE;
    $variables["page"]["masthead"]["#nitobe_force_render"] = TRUE;
    $variables["page"]["masthead"]["#theme_wrappers"]      = array("region");
  }
}


/**
 * Builds the footer columns.
 *
 * Specifically, the function adds the necessary grid class to the footer
 * columns and ensures that their containing divs render even when empty.
 *
 * @note This is only invoked by nitobe_preprocess_page() in order to pass
 *   necessary layout information to the region.
 *
 * @param array &$variables
 *   The page template variables passed from nitobe_preprocess_page(). After
 *   invoking this function, &$variables will have the key
 *   "nitobe_footers_empty" which indicates that all of the footer columns are
 *   empty if TRUE.
 *
 * @see region.tpl.php
 * @see page.tpl.php
 */
function _nitobe_build_footer_columns(&$variables) {
  $columns = array("footer_firstcolumn", "footer_secondcolumn",
                   "footer_thirdcolumn", "footer_fourthcolumn");
  $page = $variables["page"];

  // -- Are all of the footer column regions empty?
  $variables["nitobe_footers_empty"] =
    empty($page["footer_firstcolumn"]) && empty($page["footer_secondcolumn"]) &&
    empty($page["footer_thirdcolumn"]) && empty($page["footer_fourthcolumn"]);

  // -- Add the grid-4 class and force any empty columns to at least render
  // -- their containing div in order to maintain the layout.
  foreach ($columns as $column) {
    if (empty($variables["page"][$column])) {
      $variables["page"][$column]["#region"]              = $column;
      $variables["page"][$column]["#sorted"]              = TRUE;
      $variables["page"][$column]["#nitobe_force_render"] = TRUE;
      $variables["page"][$column]["#theme_wrappers"]      = array("region");
    }

    $variables["page"][$column]["#nitobe_classes"] = array("grid-4");
  }
}


/**
 * Preprocesses pages.
 *
 * @param array &$variables
 *   The template variables. After invoking this function, these keys will be
 *   added to $variables:
 *   - nitobe_footers_empty: TRUE if all of the footer column regions are
 *     empty.
 *   - nitobe_tabs_primary: The array of primary tabs for this page.
 *   - nitobe_tabs_secondary: The array of secondary tabs for this page.
 */
function nitobe_preprocess_page(&$variables) {
  $variables["nitobe_tabs_primary"]   = $variables["tabs"]['#primary'];
  $variables["nitobe_tabs_secondary"] = $variables["tabs"]['#secondary'];
  
  // -- Determine which layout to use.
  nitobe_set_layout($variables);

  // -- Build the special regions.
  _nitobe_build_title_group($variables);
  _nitobe_build_menu_bar($variables);
  _nitobe_build_masthead($variables);
  _nitobe_build_footer_columns($variables);
}


/**
 * Does final page processing before rendering.
 *
 * Specifically, this implementation pre-renders the page title header and
 * provides the node timestamp, if applicable.
 *
 * @param array &$variables
 *   The template variables.  After invoking this function, these keys will be
 *   added:
 *   - nitobe_node_timestamp The node timestamp, if applicable.
 *   - nitobe_page_title: The pre-rendered page title element with the
 *     appropriate CSS classes assigned.
 *
 * @see page.tpl.php
 * @see nitobe_show_datestamp()
 */
function nitobe_process_page(&$variables) {
  // -- Pre-render the page title with the appropriate CSS classes.
  if (isset($variables["title"])) {
    $classes_array = array("page-title");
    $title   = $variables["title"];

    if ($variables["tabs"]["#primary"]) {
      $classes_array[] = "with-tabs";
    }

    $classes = implode(" ", $classes_array);
    $variables["nitobe_page_title"] = "<h1 class=\"{$classes}\">{$title}</h1>";
  }

  // -- The formatted date for this node, if the date should be rendered.
  $node = isset($variables["node"]) ? $variables["node"] : NULL;

  if (!empty($node) && isset($node->created) && nitobe_show_datestamp($node->type)) {
    $date_format = theme_get_setting("nitobe_node_datestamp_format");
    $variables['nitobe_node_timestamp'] = 
      empty($date_format) ? "" : format_date($node->created, "custom", $date_format);
  }
}


/**
 * Preprocess the wrapping HTML.
 *
 * @param array &$variables
 *   The template variables.
 */
function nitobe_preprocess_html(&$variables) {
  $language = $variables["language"];
  $styles   = nitobe_theme_path() . "/styles/";
  $is_rtl   = (defined("LANGUAGE_RTL") && ($language->dir == LANGUAGE_RTL));

  // -- Force framework and reset CSS before everything else.
  $stylesheet = $is_rtl ? "framework/960-rtl.css" : "framework/960.css";
  $framework = array(
    "group"  => CSS_SYSTEM - 1,
    "weight" => -10,
  );

  drupal_add_css($styles . "framework/reset.css", $framework);
  drupal_add_css($styles . $stylesheet, $framework);

  // -- IE CSS fixes.
  $stylesheet   = $is_rtl ? "fix-ie-rtl.css" : "fix-ie.css";
  $ie_condition = array(
    "group"      => CSS_THEME,
    "browsers"   => array("IE" => "lt IE 8", "!IE" => FALSE),
    "preprocess" => FALSE,
  );

  drupal_add_css($styles . $stylesheet, $ie_condition);

  $variables["classes_array"][] = "nitobe";
}


/**
 * Overrides template_preprocess_comment().
 *
 * @param array &$variables
 *   The template variables. After invoking this function, these keys will be
 *   added to $vars:
 *   - links: The comment links.
 *   - nitobe_attribution: The formatted author link and date for the comment's
 *     meta data area.
 */
function nitobe_preprocess_comment(&$variables) {
  $comment = $variables["comment"];
  $author  = isset($variables["author"]) ? $variables["author"] : NULL;
  
  if ($author == NULL) {
    $vars = array("account" => $variables["user"]);
    $author = theme("username", $vars);
  }
  
  // -- The author and timestamp
  $date_format  = theme_get_setting("nitobe_comment_date_format");
  $time_format  = theme_get_setting("nitobe_comment_time_format");
  $comment_date = 
    empty($date_format) ? "" : format_date($comment->created, "custom", $date_format);
  $comment_time = 
    empty($time_format) ? "" : format_date($comment->created, "custom", $time_format);
  
  $params = array(
    '@date'   => $comment_date,
    '@time'   => $comment_time,
    '!author' => $author,
  );

  if ($comment_date && $comment_time) {
    $variables['nitobe_attribution'] =
      t("Posted by !author on @date at @time.", $params);
  }
  else if ($comment_date) {
    $variables['nitobe_attribution'] =
      t("Posted by !author on @date.", $params);
  }
  else if ($comment_time) {
    $variables['nitobe_attribution'] =
      t("Posted by !author at @time.", $params);
  }
  else {
    $variables['nitobe_attribution'] =
      t("Posted by !author.", $params);
  }

  // -- Adds the zebra state so we can visually differentiate every other
  // -- comment.
  $variables["classes_array"][] = $variables["zebra"];
}


/**
 * Renders the local tasks.
 *
 * The default implementation renders them as tabs. Overridden to split the
 * secondary tasks.
 *
 * @return string
 *   The rendered local tasks.
 */
function nitobe_menu_local_tasks() {
  return menu_primary_local_tasks();
}


/**
 * Generates IE CSS links for LTR and RTL languages.
 *
 * @return string
 *   The IE style elements.
 */
function nitobe_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="screen" href="' .
           base_path() . path_to_theme() . '/styles/fix-ie.css" />';
  if (defined('LANGUAGE_RTL') && $language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="screen">@import "' .
              base_path() . path_to_theme() . '/styles/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}


/**
 * Determines the layout.
 *
 * The layout is determined the nitobe_content_placement setting and number of
 * sidebars that have content.
 *
 * @param array &$variables
 *   The template variables. After invoking this function, these keys will be
 *   added to $vars:
 *   - nitobe_content_width: The CSS class providing the full width of the
 *     content region without any push/pull classes.
 *   - nitobe_placement: The theme setting for how the sidebars should be
 *     rendered relative to the content region. Will be one of: 'left',
 *     'center', or 'right'.
 */
function nitobe_set_layout(&$variables) {
  // -- Add the layout variables.
  $placement = theme_get_setting("nitobe_content_placement");
  $placement = empty($placement) ? "center" : $placement;
  $layout    = $variables["layout"];
  $variables["nitobe_placement"] = $placement;

  // -- Determine the classes for the content and sidebars.
  $has_first  = (($layout == "first")  || ($layout == "both"));
  $has_second = (($layout == "second") || ($layout == "both"));

  $content = nitobe_ns("grid-16", $has_first, 4, $has_second, 4);
  $variables["nitobe_content_width"]                      = $content;
  $variables["page"]["sidebar_first"]["#nitobe_classes"]  = array("grid-4");
  $variables["page"]["sidebar_second"]["#nitobe_classes"] = array("grid-4");

  // -- The grid class for items in the main column.
  $variables["nitobe_content_width"] =
    nitobe_ns("grid-16", $has_first, 4, $has_second, 4);


  // -- Add the push/pull classes.
  $push_pull = _nitobe_get_push_pull();

  foreach (array("sidebar_first", "sidebar_second") as $region) {
    if (isset($push_pull[$placement][$layout][$region])) {
      $variables["page"][$region]["#nitobe_classes"][] =
        $push_pull[$placement][$layout][$region];
    }
  }
}


/**
 * Generates the JavaScript for rotating the header image.
 *
 * @return string
 *   The JavaScript for rotating the header.
 */
function _nitobe_random_header_js() {
  global $base_url;

  $files = _nitobe_get_header_list();
  $names = array();

  foreach ($files as $file => $data) {
    $names[] = $base_url . '/' . $file;
  }

  $names_js = drupal_json_encode($names);
  $js       = <<<EOJS
jQuery(document).ready(function() {
  var names = {$names_js};
  jQuery(".region-masthead").css("background-image", "url(" + names[Math.floor(Math.random() * names.length)] + ")");
});
EOJS;

  return $js;
}


/**
 * Generates the CSS to place inline to choose the header background image.
 *
 * @param string $filename
 *   The header image filename, relative to the theme.
 *
 * @return string
 *   The CSS to add to the header.
 */
function _nitobe_fixed_header_css($filename) {
  global $base_url;

  $url    = $base_url . "/" . $filename;
  $output = ".region-masthead{background-image:url(%s);}";

  return sprintf($output, $url);
}

