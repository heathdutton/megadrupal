<?php 


function dessert_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb">' . implode('<img src="' . base_path() . path_to_theme() . '/images/breadcrumb.gif" alt="&gt;" />', $breadcrumb) . '</div>';
    return $output;
  }
}

