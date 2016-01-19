<?php

/**
 * @file
 * Custom Rules actions for Course Planner.
 *
 * Note that these actions are not intended for general use, but for some
 * scripts used by this module only. Sorry. (For most actions it would be
 * possible to use the 'bundle' property to specify that it only works for
 * selected content types, but then you would have to add conditions to all the
 * Rules components to verify that the parameters fit. Since these actions are
 * not ment for general use, the custom-coded verification seemed more
 * appropriate.)
 */

/**
 * Implements hook_rules_action_info().
 */
function courseplanner_rules_action_info() {
  $actions = array(
    'cp_add_sections' => array(
      'label' => t('Create new sections for a course outline'),
      'group' => t('Course planner'),
      'parameter' => array(
        'outline' => array(
          'type' => 'node',
          'label' => t('Course outline'),
          'save' => TRUE,
        ),
        'sections_info' => array(
          'type' => 'list<text>',
          'label' => t('Sections and their sizes'),
          'description' => t('Add the name of sections, one per line. If you want a size larger than one, add size separated by a comma (eg. "My section, 5").'),
        ),
      ),
    ),
    'cp_quick_add_embedded_resource' => array(
      'label' => t('Quick-add embedded resource'),
      'group' => t('Course planner'),
      'parameter' => array(
        'section' => array(
          'type' => 'node',
          'label' => t('Section'),
          'save' => TRUE,
        ),
        'contextual_description' => array(
          'type' => 'text',
          'label' => t('Resource description in this section'),
          'description' => t('A description of this resource, that makes sense in the section where it is used. (For example "do these exercises first".)'),
        ),
        'reference' => array(
          'type' => 'text',
          'label' => t('Reference'),
          'description' => t('A link or a book reference'),
        ),
        'title' => array(
          'type' => 'text',
          'label' => t('Title understandable in a general context'),
          'description' => t('For example "beginner exercises for completing the square".'),
        ),
      ),
    ),
    'cp_repeat_lesson' => array(
      'label' => t('Repeat lesson'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lesson' => array(
          'type' => 'field_collection_item',
          'label' => t('Lesson'),
        ),
        'end_date' => array(
          'type' => 'date',
          'label' => t('Last date'),
        ),
      ),
    ),
    'cp_copy_lesson_to_weeks' => array(
      'label' => t('Copy lesson to weeks'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lessons' => array(
          'type' => 'field_collection_item',
          'label' => t('Lesson'),
        ),
        'weeks' => array(
          'type' => 'list<text>',
          'label' => t('Weeks'),
        ),
      ),
    ),
    'cp_fill_lessons' => array(
      'label' => t('Fill lessons with course outline'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lessons' => array(
          'type' => 'list<field_collection_item>',
          'label' => t('List of lessons'),
        ),
        'outline' => array(
          'type' => 'node',
          'label' => t('Course outline'),
        ),
      ),
    ),
    'cp_fill_lessons_with_section' => array(
      'label' => t('Fill lessons with a section'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lessons' => array(
          'type' => 'list<field_collection_item>',
          'label' => t('List of lessons'),
        ),
        'section' => array(
          'type' => 'node',
          'label' => t('Section'),
        ),
      ),
    ),
    'cp_shift_lessons' => array(
      'label' => t('Push lesson content down'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lessons' => array(
          'type' => 'list<field_collection_item>',
          'label' => t('Lessons'),
        ),
        'steps' => array(
          'type' => 'integer',
          'label' => t('Number of steps'),
          'description' => t('Enter a negative number to shift lessons up.'),
        ),
      ),
    ),
    'cp_lessons_import' => array(
      'label' => t('Import lessons'),
      'group' => t('Course planner'),
      'parameter' => array(
        'offering' => array(
          'type' => 'node',
          'label' => t('Course offering'),
        ),
        'dates' => array(
          'type' => 'list<text>',
          'label' => t('Start dates'),
          'description' => t('Enter one start date/time per line. All formats parsable by strtotime() are accepted.'),
        ),
      ),
    ),
    'cp_lessons_delete' => array(
      'label' => t('Delete lessons'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lessons' => array(
          'type' => 'list<field_collection_item>',
          'label' => t('Lessons'),
        ),
      ),
    ),
    'cp_create_outline' => array(
      'label' => t('Create outline from offering'),
      'group' => t('Course planner'),
      'parameter' => array(
        'lessons' => array(
          'type' => 'list<field_collection_item>',
          'label' => t('Lessons'),
        ),
        'title' => array(
          'type' => 'text',
          'label' => t('Outline title'),
        ),
        'make_owner' => array(
          'type' => 'boolean',
          'label' => t('Make copies of any course sections I do now already own'),
        ),
      ),
    ),
  );
  return $actions;
}

