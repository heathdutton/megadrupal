<?php
/**
 * @file
 * Override of preprocess functions.
 */

/**
 * Preprocess theme function to print a single record from a row, with fields
 */
function zircon_preprocess_views_view_fields(&$vars) {
  if (isset($vars['view']->style_plugin->definition['module']) && $vars['view']->style_plugin->definition['module'] == 'views_slideshow') {
    $fields = $vars['fields'];
    if (count($fields) >= 2) {
      $fields_key = array_keys($fields);
      $fields[$fields_key[1]]->wrapper_prefix = '<div class="slideshow-group-fields-wrapper">' . $fields[$fields_key[1]]->wrapper_prefix;
      $fields[$fields_key[count($fields_key) - 1]]->wrapper_suffix = $fields[$fields_key[count($fields_key) - 1]]->wrapper_suffix . '</div>';
    }
  }
}

/**
 * Implements hook_css_alter().
 */
function zircon_css_alter(&$css) {
  global $theme_key;
  $skin = theme_get_setting('skin');
  if (isset($_COOKIE['nucleus_skin'])) {
    if ($_COOKIE['nucleus_skin'] != $skin) {
      $nucleus_skin = $_COOKIE['nucleus_skin'];
      if (!empty($nucleus_skin) && file_exists(drupal_get_path('theme', $theme_key) . "/skins/" . $nucleus_skin . "/style.css")) {
        $css = drupal_add_css(drupal_get_path('theme', $theme_key) . "/skins/" . $nucleus_skin . "/style.css", array(
          'group' => CSS_THEME,
        ));
      }
      if ($nucleus_skin == "default" || file_exists(drupal_get_path('theme', $theme_key) . "/skins/" . $nucleus_skin . "/style.css")) {
        unset($css[drupal_get_path('theme', $theme_key) . "/skins/" . $skin . "/style.css"]);
      }
    }
  }
}

/**
 * Override or insert variables into the page template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function zircon_preprocess_page(&$vars) {
  global $theme_key;
  $current_skin = theme_get_setting('skin');
  if (isset($_COOKIE['nucleus_skin'])) {
    $current_skin = $_COOKIE['nucleus_skin'];
  }

  $vars['page']['show_skins_menu'] = $show_skins_menu = theme_get_setting('show_skins_menu');
  if($show_skins_menu) {
    $skins = nucleus_get_predefined_param('skins', array("default" => t("Default Style")));
    $current_skin = theme_get_setting('skin');
    if (isset($_COOKIE['nucleus_skin'])) {
      $current_skin = $_COOKIE['nucleus_skin'];
    }
	$str = array();
    $str[] = '<div id="change_skin_menu_wrapper" class="change-skin-menu-wrapper wrapper">';
    $str[] = '<div class="container">';
    $str[] = '<ul class="change-skin-menu">';
    
	foreach ($skins as $skin => $skin_title) {
	  $li_class = ($skin == $current_skin ? ($skin . ' active') : $skin);
	  $str[] = '<li class="' . $li_class . '"><a href="#change-skin/' . $skin . '" class="change-skin-button color-' . $skin . '">' . $skin_title . '</a></li>';
	}
	$str[] = '</ul></div></div>';
	$vars['page']['show_skins_menu'] = implode("", $str);
  }
  $default_logo = theme_get_setting("default_logo");
  $toggle_logo = theme_get_setting("toggle_logo");
  if (!empty($current_skin) && $current_skin != 'default'  && $default_logo && $toggle_logo) {
    $vars['logo'] = file_create_url(drupal_get_path('theme', $theme_key)) . "/skins/" . $current_skin . "/logo.png";
  }
  drupal_add_js('
	(function ($) {
	  if (Drupal.Nucleus == undefined) {
		Drupal.Nucleus = {};
	  }
	  Drupal.behaviors.skinMenuAction = {
		attach: function (context) {
		  jQuery(".change-skin-button").click(function() {
			parts = this.href.split("/");
			style = parts[parts.length - 1];
			jQuery.cookie("nucleus_skin", style, {path: "' . base_path() . '"});
			window.location.reload();
			return false;
		  });
		}
	  }
	})(jQuery);
  ', 'inline');  
}

/**
 *
 */
function zircon_preprocess_html(&$vars) {
  $current_skin = theme_get_setting('skin');
  if (isset($_COOKIE['nucleus_skin'])) {
    $current_skin = $_COOKIE['nucleus_skin'];
  }
  if(!empty($current_skin) && $current_skin != 'default') {
    $vars['classes_array'][] = $current_skin . "-skin";
  }  
}

/**
 *
 */
function zircon_preprocess_node(&$vars) {
  
  $skins = zircon_get_predefined_param('skins', array('default' => t("Default skin")));
  foreach ($skins as $key => $val) {
    if ($vars['node_url'] == base_path() . 'skins/' . $key && (!isset($_COOKIE['nucleus_skin']) || $_COOKIE['nucleus_skin'] != $key)) {
      setcookie('nucleus_skin', $key, time() + 100000, base_path());
      header('Location: ' . $vars['node_url']);
    }
  }

}


function zircon_get_predefined_param($param, $pre_array = array(), $suf_array = array()) {
  global $theme_key;
  $theme_data = list_themes();
  $result = isset($theme_data[$theme_key]->info[$param]) ? $theme_data[$theme_key]->info[$param] : array();
  return $pre_array + $result + $suf_array;
}