<?php
include_once(drupal_get_path('theme', 'light') . '/common.inc');

function light_theme() {
  $items = array();
  $items['render_panel'] = array(
    "variables" => array(
      'page' => array(),
      'panels_list' => array(),
      'panel_regions_width' => array(),
    ),
    'preprocess functions' => array(
      'light_preprocess_render_panel'
    ),
    'template' => 'panel',
    'path' => drupal_get_path('theme', 'light') . '/tpl',
  );
  return $items;
}

function light_process_html(&$vars) {
  $current_skin = theme_get_setting('skin');
  if (isset($_COOKIE['light_skin'])) {
    $current_skin = $_COOKIE['light_skin'];
  }
  if (!empty($current_skin) && $current_skin != 'default') {
  	$vars['classes'] .= " skin-$current_skin"; 
  }
}

function light_preprocess_page(&$vars) {
  global $theme_key;
  $vars['regions_width'] = light_regions_width($vars['page']);
  $panel_regions = light_panel_regions();
  if (count($panel_regions)) {
    foreach ($panel_regions as $panel_name => $panels_list) {
      $panel_markup = theme("render_panel", array(
        'page' => $vars['page'],
        'panels_list' => $panels_list,
        'regions_width' => $vars['regions_width'],
      ));
      $panel_markup = trim($panel_markup);
      $vars[$panel_name] = empty($panel_markup) ? FALSE : $panel_markup;
    }
  }
  $current_skin = theme_get_setting('skin');
  if (isset($_COOKIE['light_skin'])) {
    $current_skin = $_COOKIE['light_skin'];
  }

  $vars['page']['show_skins_menu'] = $show_skins_menu = theme_get_setting('show_skins_menu');
  if($show_skins_menu) {
    $skins = light_get_predefined_param('skins', array("default" => t("Default Style")));
    $current_skin = theme_get_setting('skin');
    if (isset($_COOKIE['light_skin'])) {
      $current_skin = $_COOKIE['light_skin'];
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
			jQuery.cookie("light_skin", style, {path: "' . base_path() . '"});
			window.location.reload();
			return false;
		  });
		}
	  }
	})(jQuery);
  ', 'inline');
  $vars['page']['light_skin_classes'] = !empty($current_skin) ? ($current_skin . "-skin") : "";
  if (!empty($current_skin) && $current_skin != 'default' && theme_get_setting("default_logo") && theme_get_setting("toggle_logo")) {
    $vars['logo'] = file_create_url(drupal_get_path('theme', $theme_key)) . "/css/colors/" . $current_skin . "/images/logo.png";
  }
}

function light_preprocess_render_panel(&$variables) {
  $page = $variables['page'];
  $panels_list = $variables['panels_list'];
  $regions_width = $variables['regions_width'];
  $variables = array();
  $variables['page'] = array();
  $variables['panel_width'] = $regions_width;
  $variables['panel_classes'] = array();
  $variables['panels_list'] = $panels_list;
  $is_empty = TRUE;
  $panel_keys = array_keys($panels_list);

  foreach ($panels_list as $panel) {
    $variables['page'][$panel] = $page[$panel];
    $panel_width = $regions_width[$panel];
    if (render($page[$panel])) {
      $is_empty = FALSE;
    }
    $classes = array("panel-column", "span" . $panel_width, str_replace("_", "-", $panel));
    $variables['panel_classes'][$panel] = implode(" ", $classes);
  }
  $variables['empty_panel'] = $is_empty;
}

function light_css_alter(&$css) {
  global $theme_key;
  $skin = theme_get_setting('skin');
  if (isset($_COOKIE['light_skin'])) {
	$skin = $_COOKIE['light_skin'] == 'default' ? '' : $_COOKIE['light_skin'];
  }
  if (!empty($skin) && file_exists(drupal_get_path('theme', $theme_key) . "/css/colors/" . $skin . "/style.css")) {
    $css = drupal_add_css(drupal_get_path('theme', $theme_key) . "/css/colors/" . $skin . "/style.css", array(
      'group' => CSS_THEME,
    ));
  }
}

function light_preprocess_maintenance_page(&$vars) {
}

