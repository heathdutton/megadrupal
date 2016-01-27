<?php

/**
 * Output breadcrumb as an unorderd list with unique and first/last classes
 */
function semantic_ui_omega_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $crumbs = '<ul class="breadcrumbs clearfix steps ui">';
    $crumbs .= '<li class="ui step disabled breadcrumb-home">' .
            l('<i class="icon home"></i>', '', array('html' => TRUE)) .
            '</li>';
    $array_size = count($breadcrumb);
    $i = 0;
    while ($i < $array_size) {
      $class = 'disabled';
      if (strstr($breadcrumb[$i], 'active')) {
        $class = 'active';
      }
      if ($i == 0) {
        $crumbs .= ' first';
        if (drupal_is_front_page ()) {
          $class = 'active';
        }
      }
      if ($i + 1 == $array_size) {
        $crumbs .= ' last';
      }
      $crumbs .= '<li class="ui step ' . $class . ' active non-home breadcrumb-' . $i . '">' .
              $breadcrumb[$i] .
              '</li>';
      $i++;
    }

    $crumbs .= '</ul>';
//    $crumbs .= '' .
//            l('go back <i class="icon mail reply large"></i>', '', array(
//                'html' => TRUE, 'attributes' => array('class' => array('back-link')))) .
//            '';
    return '<div class="item breadcrumb-nav">' . $crumbs . '</div>';
  }
}
