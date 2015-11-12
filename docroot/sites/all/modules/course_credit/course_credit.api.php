<?php

/**
 * @file
 * Hooks provided by Course credit module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to calculate the credit expiration.
 *
 * @param string $type
 *   Credit type from {course_credit_awarded}.type.
 *
 * @return integer
 *   A course credit expiration UNIX timestamp.
 *
 * @see course_credit_award_save()
 */
function hook_course_credit_calculate_expiration($type) {
  // Example: set the credit expiration for a type to 1 week from today.
  if ($type == 'some_type') {
    $expiration = strtotime("+1 week");
  }

  return $expiration;
}

/**
 * Notify modules course credit was awarded.
 *
 * @param array $record
 *   The course credit awarded record, including 'ccaid' Course credit awarded
 *   ID from drupal_write_record().
 *
 * @see course_credit_award_save()
 */
function hook_course_credit_awarded_insert($record) {
  // Example: send users an email when they are awarded credit.
  // TODO Convert "user_load" to "user_load_multiple" if "$record['uid']" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $record['uid']))
  $params['account'] = $account = user_load($record['uid']);
  $params['record'] = $record;
  $params['subject'] = t("You've got new credit");
  drupal_mail('my_module', 'credit_awarded', $account->mail, user_preferred_language($account), $params);
}

/**
 * Notify modules course credit was awarded.
 *
 * @param array $record
 *   The course credit awarded record, including 'ccaid' Course credit awarded
 *   ID from drupal_write_record().
 *
 * @see course_credit_award_save()
 */
function hook_course_credit_awarded_update($record) {
  // @see hook_course_credit_awarded_insert()
}

/**
 * Let modules alter the credit record before writing it to the database.
 *
 * @param array $record
 *   The course credit awarded record, including 'ccaid' Course credit awarded
 *   ID from drupal_write_record().
 *
 * @see course_credit_award_save()
 */
function hook_course_credit_awarded_presave(&$record) {
  // Add an hour to the credit claim date.
  $record->date += 3600;
  // Make the credit record inactive.
  $record->status = 0;
}

/**
 * Notify modules credit is about to be claimed.
 *
 * @param object $node
 *   The course node.
 */
function hook_course_credit_check_completion($node) {
  // @see course_restrict_credit_course_credit_check_completion
}

/**
 * Implements hook_default_course_credit_type().
 */
function hook_default_course_credit_type() {

}

/**
 * Alter the eligible types.
 *
 * @param array $etypes
 *   An array of eligible user credit type objects.
 * @param type $node
 *   The course node.
 * @param type $user
 *   The user taking the course.
 */
function hook_course_credit_user_credit_types_alter(&$etypes, $node, $user) {
  if ($today == 'tuesday') {
    // User is not eligible for the wednesday credit type on tuesday.
    unset($etypes['wednesday']);
  }
  if ($weather == 'raining') {
    // User gets double the max credit on rainy days.
    $etypes['weather']->max = $etypes['weather']->max * 2;
  }
}

/**
 * Provide eligbility mapping options.
 */
function hook_course_credit_map_options() {
  return array(
    'custom_maptype' => array(
      'title' => 'Map to some arbitrary condition',
      'description' => 'See above.',
      'mappers' => array(
        'some_field' => array(
          'title' => 'Field 1',
          'options' => array(
            'some_option1' => "Test mapper field 1 value 1",
            'some_option2' => "Test mapper field 1 value 2",
          ),
        ),
        'some_field2' => array(
          'title' => 'Field 2',
          'options' => array(
            'some_option3' => "Test mapper field 2 value 1",
            'some_option4' => "Test mapper field 2 value 2",
          ),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_course_credit_map().
 */
function hook_course_credit_map($node, $account, $mappings) {
  $enrollment = course_enrollment_load($node, $account);
  $entity = entity_load_single('some_entity', $enrollment->eid);
  foreach ((array) $mappings['some_entity'] as $field => $values) {
    foreach ($entity->{$field}[LANGUAGE_NONE] as $item) {
      if (in_array($item['value'], $values)) {
        return TRUE;
      }
    }
  }
}