/**
 * Helper function verifying that the field collections in a list are lessons.
 *
 * Any items that aren't of the wanted type will be removed from the list. If
 * any items are removed, a warning message will be displayed to the user. The
 * resulting nodes are processed into a non-associative arrary, meaning that the
 * keys start on 0 and go up from there. No key-by-id.
 *
 * @param $lesson_list
 *   A list of field collection items, as passed from Rules.
 */
function courseplanner_verify_lesson_list(array &$lesson_list) {
  // Clean the list of any items that aren't actually lessons. This also builds
  // the array of lessons we will use later, where the keys are serials.
  $errors = 0;
  foreach ($lesson_list as $key => $lesson) {
    if ($lesson->bundle() != 'cp_offering_lessons') {
      unset($lesson_list[$key]);
      $errors++;
    }
  }

  // Process the list so the keys are in order again. (Rules technically works
  // with lists, not arrays.)
  $lesson_list = array_values($lesson_list);

  if ($errors) {
    // Alert the user that there were incorrect objects in the list.
    drupal_set_message(t('@n items were ignored, since they are not valid lessons.', array('@n' => $errors)), 'warning');
  }
}

/**
 * Creates a new lesson, based on an existing one but with another date.
 *
 * @param $lesson
 *   The lesson entity (field collection) to clone from.
 * @param $date
 *   A time stamp to use for as start time for the cloned lesson.
 */
function courseplanner_clone_lesson($lesson, $date) {
  // Populate some starting data for the lesson.
  $values['cp_lesson_date'][LANGUAGE_NONE][0]['value'] = $date;
  if (isset($lesson->cp_lesson_date[LANGUAGE_NONE][0]['value2'])) {
    $diff = $lesson->cp_lesson_date[LANGUAGE_NONE][0]['value2'] - $lesson->cp_lesson_date[LANGUAGE_NONE][0]['value'];
    $values['cp_lesson_date'][LANGUAGE_NONE][0]['value2'] = $date + $diff;
  }
  $values['cp_lesson_room'] = $lesson->cp_lesson_room;
  $values['field_name'] = 'cp_offering_lessons';

  $new_lesson = entity_create('field_collection_item', $values);

  $new_lesson->setHostEntity('node', $lesson->hostEntity());
  $new_lesson->save();
}

/**
 * Search function used to load a course section based on its name.
 *
 * The function will first try to find a matching section created by $account_id
 * and if none is found, try to find section with matching name. If multiple are
 * found, the one with lowest ID is returned.
 *
 * @param $title
 *   A string with the title to search for.
 * @param (optional) $account_id
 *   Any user ID to use as author. If empty, the acting user will be used.
 * @return
 *   Either the ID for the first found section, or FALSE if none is found.
 */
function courseplanner_get_section_by_title($title, $account_id = NULL) {
  // Get the default user ID, if necessary.
  if (is_null($account_id)) {
    global $user;
    $account_id = $user->uid;
  }

  // Try finding a matching section created by the given user.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'cp_section')
    ->propertyCondition('title', $title)
    ->propertyCondition('uid', $user->uid)
    ->range(0, 1)
    ->execute();
  // If we have any results, use the first one as the section.
  if (isset($query->ordered_results)) {
    return $query->ordered_results[0]->entity_id;
  }

  // If no results, check if there is a section with this name created by
  // someone else.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'cp_section')
    ->propertyCondition('title', $title)
    ->range(0, 1)
    ->execute();
  if (isset($query->ordered_results)) {
    return $query->ordered_results[0]->entity_id;
  }

  // If no hits at all, return FALSE.
  return FALSE;
}

