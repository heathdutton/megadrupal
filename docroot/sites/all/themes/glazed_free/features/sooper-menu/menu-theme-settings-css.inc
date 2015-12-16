<?php

if (module_exists('color')) {
  module_load_include('module', 'color');
  $theme_colors = color_get_palette($theme);
  $base_color = $theme_colors['base'];
}
else {
  $base_color = "#1488cb";
}

if (theme_get_setting('menu_hover', $theme) == 'background') {
  $CSS .= <<<EOT
#navbar .navbar-nav a:hover,
#navbar .navbar-nav .active > a,
#navbar .navbar-nav .active > a:hover,
#navbar .navbar-nav .active > a:focus {
  color: #ffffff;
  background-color: {$base_color};
}

EOT;
}
elseif (theme_get_setting('menu_hover', $theme) == 'text') {
  $CSS .= <<<EOT
#navbar .navbar-nav a:hover,
#navbar .navbar-nav .active > a,
#navbar .navbar-nav .active > a:hover,
#navbar .navbar-nav .active > a:focus {
  color: {$base_color};
}

EOT;
}
elseif (theme_get_setting('menu_hover', $theme) == 'opacity') {
  $CSS .= <<<EOT
#navbar .navbar-nav a:hover,
#navbar .navbar-nav .active > a,
#navbar .navbar-nav .active > a:hover,
#navbar .navbar-nav .active > a:focus {
  opacity: 0.8;
}

EOT;
}


if (theme_get_setting('menu_type', $theme) == 'uppercase') {
  $CSS .= <<<EOT
.glazed-header .menu > li > a {
  text-transform: uppercase;
}

EOT;
}
elseif (theme_get_setting('menu_type', $theme) == 'bold') {
  $CSS .= <<<EOT
.glazed-header .menu > li > a {
  font-weight: bold;
}

EOT;
}
elseif (theme_get_setting('menu_type', $theme) == 'lead') {
  $CSS .= <<<EOT
.glazed-header .menu > li > a {
  font-size: 16px;
  font-weight: 300;
}
@media (min-width: 768px) {
  .glazed-header .menu > li > a {
    font-size: 21px;
  }
}

EOT;
}
