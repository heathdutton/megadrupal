<?php

/**
 * @file
 * Entity List Field pages.
 */

/**
 * AJAX callback for pager.
 */
function entity_list_field_pageturner_callback() {

  $list_class = $_POST['list_class'];

  // Is $list_class one of our allowed classes?
  $allowed_classes = variable_get('entity_lister_allowed_classes', NULL);
  if (!in_array($list_class, $allowed_classes)) {
    print '';
    drupal_exit();
  }

  // Though the delta does wind up in the output, we don't need check_plain
  // because casting as an int ensures that nothing malicious can sneak through.
  $delta = (int) $_POST['delta'];
  // This prefix becomes part of the output, so call check_plain().
  $id_prefix = check_plain($_POST['id_prefix']);

  $obj = new $list_class($_POST['config'], $delta, $id_prefix, $_POST['extra']['host_item']);
  $list = $obj->getList(urldecode($_POST['page']));
  $element = $obj->element($list);
  print drupal_render($element);

  drupal_exit();

}