/**
 * Action callback, creating sections and adding them to a course outline.
 *
 * @param $outline
 *   A node object for a course outline.
 * @param $sections_info
 *   An array of strings, containing titles for the new sections. Each title may
 *   be appended with a size, like so: "My section, 5".
 */
function cp_add_sections($outline, $sections_info) {
  // Verify that the course outline is actually a course outline.
  if ($outline->type != 'cp_outline') {
    drupal_set_message(t('Sections can only be added to course outlines.'), 'warning');
    return;
  }

  foreach ($sections_info as $section_info) {
    // Build a title and a size, taking any trailing section size into
    // consideration.
    $pieces = explode(',', $section_info);
    if (ctype_digit(trim(end($pieces)))) {
      $size = trim(array_pop($pieces));
      $title = implode(',', $pieces);
    }
    else {
      $size = 1;
      $title = $section_info;
    }

    // Take care of the case of empty lines by just skipping them.
    if (!$title) {
      continue;
    }

    // If the line starts with an asterisk, we should try and fetch an existing
    // section with the given title.
    global $user;
    if (substr($title, 0, 1) == '*') {
      $title = trim(substr($title, 1));
      $section_nid = courseplanner_get_section_by_title($title);
    }
    // If not, we should create a new section.
    else {
      // Create a new section, so we can rerence it from the outline.
      $section = entity_create('node', array('type' => 'cp_section', 'title' => $title, 'uid' => $user->uid, 'cp_canonical_course' => $outline->cp_canonical_course));
      entity_save('node', $section);
      $section_nid = $section->nid;
    }

    // Add the section, and its size, to the field collection in the outline.
    // Thanks to dale42 and for the guide at http://drupal.org/node/1477186
    // explaining how to add field collection items.
    $values = array(
      'field_name' => 'cp_sections',
      // @TODO: Check if multilingual sites will need language-specific settings.
      'cp_section' => array(LANGUAGE_NONE => array(array('target_id' => $section_nid))),
      'cp_size' => array(LANGUAGE_NONE => array(array('value' => $size))),
    );
    $entity = entity_create('field_collection_item', $values);
    $entity->setHostEntity('node', $outline);
    $entity->save();
  }

  if (isset($section)) {
    drupal_set_message(t('Sections added. You may add section descriptions by using the edit links on the course outline page.'));
  }
}

/**
 * Action callback for quickly creating and adding resources to a section.
 *
 * Before the resource is created, this action will also look for any existing
 * resource with the same reference.
 *
 * @param node $section
 *   The section node where the resource should be embedded.
 * @param string $contextual_description
 *   The description of the resource, as it should appear in the section.
 * @param string $reference
 *   Either a URL or a something that is assumed to be a book reference.
 * @param string $title
 *   The title of the new resource, as it should appear outside the context of
 *   the section.
 */
