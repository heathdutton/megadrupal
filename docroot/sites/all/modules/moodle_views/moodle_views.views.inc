<?php

/**
 * @file
 * This file tells the Views module about the Moodle course tables.
 */

/**
 * Implements hook_views_data().
 */
function moodle_views_views_data() {
  $data = array();

  // ----- course table -----
  $data['course']['table']['group'] = t('Moodle Course');
  $data['course']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Moodle courses'),
    'help' => t('Course data.'),
    'database' => 'moodle');

  $data['course']['id'] = array(
    'title' => t('ID'),
    'help' => t('The unique internal ID for a course.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE),
    'filter' => array(
      'label' => 'Course ID',
      'handler' => 'views_handler_filter_numeric'),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['course']['fullname'] = array(
    'title' => t('Full name'),
    'help' => t('The full name of the course.'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_course',
      'click sortable' => TRUE),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'label' => t('Course full name')),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['course']['shortname'] = array(
    'title' => t('Short Name'),
    'help' => t('The short name of the course.'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_course',
      'click sortable' => TRUE),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'label' => t('Course short name')),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['course']['idnumber'] = array(
    'title' => t('ID number'),
    'help' => t('The ID number of the course.'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_course',
      'click sortable' => TRUE),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'label' => t('Course short name')),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['course']['summary'] = array(
    'title' => t('Summary'),
    'help' => t('The summary text of the course.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => 'full_html'));
  $data['course']['sortorder'] = array(
    'title' => t('Sort order'),
    'help' => t('The sort order of the course.'),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['course']['visible'] = array(
    'title' => t('Course is visible'),
    'help' => t('Whether the course is visible in Moodle or not'),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Course is visible'),
      'type' => 'yes-no'));
  $data['course']['coursedisplay'] = array(
    'title' => t('Display one section per page'),
    'help' => t('The display setting of the course (Show all sections on one page / Show one section per page.'),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Display one section per page'),
      'type' => 'yes-no'));
  $data['course']['startdate'] = array(
    'title' => t('Start date'),
    'help' => t('The date this course starts on.'),
    'field' => array(
      'handler' => 'views_handler_field_date'),
    'sort' => array(
      'handler' => 'views_handler_sort_date'),
    'filter' => array(
      'handler' => 'views_handler_filter_date'));
  $data['course']['timecreated'] = array(
    'title' => t('Time created'),
    'help' => t('The date and time this course was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date'),
    'sort' => array(
      'handler' => 'views_handler_sort_date'),
    'filter' => array(
      'handler' => 'views_handler_filter_date'));
  $data['course']['timemodified'] = array(
    'title' => t('Time modified'),
    'help' => t('The date and time this course was last modified.'),
    'field' => array(
      'handler' => 'views_handler_field_date'),
    'sort' => array(
      'handler' => 'views_handler_sort_date'),
    'filter' => array(
      'handler' => 'views_handler_filter_date'));

  // ----- course_sections table -----
  $data['course_sections']['table']['group'] = t('Moodle Section');

  $data['course_sections']['section'] = array(
    'title' => t('Number'),
    'help' => t('The number of the course section'),
    'field' => array(
      'handler' => 'views_handler_field_numeric'),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['course_sections']['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the course section'),
    'field' => array(
      'handler' => 'views_handler_field'));
  $data['course_sections']['summary'] = array(
    'title' => t('Summary'),
    'help' => t('The summary text of the course section'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => 'full_html'));

  // ----- course_modules table -----
  $data['course_modules']['table']['group'] = t('Moodle Course Module');

  $data['course_modules']['idnumber'] = array(
    'title' => t('ID number'),
    'help' => t('The ID number of the course module'),
    'field' => array(
      'handler' => 'views_handler_field'));

  // ----- modules table -----
  $data['modules']['table']['group'] = t('Moodle Modules');

  // ----- graded course alias -----
  $data['course_graded'] = $data['course'];
  unset($data['course_graded']['table']['base']);
  $data['course_graded']['table']['group'] = t('Moodle Grades');

  // ----- course category table -----
  $data['course_categories']['table']['group'] = t('Moodle Course');

  $data['course_categories']['name'] = array(
    'title' => t('Category'),
    'help' => t('The category name of the course.'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_category',
      'click sortable' => TRUE),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'label' => t('Member’s forenames')));

  $data['course_categories']['description'] = array(
    'title' => t('Category description'),
    'help' => t('The category description for the course.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => 'full_html'));

  $data['course_categories']['table']['join']['course'] = array(
    'left_field' => 'category',
    'field' => 'id');

  // ----- graded course category alias -----
  $data['course_categories_graded'] = $data['course_categories'];
  unset($data['course_categories_graded']['table']['base']);
  $data['course_categories_graded']['table']['group'] = t('Moodle Grades');

  // ----- user table -----
  $data['user']['table']['group'] = t('Moodle User');
  $data['user']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Moodle users'),
    'help' => t('User data.'),
    'database' => 'moodle');

  $data['user']['id'] = array(
    'title' => t('ID'),
    'help' => t('The unique internal ID for a user.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['username'] = array(
    'title' => t('Username'),
    'help' => t('The username for the user.'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_user',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'),
    'filter' => array(
      'handler' => 'views_handler_filter_string'),
    'argument' => array(
      'handler' => 'views_handler_argument_string'),
    'relationship' => array(
      'label' => 'Moodle teacher name',
      'database' => 'moodle',
      'base' => 'moodle_views_live.node',
      'base_field' => 'title'));
  $data['user']['firstname'] = array(
    'title' => t('First name'),
    'help' => t('The first name of the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['lastname'] = array(
    'title' => t('Last name'),
    'help' => t('The last name of the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['idnumber'] = array(
    'title' => t('ID number'),
    'help' => t("The idnumber of the user. Can be the matching account's UID in Drupal."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'label' => t('ID number')),
    'argument' => array(
      'handler' => 'views_handler_argument_string'),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['email'] = array(
    'title' => t('Email'),
    'help' => t('The email address for the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['city'] = array(
    'title' => t('City/Town'),
    'help' => t('The city/town of the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['country'] = array(
    'title' => t('Country'),
    'help' => t('The country of the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['user']['lastaccess'] = array(
    'title' => t('Last access time'),
    'help' => t('The time this user last accessed Moodle'),
    'field' => array(
      'handler' => 'views_handler_field_date'),
    'filter' => array(
      'handler' => 'views_handler_filter_date'));

  // ----- author table alias -----
  $data['author'] = $data['user'];
  unset($data['author']['table']['base']);
  $data['author']['table']['group'] = t('Moodle Course Teacher');
  $data['author']['username']['help'] = t('The username for the course teacher.');

  // ----- creator table alias -----
  $data['creator'] = $data['user'];
  unset($data['creator']['table']['base']);
  $data['creator']['table']['group'] = t('Moodle Course Creator');
  $data['creator']['username']['help'] = t('The username for the course creator.');

  // ----- enrol table -----
  $data['enrol']['table']['group'] = t('Moodle Enrolment');

  // TODO: inverse boolean handler needed!
  $data['enrol']['status'] = array(
    'title' => t('Enrolment suspended'),
    'help' => t('Whether this enrolment has been suspended or not'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'output formats' => array(
        'suspended' => array(
          'Suspended',
          'Active'))),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Enrolment status suspended'),
      'type' => 'dfyes-no'));
  $data['enrol']['enrol'] = array(
    'title' => t('Enrolment type'),
    'help' => t('The type of enrolement'),
    'field' => array(
      'handler' => 'views_handler_field'));

  // ----- enrol table alias for prices -----
  $data['price'] = $data['enrol'];
  $data['price']['table']['group'] = t('Moodle Course');

  $data['price']['cost'] = array(
    'title' => t('Enrolment price'),
    'help' => t('The price for enrolement'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_money',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));
  $data['price']['currency'] = array(
    'title' => t('Enrolment currency'),
    'help' => t('The currency for enrolement'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE),
    'sort' => array(
      'handler' => 'views_handler_sort'));

  // ----- user_enrolments table -----
  $data['user_enrolments']['table']['group'] = t('Moodle User-enrolement');

  $data['user_enrolments']['status'] = array(
    'title' => t('User-enrolment suspended'),
    'help' => t('Whether this user-enrolment has been suspended or not'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'output formats' => array(
        array(
          'Suspended',
          'Active'))),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Suspended'),
      'type' => 'yes-no'));

  $data['user_enrolments']['timestart'] = array(
    'title' => t('Start time'),
    'help' => t('The date this user-enrolement started'),
    'field' => array(
      'handler' => 'views_handler_field_date'),
    'filter' => array(
      'handler' => 'views_handler_filter_date'));

  // ----- grade_grades table -----
  // @See http://docs.moodle.org/dev/Grades#grade_grades
  $data['grade_grades']['table']['group'] = t('Moodle Grades');

  $data['grade_grades']['id'] = array(
    'title' => t('ID'),
    'help' => t('The internal Moodle grade ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric'));
  $data['grade_grades']['finalgrade'] = array(
    'title' => t('Final grade'),
    'help' => t('The final grade'),
    'field' => array(
      'handler' => 'moodle_views_handler_field_grade'));
  $data['grade_grades']['timecreated'] = array(
    'title' => t('Date and time'),
    'help' => t('The date and time this grade was created'),
    'field' => array(
      'handler' => 'views_handler_field_date'),
    'filter' => array(
      'handler' => 'views_handler_filter_date'));

  // ----- grade_items table -----
  // @See http://docs.moodle.org/dev/Grades#grade_items
  $data['grade_items']['table']['group'] = t('Moodle Grades');

  $data['grade_items']['itemname'] = array(
    'title' => t('Name'),
    'help' => t('The name of the thing generating the grade'),
    'field' => array(
      'handler' => 'views_handler_field'),
    'filter' => array(
      'handler' => 'views_handler_filter_string'));
  $data['grade_items']['itemmodule'] = array(
    'title' => t('Module'),
    'help' => t('The module generating the grade (e.g. "quiz")'),
    'field' => array(
      'handler' => 'views_handler_field'),
    'filter' => array(
      'handler' => 'views_handler_filter_string'));
  $data['grade_items']['iteminstance'] = array(
    'title' => t('Module instance'),
    'help' => t('The module instance generating the grade'),
    'field' => array(
      'handler' => 'views_handler_field'));
  $data['grade_items']['grademax'] = array(
    'title' => t('Maximum score'),
    'help' => t('The maximum score for the grade'),
    'field' => array(
      'handler' => 'views_handler_field'));
  $data['grade_items']['grademin'] = array(
    'title' => t('Minimum score'),
    'help' => t('The minimum score for the grade'),
    'field' => array(
      'handler' => 'views_handler_field'));

  // ----- grade_letters table -----
  // @See http://docs.moodle.org/dev/Grades#grade_letters
  $data['grade_letters']['table']['group'] = t('Moodle Grades');
  $data['grade_letters']['letter'] = array(
    'title' => t('Letter'),
    'help' => t('The grade letter assigned to this grade'),
    'field' => array(
      'handler' => 'views_handler_field'));

  // ============== Link courses to users =================
  $data['user']['table']['join']['course'] = array(
    'left_table' => 'user_enrolments',
    'left_field' => 'userid',
    'field' => 'id');
  $data['user_enrolments']['table']['join']['course'] = array(
    'left_table' => 'enrol',
    'left_field' => 'id',
    'field' => 'enrolid');
  $data['enrol']['table']['join']['course'] = array(
    'left_field' => 'id',
    'field' => 'courseid');

  // ============== Link courses to prices (enrol) ==============
  $data['price']['table']['join']['course'] = array(
    'table' => 'enrol',
    'left_field' => 'id',
    'field' => 'courseid',
    'extra' => array(
      array(
        'field' => 'enrol',
        'value' => 'paypal')));

  // ============== Link courses to teachers (role ID = 3) ==============
  $data['author']['table']['join']['course'] = array(
    'table' => 'user',
    'left_table' => 'author_role_assignments',
    'left_field' => 'userid',
    'field' => 'id');

  $data['author_role_assignments']['table']['join']['course'] = array(
    'table' => 'role_assignments',
    'field' => 'contextid',
    'left_table' => 'context',
    'left_field' => 'id',
    'extra' => array(
      array(
        'field' => 'roleid',
        'value' => 3,
        'numeric' => TRUE)));

  $data['context']['table']['join']['course'] = array(
    'field' => 'instanceid',
    'left_table' => 'course',
    'left_field' => 'id',
    'extra' => array(
      array(
        'field' => 'contextlevel',
        'value' => 50,
        'numeric' => TRUE)));

  // ============== Link courses to creator ==============
  $data['creator']['table']['join']['course'] = array(
    'table' => 'user',
    'left_table' => 'creator_role_assignments',
    'left_field' => 'userid',
    'field' => 'id');

  $data['creator_role_assignments']['table']['join']['course'] = array(
    'table' => 'role_assignments',
    'field' => 'contextid',
    'left_table' => 'creator_context',
    'left_field' => 'id',
    'extra' => array(
      array(
        'field' => 'roleid',
        'value' => 2,
        'numeric' => TRUE)));

  $data['creator_context']['table']['join']['course'] = array(
    'table' => 'context',
    'field' => 'instanceid',
    'left_table' => 'course',
    'left_field' => 'id',
    'extra' => array(
      array(
        'field' => 'contextlevel',
        'value' => 50,
        'numeric' => TRUE)));

  // =============== Link users to courses =============
  $data['course']['table']['join']['user'] = array(
    'left_table' => 'enrol',
    'left_field' => 'courseid',
    'field' => 'id');

  $data['enrol']['table']['join']['user'] = array(
    'left_table' => 'user_enrolments',
    'left_field' => 'enrolid',
    'field' => 'id');

  $data['user_enrolments']['table']['join']['user'] = array(
    'left_field' => 'id',
    'field' => 'userid');

  // ============ Link users to grades ================
  // LEFT JOIN {grade_grades} grade_grades
  // ON user.id = grade_grades.userid AND (finalgrade IS NOT NULL)
  $data['grade_grades']['table']['join']['user'] = array(
    'left_field' => 'id',
    'field' => 'userid',
    'extra' => 'finalgrade IS NOT NULL');

  // ============ Link users to grade_items ================
  // INNER JOIN {grade_items} grade_items
  // ON grade_grades.itemid = grade_items.id AND (rawgrade IS NOT NULL)
  $data['grade_items']['table']['join']['user'] = array(
    'left_table' => 'grade_grades',
    'left_field' => 'itemid',
    'field' => 'id',
    'type' => 'INNER',
    'extra' => 'rawgrade IS NOT NULL');

  // ============ Link users to grade_letters ================
  $data['grade_letters']['table']['join']['user'] = array(
    'handler' => 'moodle_views_range_join',
    'left_table' => 'grade_grades',
    'left_field' => 'finalgrade',
    'field' => 'lowerboundary');

  // ============ Link users to graded courses ================
  // LEFT JOIN {course} course_graded
  // ON grade_items.courseid = course_graded.id
  $data['course_graded']['table']['join']['user'] = array(
    'table' => 'course',
    'left_table' => 'grade_items',
    'left_field' => 'courseid',
    'field' => 'id');

  // ============ Link users to graded course categories ================
  // LEFT JOIN {course_categories} course_categories_graded
  // ON course_graded.category = course_categories_graded.id
  $data['course_categories_graded']['table']['join']['user'] = array(
    'table' => 'course_categories',
    'left_table' => 'course_graded',
    'left_field' => 'category',
    'field' => 'id');

  // ========== Link courses to course sections ==========
  $data['course_sections']['table']['join']['course'] = array(
    'left_field' => 'id',
    'field' => 'course');

  // ========== Link users to course_sections ==========
  // TODO: may need custom join handler here...
  $data['course_sections']['table']['join']['user'] = array(
    'handler' => 'moodle_views_multi_join',
    'left_table' => 'course_modules',
    'left_fields' => array(
      'course',
      'section'),
    'fields' => array(
      'course',
      'id'));

  // TODO make custom handler?
  $data['course_modules']['table']['join']['user'] = array(
    'left_table' => 'grade_items',
    'left_field' => 'iteminstance',
    'field' => 'instance',
    'extra' => 'itemmodule = \'quiz\' AND course_modules.module = 16');
  return $data;
}
