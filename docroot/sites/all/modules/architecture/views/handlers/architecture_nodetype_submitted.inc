<?php

/**
 * @file
 * Views handler override for field that exposes nodetype submit settings.
 */


/**
 * @inheritdoc
 */
class architecture_nodetype_submitted_views_handler extends views_handler_field {

  /**
  * Render the submit settings for this nodetype.
  */
  function render($values) {
    $value = $this->get_value($values);
    $submitted = '';
    if(!empty($value)) {
      $submitted = (variable_get('node_submitted_' . $value) > 0 ? t('Yes') : t('No'));
    }
    return $this->sanitize_value($submitted);
  }
}
