URL stucture: /mean/json/[TYPE]/[NAME]/[ARG1]/[ARG2]/[ARG3]
currently supported TYPE values and their arguments:

- view - returns results for requested Drupal view object, arguments:
  - NAME - view's name
  - ARG1 - (optional) display_name
  - ARG2 - (optional) items per page value (-1 for unlimited, 0 - skip arg)
  - ARG3 - (optional) arguments divided by '+' sign without whitespaces
  
- menu - output from menu_tree_all_data() function

- load - looks for load_[NAME_mandatory] function and returns its output 
(node_load([ARG1]), user_load([ARG1]), menu_load([ARG1]) ...), arguments:
  - ARG1 - ID or name to pass to load function

- help - /mean/json/help/me will output current instruction

- user - /mean/json/user/logout - Logging current user out
  - /mean/json/user/login (along with name and pass values passed as post
  will log user in and return its object)
  - /mean/json/user/session - returns $_SESSION
  - /mean/json/user/me - returns global user object

- taxonomy
  - returns output of taxonomy_get_tree($name), $name is vocabulary ID
  if term_fields module is enabled and there are fields, they will be
  attached to $term->fields, if the field is file, its absolute URL
  will be returned instead of FID

- role
  - return output of user_roles()

- get-mean-packet 
  - returns set of JSON objects, set at admin/build/mean-packets
  - NAME - packet's name

-------------------------------------------------------------------------
To skip any optional ARG, use 0, (example: /mean/json/forum_search/0/5)

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
-------------------------------------------------------------------------
Define custom mean type with hook_mean_json_type_info():
/**
 *  Implementation of hook_mean_json_type_info().
 *  name and description used in types list at mean packets page
 *  name and menu_callback used for processing arguments and fetching data
 */
function YOUR_MODULE_mean_json_type_info() {
  return array(
    'name' => 'CUSTOM_MEAN_TYPE_NAME',
    'description' => t('CUSTOM_MEAN_TYPE_DESCRIPTION'),
    'menu_callback' => 'CUSTOM_MEAN_TYPE_MENU_CALLBACK'
  );
}

/**
 * Custom json type menu callback
 * returns desired data, which will be fetched as JSON
 * must receive 4 arguments (see mean_json() for reference)
 */
function ideal_mean_menu_callback($name, $arg1, $arg2, $arg3) {
  return 'DESIRED_DATA_STRING/OBJECT';
}


Mean (http://mean.io) is an open source project by Linnvate (http://linnovate.net)
