<?php

/**
 * @file
 * Display functions for course relationships.
 */

/**
 * Build FAPI form from child NIDs.
 */
function course_relationships_build_children_form($parent_node, $level = 0) {
  global $user;
  global $show_enroll_btn;
  ctools_include('dependent');
  $children = array(
    '#tree' => TRUE,
  );

  $course = course_get_course($parent_node);
  $nids = array();
  foreach ($course->getObjects() as $courseObject) {
    if ($courseObject->getModule() == 'course_relationships') {
      $nids[] = $courseObject->getInstanceId();
    }
  }
  $nids = array_filter($nids);

  // Construct parent checkbox - disable if appropiate.
  $parent['#title'] = l($parent_node->title, "node/$parent_node->nid");
  if ((!empty($parent_node->sell_price) && $parent_node->sell_price > 0) && $parent_node->course['relationships']['enroll_options'] != 'children') {
    // Activity set is for sale, and setup to allow purchase of this course.
    $parent['#title'] .= '<span class="price child-price"> - ' . uc_currency_format($parent_node->sell_price) . '</span>';
  }
  if (node_access('update', $parent_node)) {
    $parent['#title'] .= ' ' . l('[edit]', "node/$parent_node->nid/edit");
  }
  $parent['#type'] = 'checkbox';

  if (!$user->uid) {
    $parent['#type'] = 'markup';
    $parent['#markup'] = $parent['#title'];
  }

  $results = course_enroll_access($parent_node, $user, FALSE, TRUE);
  if (!empty($results['course_must_purchase'])) {
    unset($results['course_must_purchase']);
  }
  if (!empty($results)) {
    $result = reset($results);
  }
  else {
    $result['success'] = TRUE;
  }

  if (!$result['success']) {
    $parent['#disabled'] = TRUE;
  }

  if ($parent_node->course['relationships']['enroll_options'] == 'parent' && $level == 0) {
    // Parent only should disable the topmost checkbox. Check result before
    // prefilling.
    $parent['#default_value'] = $result['success'] ? $parent_node->nid : NULL;
    $parent['#disabled'] = TRUE;
  }

  if ($parent_node->course['relationships']['enroll_options'] != 'children') {
    $parent['#return_value'] = $parent_node->nid;

    if ($type = course_relationships_user_has_product($parent_node->nid)) {
      // Course enrolled or in cart, disable and check by default.
      $parent['#disabled'] = TRUE;
      $parent['#default_value'] = $parent_node->nid;
      $record = course_report_load($parent_node, $user);
      $parent['#description'] = $type == 'Cart' ? '(' . l('in cart', 'cart') . ')' : "($record->section_name)";
    }
  }
  else {
    $parent['#type'] = 'markup';
    $parent['#markup'] = '<div class="form-item">' . $parent['#title'] . '</div>';
  }

  $parent_blockers = course_enroll_access($parent_node, NULL, FALSE, TRUE);
  $parent_block = FALSE;
  if (!empty($parent_blockers)) {
    foreach ($parent_blockers as $blocktype => $parent_blocker) {
      if (!in_array($blocktype, array('course_must_purchase', 'course_noanon')) && !$parent_blocker['success']) {
        $parent_block = TRUE;
        break;
      }
      if (!course_enrollment_check($parent_node->nid, $user->uid) && !course_relationships_user_has_product($parent_node->nid) && ($parent_blocker['success'] || ($blocktype == 'course_must_purchase' && count($parent_blockers) < 2))) {
        // course has not already been purchased/enrolled in and is not in their cart
        // and is either enrollable or the only blocker is needing to purchase it
        $show_enroll_btn = TRUE;
      }
    }
  }
  elseif (!course_enrollment_check($parent_node->nid, $user->uid)) {
    // course has not already been enrolled in
    $show_enroll_btn = TRUE;
  }

  if ($parent_block) {
    $parent['#title'] = t('!image !title<br/>!header<br/>!message', array('!image' => theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()), array()), '!header' => $parent_blocker['header'], '!title' => $parent['#title'], '!message' => $parent_blocker['message']));
  }

  $children['parent'] = $parent;

  $children['activities']['#type'] = 'fieldset';

  foreach ($nids as $nid) {
    // Process each child.
    $node = node_load($nid);

    if (course_enrollment_check($node->nid, $user->uid)) {
      // User is enrolled so check the take course blockers.
      $blockers = course_take_course_access($node, $user, FALSE, TRUE);
    }
    else {
      // User not enrolled yet.
      $blockers = course_enroll_access($node, $user, FALSE, TRUE);
    }
    $block = FALSE;
    if (!empty($blockers)) {
      foreach ($blockers as $blocktype => $blocker) {
        if (!in_array($blocktype, array('course_must_purchase', 'course_noanon')) && !$blocker['success']) {
          $block = TRUE;
          break;
        }
        if (!course_relationships_user_has_product($nid) && ($blocker['success'] || ($blocktype == 'course_must_purchase' && count($blockers) < 2))) {
          // course has not already been purchased/enrolled in and is not in their cart
          // and is either enrollable or the only blocker is needing to purchase it
          $show_enroll_btn = TRUE;
        }
      }
    }
    elseif (!course_enrollment_check($nid, $user->uid)) {
      // course has not already been enrolled in
      $show_enroll_btn = TRUE;
    }

    $title = l($node->title, "node/$node->nid");

    $child = array();
    $child['#title'] = t("!title", array('!title' => $title));
    if ((!empty($node->sell_price) && $node->sell_price > 0) && $parent_node->course['relationships']['enroll_options'] != 'parent') {
      // Course is for sale, and activity set is setup to allow purchase of this course.
      $title .= '<span class="price child-price"> - ' . uc_currency_format($node->sell_price) . '</span>';
    }

    if (node_access('update', $node)) {
      // Admin has access to update. Add a link to edit.
      $title .= ' ' . l('[edit]', "node/$node->nid/edit");
    }

    if ($block) {
      $title = t('!image !title<br/>!header<br/>!message', array('!image' => theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()), array()), '!header' => $blocker['header'], '!title' => $title, '!message' => $blocker['message']));
    }

    $child['#title'] = $title;


    if (course_relationships_is_parent($node)) {
      // The child is an activity set.
      $level++;
      $child = array_merge($child, course_relationships_build_children_form($node, $level));
    }
    else {
      // Child is a course. Build a checkbox.
      $child['#type'] = 'checkbox';

      if (!$user->uid) {
        // Not logged in.
        $child['#disabled'] = TRUE;
        $child['#return_value'] = NULL;
      }

      $type = course_relationships_user_has_product($node->nid);

      if ($type) {
        // User already purchased this course, so disable the checkbox.
        $child['#disabled'] = TRUE;
        $child['#default_value'] = $type ? 1 : 0;
        $record = course_report_load($node, $user);
        $child['#description'] = $type == 'Cart' ? '(' . l('in cart', 'cart') . ')' : "($record->section_name)";
      }

      if ($block) {
        // User is blocked or can only purchase parent, so we don't want them selecting children.
        $child['#disabled'] = TRUE;
        $child['#return_value'] = NULL;
      }
      else {
        $child['#return_value'] = $node->nid;
      }

      // Ensure user has view access to the child node.
      $child['#access'] = node_access('view', $node);
    }
    $children['activities'][$node->nid] = $child;
  }

  return $children;
}