function cp_quick_add_embedded_resource($section, $contextual_description, $reference, $title) {
  // First: Run security checks on string parameters. (Note that checks on the
  // $reference is managed within the conditions below.)
  $title = check_plain($title);
  $contextual_description = check_plain($contextual_description);
  // Check if the reference is a link or not.
  if (filter_var(link_cleanup_url($reference), FILTER_VALIDATE_URL) == TRUE) {
    $reference = link_cleanup_url($reference);
    $reference = drupal_strip_dangerous_protocols($reference);
    $resource_type = 'link';
    $search_field = 'cp_resource_link';
    $search_column = 'url';
  }
  else {
    $reference = check_plain($reference);
    $resource_type = 'book';
    $search_field = 'cp_resource_reference';
    $search_column = 'value';
  }

  // Try finding a matching resource.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'cp_resource')
    ->fieldCondition($search_field, $search_column, $reference)
    ->range(0, 1)
    ->execute();
  // If we have any results, we use the first one as our resource.
  if (isset($query->ordered_results)) {
    $resource_id = $query->ordered_results[0]->entity_id;
    $new_reference_created = FALSE;
  }
  // If not, we create a new resource.
  else {
    global $user;
    $values = array(
      'type' => 'cp_resource',
      'uid' => $user->uid,
      'language' => $section->language,
      'cp_canonical_course' => $section->cp_canonical_course,
      'cp_keywords' => $section->cp_keywords,
      'title' => $title,
      'cp_resource_type' => array(LANGUAGE_NONE => array(array('value' => $resource_type))),
    );
    if ($resource_type == 'link') {
      $values['cp_resource_link'] = array(LANGUAGE_NONE => array(array('url' => $reference, 'title' => $title)));
    }
    elseif ($resource_type == 'book') {
      $values['cp_resource_reference'] = array(LANGUAGE_NONE => array(array('value' => $reference)));
    }
    $resource = entity_create('node', $values);
    entity_save('node', $resource);
    $resource_id = $resource->nid;
    $new_reference_created = TRUE;
  }

  // Finally, we create a field collection, embedding the resource in the
  // section.
  $values = array(
    'field_name' => 'cp_section_resources_embedded',
    // @TODO: Check if multilingual sites will need language-specific settings.
    'cp_list_promote' => array(LANGUAGE_NONE => array(array('value' => 1))),
    'cp_embedded_resource_description' => array(LANGUAGE_NONE => array(array('value' => $contextual_description))),
    'cp_resource' => array(LANGUAGE_NONE => array(array('target_id' => $resource_id))),
  );
  $entity = entity_create('field_collection_item', $values);
  $entity->setHostEntity('node', $section);
  $entity->save();

  if ($new_reference_created) {
    drupal_set_message(t('A new resource was created and embedded. You can edit the resource to set its source and give a richer description.'));
  }
  else {
    drupal_set_message(t('A matching resource was found. Please verify that it matches your expectations.'));
  }
}

/**
 * Action callback for creating repeated lessons.
 *
 * @param $lesson
 *   A field collection item object representing a lesson.
 * @param $end_date
 *   A timestamp representing the last date for the repetition.
 */
function cp_repeat_lesson($lesson, $end_date) {
  // Verify that $lesson is actually a lesson node.
  if ($lesson->bundle() != 'cp_offering_lessons') {
    drupal_set_message(t('%lesson is not a valid lesson, and cannot be repeated.', array('%lesson' => $lesson->title)), 'warning');
    return;
  }

  // Get start date and set repeat interval to one week.
  $date = $lesson->cp_lesson_date[LANGUAGE_NONE][0]['value'];
  $interval = 60 * 60 * 24* 7;

  $count = 0;
  while ($date <= ($end_date - $interval)) {
    $count++;
    // Add one week to start date and, if set, end date.
    $date += $interval;

    courseplanner_clone_lesson($lesson, $date);

    // Abort the repetition after 52+ times. Chances are that someone is
    // repeating lessons with a whacky date somewhere. (Not that it ever
    // happened to the module maintainer...)
    if ($count > 52) {
      drupal_set_message(t('Maximum lesson repetition is one year. (Last date was @date.)', array('@date' => date('Y-m-d', $date))), 'warning');
      return;
    }
  }
}

/**
 * Action callback for cp_copy_lessons_to_weeks().
 *
 * This action takes a list of lessons and a list of weeks, and copies each
 * lesson into each week.
 *
 * @param $lesson
 *   A field collection item of the type cp_offering_lessons.
 * @param $weeks
 *   A list of strings representing weeks, provided on the form "2013 30".
 */
function cp_copy_lesson_to_weeks($lesson, $weeks) {
  if ($lesson->bundle() != 'cp_offering_lessons') {
    drupal_set_message(t('Invalid lesson. Skipping.'), 'warning');
    return;
  }

  $year_now = date('Y');
  $week_now = date('W');
  foreach ($weeks as $week) {
    // Parse the week input to a year and a week.
    $week = explode(' ', $week);
    // If only week is set, assume that it is now or closest future week with
    // that number.
    if (count($week) == 1) {
      $week = $week[0];
      $year = $year_now + ($week_now > $week);
    }
    else {
      $year = $week[0];
      $week = $week[1];
    }
    if ($year < 1950 || $week > 52 || $week < 1) {
      drupal_set_message(t('Cannot parse "@year @week", sorry. Please enter year and week number separated with space, on on each line', array('@year' => $year, '@week' => $week)), 'warning');
      continue;
    }
    // The week number must be two-digit to be understood by strtotime.
    $week = (string) $week;
    if (strlen($week) == 1) {
      $week = "0$week";
    }

    $start_time = date('H:i +N', $lesson->cp_lesson_date[LANGUAGE_NONE][0]['value']);
    $date = strtotime($year . 'W' . "$week $start_time days -1 day");
    courseplanner_clone_lesson($lesson, $date);
  }
}

