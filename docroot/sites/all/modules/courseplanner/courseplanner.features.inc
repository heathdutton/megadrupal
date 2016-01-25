<?php
/**
 * @file
 * courseplanner.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function courseplanner_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "pm_existing_pages" && $api == "pm_existing_pages") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function courseplanner_flag_default_flags() {
  $flags = array();
  // Exported flag: "Favourite".
  $flags['cp_facourite'] = array(
    'content_type' => 'node',
    'title' => 'Favourite',
    'global' => '0',
    'types' => array(
      0 => 'cp_offering',
      1 => 'cp_outline',
      2 => 'cp_section',
      3 => 'cp_resource',
    ),
    'flag_short' => 'Add to favourites',
    'flag_long' => 'Click here to add this item to your favourites',
    'flag_message' => 'Item added.',
    'unflag_short' => 'In favourites',
    'unflag_long' => 'Click here to remove this item from your favourites',
    'unflag_message' => 'Item removed',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'courseplanner',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}

/**
 * Implements hook_node_info().
 */
function courseplanner_node_info() {
  $items = array(
    'cp_offering' => array(
      'name' => t('Course offering'),
      'base' => 'node_content',
      'description' => t('A course offering is an actual scheduling of a course, with participants, lessons and so on.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'cp_outline' => array(
      'name' => t('Course outline'),
      'base' => 'node_content',
      'description' => t('A course outline contains a number of course sections. Each section has a size, describing the (intended) number of lessons to spend on that section. Course outlines may be re-used over several course offerings.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'cp_resource' => array(
      'name' => t('Resource'),
      'base' => 'node_content',
      'description' => t('A resource can be a link to a web page, a book reference, a file upload, or something else that is useful as a learning resource. They can be included in lists in course sections, or used by themselves.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Please enter a title that summarizes this resource, useful in lists of resources.'),
    ),
    'cp_section' => array(
      'name' => t('Course section'),
      'base' => 'node_content',
      'description' => t('When dividing a course into smaller parts, these are called sections. You may use sections to describe parts that have their own defined goals, chapters in a course book, or whatever suits your needs.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