/**
 * Linked form wrapper. Calls the recursive form building function that will
 * loop over activity set children.
 *
 * @param array $form_state
 *   Form state passed by drupal_get_form().
 * @param int $node
 *   Parent node object.
 * @param array $children
 *   Node IDs belonging to the parent.
 */
function course_relationships_build_form($form, &$form_state, $parent_nid) {
  $node = node_load($parent_nid);
  global $show_enroll_btn;
  $show_enroll_btn = FALSE;
  $form = course_relationships_build_children_form($node);

  $form['#node'] = $node;

  $result = course_enroll_access($node);

  if ($node->course['relationships']['enroll_options'] == 'parent' && !$result['success']) {
    $form['submit'] = array(
      '#markup' => $result['message'],
    );
  }
  elseif ($show_enroll_btn) {
    global $user;
    if (user_is_logged_in() && !course_enrollment_load($node->nid, $user->uid)) {
      $instances = field_info_instances('course_enrollment', $node->course['enrollment_type']);
      foreach ($instances as $field_name => $instance) {
        if ($instance['settings']['course_enrollment_user_field']) {
          // At least one field must be shown when enrolling. Display the user
          // enrollment form.
          $values = array(
            'nid' => $node->nid,
            'uid' => $user->uid,
            'type' => $node->course['enrollment_type'],
          );
          $enrollment = entity_create('course_enrollment', $values);
          $form['course_enrollment'] = array();
          $form['course_enrollment']['#tree'] = TRUE;
          $form['course_enrollment']['#parents'] = array('course_enrollment');
          field_attach_form('course_enrollment', $enrollment, $form['course_enrollment'], $form_state);
        }
      }
    }

    // Show our enrollment button when the user has the option of not only
    // purchasing the parent.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Enroll'),
      '#access' => user_is_logged_in(),
    );
  }

  drupal_add_js(drupal_get_path('module', 'course_relationships') . '/course_relationships.js');

  $form['#theme'] = 'course_relationships_build_form';

  return $form;
}