/**
 * Action callback. Points a list of lessons to the sections in an outline.
 *
 * This function will read the sections in the course outline, and their sizes,
 * and update the list of lessons so that they refer to the sections. If the
 * first section in the outline has size 3, the three first lessons will point
 * to that section before moving on to the next section in the outline.
 *
 * @param $lessons
 *   An array with field collection items of type cp_offering_lessons.
 * @param $outline
 *   A node object representing a course outline.
 */
function cp_fill_lessons($lessons, $outline) {
  // Verify that the course outline is actually a course outline.
  if ($outline->type != 'cp_outline') {
    drupal_set_message(t('@outline is not a valid course outline.', array('@outline' => $outline->title)), 'warning');
    return;
  }

  // Verify the list of lessons.
  courseplanner_verify_lesson_list($lessons);
  // If no valid lesson is found, display an error message and quit.
  if (count($lessons) == 0) {
    drupal_set_message(t('None of the selected items are valid lessons, sorry.'), 'warning');
    return;
  }

  // Get the first valid lesson in the list.
  $lesson = reset($lessons);

  // Get the course offering for the first lesson. We assume that all lessons
  // belong to the same course offering (which should really be the case).
  $offering = $lesson->hostEntity();

  // Build an array with section data, to use with the lessons.
//  $i = 0;
//  foreach ($outline->cp_sections[LANGUAGE_NONE] as $section) {
//    $section_info = field_collection_field_get_entity($section);
//    $section_title = entity_label('node', entity_load_single('node', $section_info->cp_section[LANGUAGE_NONE][0]['target_id']));
//    for ($repeat = 1; $repeat <= $section_info->cp_size[LANGUAGE_NONE][0]['value']; $repeat++) {
//      $section_data[$i]['id'] = $section_info->cp_section[LANGUAGE_NONE][0]['target_id'];
//      if ($section_info->cp_size[LANGUAGE_NONE][0]['value'] > 1) {
//        $section_data[$i]['lesson_title'] = t('@section-title (@repeat of @size)', array('@section-title' => $section_title, '@repeat' => $repeat, '@size' => $section_info->cp_size[LANGUAGE_NONE][0]['value']));
//      }
//      else {
//        $section_data[$i]['lesson_title'] = $section_title;
//      }
//      $i++;
//    }
//  }

  // Get a flat list with all the sections in this outline, and any of its sub-
  // outlines.
  $section_array = cp_get_sections_and_sizes($outline);

  // Walk through all the lessons or all the sections, whichever is the least,
  // and update the lessons.
  for ($i = 0; $i < min(array(count($lessons), count($section_array))); $i++) {
    $lessons[$i]->cp_lesson_section[LANGUAGE_NONE][0]['target_id'] = $section_array[$i]['nid'];
    $lessons[$i]->cp_lesson_title[LANGUAGE_NONE][0]['value'] = $section_array[$i]['title'];
    $lessons[$i]->save();
  }

  // Update the course offering, to point to the new outline.
  $offering->cp_offering_outline[LANGUAGE_NONE][0]['target_id'] = $outline->nid;
  entity_save('node', $offering);

  if (count($section_array) > count($lessons)) {
    drupal_set_message(t('Updated @lessons lessons. @diff lesson(s) were missing to fit the whole outline.', array('@lessons' => count($lessons), '@diff' => count($section_array) - count($lessons))));
  }
  else {
    drupal_set_message(t('Added all @lessons lessons in the outline. @diff lesson(s) overshooting lessons were left untouched.', array('@lessons' => count($section_array), '@diff' => count($lessons) - count($section_array))));
  }
}

