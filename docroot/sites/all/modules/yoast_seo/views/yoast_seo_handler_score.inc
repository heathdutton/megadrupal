<?php

class yoast_seo_handler_score extends views_handler_field {
  /**
   * @inheritdoc
   */
  function query() {
    $this->ensure_my_table();
  }

  /**
   * Renders the field handler.
   */
  function render($values) {
    $output = '';

    if (user_access('use yoast seo')) {
      // Add our own CSS.
      drupal_add_css(drupal_get_path('module', 'yoast_seo') . '/css/yoast_seo.css');

      // Check if entity has SEO availability otherwise show a message.
      if (yoast_seo_entity_supports_yoast_seo('node', strtolower($values->node_type))) {
        // Score will be either 0 or a higher int. 0 is default.
        $score = yoast_seo_get_score($values->nid);
        // Class will represent classname for current score. Like poor or bad
        // it's used for theming purposes.
        $class = yoast_seo_score_rating($score);

        // Add Yoast score to the overview.
        $output = '<div id="yoast-overallscore" class="overallScore ' . $class . '"><div class="score_circle"></div></div>';
      }
      else {
        $output = '';
      }
    }

    return $output;
  }
}