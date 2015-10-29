<?php

/**
 * Override or insert variables into the block template.
 */
function pushbutton_preprocess_block(&$variables) {
  $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Override or insert variables into the node template.
 */
function pushbutton_preprocess_node(&$variables) {
  $variables['title_attributes_array']['class'][] = 'title';
}
