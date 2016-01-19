<?php

/**
 * @file
 * Default implementation of the review pane template.
 *
 * Available variables:
 * - $panes: An array of checkout panes containing title and data.
 *
 * Helper variables:
 * - $form: The complete checkout review form array.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<div class="<?php print $classes;?>">
  <?php
  foreach ($panes as $pane_id => $pane) {
    if (is_array($pane['data'])) {
      $output = '';
      foreach ($pane['data'] as $key => $value) {
        $output .= '<div class="pane-data-key">' . $key .'</div>';
        $output .= '<div class="pane-data-value">'. $value .'</div>';
      }
    }
    else {
      $output = $pane['data'];
    }
    echo theme('fieldset', array('element' => array(
      '#title' => $pane['title'],
      '#children' => $output,
      '#collapsible' => false,
      '#attributes' => array('class' => array('pane', $pane_id)),
    )));
  }
  ?>
</div>