/**
 * Builds a list of sections in an outline, including those in sub-outlines.
 *
 * This function calls itself recursively in order to build a flat list of all
 * sections included in an outline, and any of its sub-outlines. There is a
 * crude check for infinite recursion. Sections will be repeated the number of
 * times indicated by the sizes in the outlines.
 * This is a helper function, called by action handlers.
 *
 * @param $outline
 *   A course outline node.
 * @return
 *   An array with the following keys/values:
 *     - nid: The node ID of the section.
 *     - title: The title of the section, possibly with '(REPEAT of TOTAL)' too.
 */
function cp_get_sections_and_sizes($outline) {
  // We use a static variable to keep track of which outlines have been
  // processed already. We don't want circular references or infinite recursion.
  // (Note that this check is crude -- it could give false-positives.)
  static $processed_outlines;
  if (isset($processed_outlines[$outline->nid])) {
    drupal_set_message(t('@outline appears more than once. Skipping all but the first, to avoid circular references.', array('@outline' => $outline->title)), 'warning');
    return array();
  }
  $processed_outlines[$outline->nid] = TRUE;

  // Verify that the course outline is actually a course outline.
  if ($outline->type != 'cp_outline') {
    drupal_set_message(t('@outline is not a valid course outline.', array('@outline' => $outline->title)), 'warning');
    return array();
  }

  $section_array = array();

  // Loop through each section and add either add it to the array of sections,
  // or -- if the entry is an outline -- call this function and get a flat list
  // of its content to add all at once. Recursion for the win.
  foreach ($outline->cp_sections[LANGUAGE_NONE] as $section) {
    $section_info = field_collection_field_get_entity($section);
    $section = entity_load_single('node', $section_info->cp_section[LANGUAGE_NONE][0]['target_id']);
    if ($section->type == 'cp_section') {
      // This loop is to repeat all sections that have a greater size than one.
      // Those will be repeated, with the lesson titles
      // 'SECTION NAME (REPEAT of TOTAL)'. Sorry for the somewhat lengthy
      // expression.
      for ($repeat = 1; $repeat <= $section_info->cp_size[LANGUAGE_NONE][0]['value']; $repeat++) {
        $section_array[] = array(
          'nid' => $section->nid,
          'title' => ($section_info->cp_size[LANGUAGE_NONE][0]['value'] == 1) ? entity_label('node', $section) : t('@section-title (@repeat of @size)', array('@section-title' => entity_label('node', $section), '@repeat' => $repeat, '@size' => $section_info->cp_size[LANGUAGE_NONE][0]['value'])),
        );
      }
    }
    else {
      // If the section actually isn't a section, we assume that it is an
      // outline. Call this function and get its content in a flat list.
      $section_array = array_merge($section_array, cp_get_sections_and_sizes($section));
    }
  }
  return $section_array;
}

/**
 * Action callback. Points a list of lessons to a single section.
 *
 * This function updates all the lessons to point to the given section, and also
 * updates the lesson titles to fit the section title.
 *
 * @param $lessons
 *   An array with field collection items of type cp_offering_lessons.
 * @param $section
 *   A node object representing a course section.
 */
function cp_fill_lessons_with_section($lessons, $section) {
  // Verify that the course section is actually a course section.
  if ($section->type != 'cp_section') {
    drupal_set_message(t('@section is not a valid course section.', array('@section' => $section->title)), 'warning');
    return;
  }

  // Verify the list of lessons.
  courseplanner_verify_lesson_list($lessons);
  // If no valid lesson is found, display an error message and quit.
  if (count($lessons) == 0) {
    drupal_set_message(t('None of the selected items are valid lessons, sorry.'), 'warning');
    return;
  }

  foreach ($lessons as $i => $lesson) {
    $lesson->cp_lesson_section[LANGUAGE_NONE][0]['target_id'] = $section->nid;
    if (count($lessons) > 1) {
      $lesson->cp_lesson_title[LANGUAGE_NONE][0]['value'] = t('@section-title (@repeat of @size)', array('@section-title' => $section->title, '@repeat' => $i + 1, '@size' => count($lessons)));
    }
    else {
      $lesson->cp_lesson_title[LANGUAGE_NONE][0]['value'] = $section->title;
    }
    $lessons[$i]->save();
  }
}

