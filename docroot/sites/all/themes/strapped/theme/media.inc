<?php
function strapped_process_media_element($element) {


  // Add some button classes to the media elements browse button
  $element['browse_button']['#attributes']['class'][] = 'btn';
  $element['browse_button']['#attributes']['class'][] = 'btn-default';

  // Change the edit link into a button
  $element['edit']['#attributes']['class'][] = 'btn';
  $element['edit']['#attributes']['class'][] = 'btn-default';

  // If epsa crop is installed then make the link into a button
  if (isset($element['epsacrop'])) {
      $element['epsacrop']['#attributes']['class'][] = 'btn';
      $element['epsacrop']['#attributes']['class'][] = 'btn-default';
      $element['epsacrop']['#suffix'] = ' ';
  }

  return $element;

}