<?php
/**
 * @file
 * Container related theme functions.
 */


/**
 * Overrides theme_container().
 */
function strapped_container($variables) {

  // Grab the element for ease of use.
  $element = $variables['element'];

  if (!isset($element['#element_offset'])) {
    $element['#element_offset'] = 0;
  }

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {

    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }

    // Add classes.
    $element['#attributes']['class'][] = 'form-wrapper';
    //  $element['#attributes']['class'][] = 'form-group';
  }


  if (isset($element['#title_display']) && $element['#title_display'] == 'inline') {
    // @todo closing div??
    return '<div class="row">
          <div class="col-md-offset-' . $element['#element_offset'] . ' col-md-' . (12 - $element['#element_offset']) . '">
               <div' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>
          </div>
        </div>';
  }
  else {
    return '<div' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>';
  }


}