/**
 * Action callback for moving lesson content further down in a course outline.
 *
 * This function inserts a number of empty lessons on top of a list, shifts the
 * dates of the existing lessons donw, and cuts off the overshooting lessons
 * from the end. This function can also be used to shift lesson content up.
 *
 * @param $lessons
 *   An array with lesson field collection items.
 * @param $steps
 *   The number of steps to push lessons down the list. If a negative number is
 *   used, the lesson content will be moved up instead.
 */
function cp_shift_lessons($lessons, $steps) {
  // Make sure that the lessons are actually lessons.
  courseplanner_verify_lesson_list($lessons);

  // Check if we should shift lessons up or down.
  $steps = (int) $steps;
  if ($steps > 0) {
    $mode = 'down';
    // The lessons are processed from last to first.
    $lessons = array_reverse($lessons);
  }
  if ($steps < 0) {
    // Tricky trick: If we are to move the lessons up the list, DON'T flip the
    // list and move them down instead.
    $mode = 'up';
    $steps = -$steps;
  }

  // If the number of steps isn't well defined, alert the user and quit.
  if (!isset($mode)) {
    drupal_set_message(t('@steps is not a valid number of steps to shift the lessons. (Must be a non-zero integer.)', array('@steps' => $steps)), 'warning');
    return;
  }

  // If we are asked to shift the list so much that all lessons will be pushed
  // out, display a warning and quit.
  if ($steps >= count($lessons)) {
    drupal_set_message(t('The list of lessons is to small to shift @steps step(s).', array('@steps' => $steps)), 'warning');
    return;
  }

  $transferrable_properties = array(
    'cp_lesson_title',
    'cp_lesson_section',
    'cp_lesson_notes',
    'cp_lesson_status',
  );
  foreach ($lessons as $i => $lesson) {
    if (isset($lessons[$i + $steps])) {
      foreach($transferrable_properties as $property) {
        $lesson->{$property} = $lessons[$i + $steps]->{$property};
      }
    }
    else {
      foreach($transferrable_properties as $property) {
        $lesson->{$property} = array();
      }
    }
    $lesson->save();
  }
  return;
}

/**
 * Adds a number of lessons to a course offering, based on a text import.
 *
 * @param $offering
 *   The course offering the lessons should be attached to.
 * @param $dates
 *   A list of dates, represented by strings.
 */
function cp_lessons_import($offering, $dates) {
  // Verify that $offering is actually a course offering.
  if ($offering->type != 'cp_offering') {
    drupal_set_message(t('%offering is not a valid course offering.', array('%offering' => $offering->title)), 'warning');
    return;
  }

  global $user;
  $skips = 0;
  $title = '';

  foreach ($dates as $date) {
    // Skip blank lines.
    if (trim($date) == '') {
      $skips++;
      continue;
    }
    // Check if the line starts with a "#". If so, treat it as a title for the
    // next lesson.
    if (substr($date, 0, 1) == '#') {
      $title = trim(substr($date, 1));
      $skips++;
      continue;
    }

    // Convert the date into a unix timestamp, if necessary.
    $parts = explode(',', $date);
    if (!is_numeric($parts[0])) {
      $parts[0] = strtotime($parts[0]);
    }
    // If there still isn't a sensible timestamp, skip this row.
    if (!is_numeric($parts[0])) {
      drupal_set_message(t('Skipped date "@date": unable to parse.', array('@date' => $date)), 'warning');
      $skips++;
      continue;
    }

    // Check if we have an end date set. If it is relative, make it relative to
    // the start date.
    if (isset($parts[1]) && !is_numeric($parts[1])) {
      $parts[1] = strtotime($parts[1], $parts[0]);
    }
    if (isset($parts[1]) && !is_numeric($parts[1])) {
      unset($parts[1]);
    }

    // Populate some starting data for the lesson.
    $values['cp_lesson_date'][LANGUAGE_NONE][0]['value'] = $parts[0];
    if (isset($parts[1])) {
      $values['cp_lesson_date'][LANGUAGE_NONE][0]['value2'] = $parts[1];
    }
    $values['field_name'] = 'cp_offering_lessons';
    if ($title) {
      $values['cp_lesson_title'][LANGUAGE_NONE][0]['value'] = $title;
    }

    $entity = entity_create('field_collection_item', $values);
    $entity->setHostEntity('node', $offering);
    $entity->save();

    // Reset any title stored on the previous loop.
    $title = '';
  }

  drupal_set_message(t('Added @count empty lessons.', array('@count' => count($dates) - $skips)));
}

