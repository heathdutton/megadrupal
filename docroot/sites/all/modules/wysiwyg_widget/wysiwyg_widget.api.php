<?php

/**
 * @file
 * API documentation for wysiwyg_widget module.
 */

/**
 * Provide custom placeholders for widgets.
 *
 * @param $placeholders
 *   Data used for determining the placeholder to use for widgets.
 *
 */
function hook_wysiwyg_widget_embed_alter(&$placeholders) {

  $placeholders['tradingview_mediumwidget'] = array(
    // Required: an array containing regular expression info.
    'regex' => array(
      // Required: the regular expression pattern.
      'pattern' => 'TradingView.MediumWidget',
      // Optional: flags to be passed to new RegExp(pattern, flags);
      'flags' => 'gi',
    ),
    // Required: the image markup to be used for the placeholder.
    'icon_markup' => '<img class="wysiwyg-widget-embed-img" title="Widget embed" ' .
      'src="' . drupal_get_path('module', 'wysiwyg_widget') .
      '/plugins/widget_embed/images/tradingview_mediumwidget.png" />',
  );

}