/**
 * @param $values a parent
 */
function course_relationships_recursive_add_to_cart(&$values) {
  global $user;
  static $ret = array();
  $delete_children = FALSE;
  $added = 0;

  $context = $values;
  if (!empty($values['parent'])) {
    $product = node_load($values['parent']);
    drupal_alter('course_relationships_add_to_cart_context', $context);
    $ret[$product->nid] = course_relationships_bulk_add_to_cart($product, $context);
    if ($product->course['relationships']['auto_enroll'] && !course_relationships_user_has_product($product->nid)) {
      $delete_children = TRUE;
    }
    $added++;
    // add parent to cart, continue
  }
  if (is_array($values['activities'])) {
    if ($delete_children) {
      // Don't add any children, also delete them all.
      if (module_exists('uc_cart')) {
        db_delete('uc_cart_products')
          ->condition('nid', array_keys($values['activities']))
          ->condition('cart_id', $user->uid)
          ->execute();
        drupal_set_message('Items have been removed from your cart because you are purchasing a set.', 'warning', FALSE);
      }
    }
    else {
      // Process adding children to the cart.
      $activities = array_filter($values['activities']);
      foreach ($activities as $key => $item) {
        if (!is_array($item) && !course_relationships_user_has_product($item)) {
          if ($key == $item) {
            //add course to cart, end here
            $product = node_load($item);
            drupal_alter('course_relationships_add_to_cart_context', $context);
            $ret[$product->nid] = course_relationships_bulk_add_to_cart($product, $context);
            $added++;
          }
        }
        else {
          // this is an activity set , keep going
          course_relationships_recursive_add_to_cart($item);
        }
      }
    }
  }

  return $ret;
}

/**
 * Submit handler for bulk add form.
 */
function course_relationships_build_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $ret = course_relationships_recursive_add_to_cart($values);

  if (count($ret) == 0) {
    drupal_set_message(t('Please select the items you want to register for.'), 'error');
  }

  $all_failed = count(array_filter($ret)) == 0;

  if ($all_failed || (count($ret) && count(array_keys($ret, 'enroll')) == count($ret))) {
    drupal_goto("node/" . $form['#node']->nid);
  }

  if (count($ret) > 0) {
    drupal_goto('cart');
  }
}

/**
 * Implements hook_field_extra_fields().
 */
function course_relationships_field_extra_fields() {
  $extra = array();
  foreach (entity_get_info() as $entity_type => $entity_info) {
    foreach (array_keys($entity_info['bundles']) as $bundle) {
      if (in_array($bundle, course_get_types())) {
        $extra[$entity_type][$bundle]['form']['course_relationships'] = array(
          'label' => t('Course relationships'),
          'description' => t('Course relationship form elements.'),
          'weight' => 0,
        );
        $extra[$entity_type][$bundle]['display']['course_relationships'] = array(
          'label' => t('Course relationships tree'),
          'weight' => 0,
        );
      }
    }
  }
  return $extra;
}

/**
 * Theme the course relationships nested form.
 */
function theme_course_relationships_build_form($variables) {

}
