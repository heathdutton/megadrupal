<?php

/**
 * @file
 * Views handler override for field that exposes nodetype options.
 */


/**
 * @inheritdoc
 */
class architecture_nodetype_options_views_handler extends views_handler_field {

  /**
  * Render the options settings for this nodetype.
  */
  function render($values) {
    $value = $this->get_value($values);
    $available_options = array(
      'status' => t('Published'),
      'promote' => t('Promoted to front page'),
      'sticky' => t('Sticky at top of lists'),
      'revision' => t('Create new revision'),
    );

    $options_list = '';
    if(!empty($value)) {
      $options = variable_get('node_options_' . $value);
      $count = 0;
      if (!empty($options)) {
        foreach ($options as $option) {
          $options_list .= $available_options[$option];
          $count++;
          if ($count < (count($options))) {
            $options_list .= ', ';
          }
        }
      }
      foreach($available_options as $key => $option) {
        if (!empty($options) &&(!in_array($key, $options))) {
          if (($options_list[strlen($options_list)-1] !== ',') && !empty($options_list)) {
            $options_list .= ', ';
          }
          switch ($key) {
            case 'status':
              $options_list .= 'Not published';
              break;
            case 'promote':
              $options_list .= 'Not promoted';
              break;
            case 'sticky':
              $options_list .= 'Not stickied';
              break;
            case 'revision':
              $options_list .= 'No revisions';
              break;
          }
        }
      }
    }
    if (!empty($options_list)) {
      $options_list .= '.';
    }
    return $this->sanitize_value($options_list);
  }
}
