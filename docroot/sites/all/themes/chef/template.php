<?php

/**
 * @file
 * Theme functions
 */

/**
 * Implements template_preprocess_page().
 */
function chef_preprocess_page(&$variables) {
  // footer links
  if (module_exists('restaurant_base')) {
    $links = array(
      'twitter' => array(
        'title' => t('Follow Us on Twitter'),
        'sub_title' => t('Get the latest'),
        'href' => restaurant_base_get_settings('twitter'),
        'icon' => 'icon-twitter',
      ),
      'facebook' => array(
        'title' => t('We\'re on Facebook'),
        'sub_title' => t('Like Us'),
        'href' => restaurant_base_get_settings('facebook'),
        'icon' => 'icon-facebook',
      ),
      'contact' => array(
        'title' => t('Say Hello here'),
        'sub_title' => t('Keep in touch'),
        'href' => 'contact',
        'icon' => 'icon-envelope',
      ),
      'phone' => array(
        'title' => restaurant_base_get_settings('phone'),
        'sub_title' => t('Talk to Us'),
        'icon' => 'icon-phone',
      ),
    );

    $footer_links = '<div class="footer-links row-fluid">';
    foreach ($links as $link) {
      $title = $link['title'];
      $href = '';
      
      $options = array();
      $options['html'] = TRUE;

      if (isset($link['href'])) {
        $href = $link['href'];
      }
      else {
        $options['fragment'] = FALSE;
        $options['absolute'] = TRUE;
      }

      $icon = isset($link['icon']) ? $link['icon'] : '';
      $title .=  '<i class="' . $icon . '"></i>';

      $footer_links .= '<div class="span3">';
      $footer_links .= '<h4>' . $link['sub_title'] . '</h4>';
      $footer_links .= '<h3>' . l($title, $href, $options) . '</h3>';
      $footer_links .= '</div>';  
    }
    $footer_links .= '</div>';

    $variables['footer_links'] = $footer_links;
  }
}