/**
 * Action callback for deleting lessons.
 *
 * This action is currently necessary since (1) field entries for the host
 * entity must be updated when the field collection items are saved, and (2) for
 * some reason accessing the host entity directly in a Rules component makes the
 * component unavailable for VBO.
 *
 * @param $lessons
 *   An array with field collection lessons to delete. They are assumed to all
 *   belong to the same course offering.
 */
function cp_lessons_delete($lessons) {
  $offering = reset($lessons)->hostEntity();
  $ids = array_keys($lessons);
  foreach ($offering->cp_offering_lessons[LANGUAGE_NONE] as $delta => $entry) {
    if (in_array($entry['value'], $ids)) {
      unset($offering->cp_offering_lessons[LANGUAGE_NONE][$delta]);
    }
  }
  entity_save('node', $offering);
  entity_delete_multiple('field_collection_item', $ids);
}

/**
 * Action callback, creating a new outline based on lessons (in an offering).
 *
 * @param type $lessons
 * @param type $title
 * @param type $make_owner
 */
function cp_create_outline($lessons, $title, $make_owner) {
  $offering = reset($lessons)->hostEntity();
  $sections = array();
  $section_mapping = array();
  global $user;

  // Loop through all the sections in all the lessons.
  foreach ($lessons as $lesson) {
    foreach ($lesson->cp_lesson_section[LANGUAGE_NONE] as $section) {

      // Check if this is a new section, in need of some processing.
      if (is_null($section_mapping[$section['target_id']])) {
        // Store mapping, 'section 123 is in place 5'.
        $section_mapping[$section['target_id']] = count($sections);

        // Check if we need to copy the section.
        if ($make_owner) {
          $loaded_section = array_shift(entity_load('node', array($section['target_id'])));
          if ($loaded_section->uid != $user->uid) {
            // Create a new section, with $user as owner.
            $values = array();
            $values_to_copy = array('type', 'title', 'language', 'body', 'cp_canonical_course', 'cp_keywords', 'cp_section_resources_embedded');
            foreach ($values_to_copy as $key) {
              $values[$key] = $loaded_section->{$key};
            }
            $values['uid'] = $user->uid;
            $new_section = entity_create('node', $values);
            // Take care of the embedded resources. (This function is actually
            // ment to be used by node_clone.)
            field_collection_clone_items('node', $new_section, 'cp_section_resources_embedded', $values['language']);
            entity_save('node', $new_section);
            $section['target_id'] = $new_section->nid;
          }
        }

        // Store a new section entry for the outline.
        array_push($sections, array(
            'nid' => $section['target_id'],
            'size' => 1,
        ));
      }
      // If the section has appeared before, we just tick up the size.
      else {
        $sections[$section_mapping[$section['target_id']]]['size']++;
      }
    }
  }

  // Create a new outline and populate it with the collected sections.
  $values = array(
    'type' => 'cp_outline',
    'title' => $title,
    'uid' => $user->uid,
    'language' => $offering->language,
    'cp_canonical_course' => $offering->cp_canonical_course,
    'cp_keywords' => $offering->cp_keywords,
  );
  $outline = entity_create('node', $values);
  entity_save('node', $outline);
  // Add all the sections, with proper size.
  $values = array('field_name' => 'cp_sections');
  foreach ($sections as $section) {
    // @TODO: Check if multilingual sites will need language-specific settings.
    $values['cp_section'] = array(LANGUAGE_NONE => array(array('target_id' => $section['nid'])));
    $values['cp_size'] = array(LANGUAGE_NONE => array(array('value' => $section['size'])));
    $entity = entity_create('field_collection_item', $values);
    $entity->setHostEntity('node', $outline);
    $entity->save();
  }

  // Display a link to the new outline.
  drupal_set_message(t('Created a new outline @outline containing @count sections. !link', array(
      '@outline' => $title,
      '@count' => count($sections),
      '!link' => l('View the outline.', 'node/' . $outline->nid)
    )));
}
