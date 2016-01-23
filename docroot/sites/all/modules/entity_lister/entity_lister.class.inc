<?php

/**
 * @file
 * Entity Lister class file.
 */

class EntityLister {

  public $config;
  public $delta;
  public $idPrefix;
  public $total;
  protected $bundleFields;

  /**
   * Construct the list object.
   *
   * @param array $config
   *   The config options for the list, in an associative array.  See below.
   * @param int $delta
   *   The delta.
   * @param string $id_prefix
   *   An id prefix for the container div.  If the prefix is foo, and the delta
   *   is 0, the id for the container div will be "foo-0".
   *
   *   $config['type']                string
   *     Entity type (e.g. 'comment', etc).
   *   $config['bundle']              array or json encoded string
   *   $config['view_mode']           string
   *   $config['numitems']            '1', '2', etc
   *   $config['sort']                array
   *     An array defining one or more sort configurations. Each
   *     configuration is itself an array with keys for 'method' and
   *     'args'.  The 'method' for each configuration should be set to either
   *     'propertyOrderBy' or 'fieldOrderBy'.  For the args, see the docs for
   *     EntityFieldQuery::propertyOrderBy and EntityFieldQuery::fieldOrderBy
   *     at https://api.drupal.org.  See also the demo page and
   *     EntityLister::defaults() for a 'propertyOrderBy' example.
   *   $config['status']              'all', '0' or '1'
   *     Set status to 'all' to list all items, unpublished and published.
   *     Set status to '0' to list only unpublished items.
   *     Set status to '1' to list only published items.
   *   $config['pager']               '0', '1', or '2'
   *     Set to '0' for none, '1' for standard pager, or '2' for AJAX pager.
   *   $config['pager_position']      '0', '1', or '2'
   *     Set to '0' for above, '1' for below, or '2' for both.
   *   $config['pager_st']            '0' or '1'
   *     Set to '0' to deactivate scrolling to the top of the container.
   *   $config['pager_st_offset']     string
   *     Scrolltop offset from container top.
   *   $config['pager_st_time']       string
   *     Scrolltop time.
   *   $config['pager_path']          string
   *     The path for the AJAX pager callback.
   *   $config['cache']               '0' or '1'
   *     Set to '1' to enable caching.
   *   $config['get_total']           '0' or '1'
   *     Set to '1' to execute a separate query for the total count of items.
   *   $config['template']            string
   *     Specify an alternate template.
   *   $config['item_list']           '0' or '1'
   *     Set to '1' to pass the items to theme('item_list').
   *   $config['tag']                 string
   *     Specify a tag for addTag().
   *   $config['property_condition']  array
   *     Define one or more property conditions.
   *   $config['field_condition']     array
   *     Define one or more field conditions.
   *   $config['headers']             string
   *     Table header output.
   *   $config['table_sort']          array
   *     A header definition to pass to tableSort().
   */
  public function __construct($config = array(), $delta = 0, $id_prefix = 'entity-lister') {

    $this->config = $config;
    $this->delta = $delta;
    $this->idPrefix = $id_prefix;

  }

  /**
   * Apply default values to the config where needed.
   */
  protected function defaults() {

    $c = $this->config;

    $config['type'] = isset($c['type']) ? $c['type'] : 'node';

    // For bundle, we accept either an array or a json encoded string. If it's
    // the latter, convert to an array here for consistency.
    if (isset($c['bundle']) && !is_array($c['bundle'])) {
      $c['bundle'] = json_decode($c['bundle']);
      if (NULL === $c['bundle']) {
        $msg = 'Invalid json string in EntityLister::getList().';
        watchdog('entity_lister', $msg);
        return '';
      }
    }

    if (!isset($c['bundle']) || empty($c['bundle'])) {
      // Bundle is not set or is empty, so we set it to all bundles for the
      // entity type.
      $entity_info = entity_get_info($config['type']);
      $c['bundle'] = array_keys($entity_info['bundles']);
    }

    // At this point we have one or more bundles in the form of an array.
    $config['bundle'] = $c['bundle'];

    $config['view_mode'] = isset($c['view_mode']) ? $c['view_mode'] : 'teaser';
    $config['numitems'] = isset($c['numitems']) ? $c['numitems'] : '3';
    $sort = array(
      array(
        'method' => 'propertyOrderBy',
        'args' => array(
          'property' => 'title',
          'direction' => 'DESC',
        ),
      ),
    );
    $config['sort'] = isset($c['sort']) ? $c['sort'] : $sort;
    $config['status'] = isset($c['status']) ? $c['status'] : '1';
    $config['pager'] = isset($c['pager']) ? $c['pager'] : '2';
    $config['pager_position'] = isset($c['pager_position']) ? $c['pager_position'] : '1';
    $config['pager_st'] = isset($c['pager_st']) ? $c['pager_st'] : '1';
    $config['pager_st_offset'] = isset($c['pager_st_offset']) ? $c['pager_st_offset'] : '175';
    $config['pager_st_time'] = isset($c['pager_st_time']) ? $c['pager_st_time'] : '900';
    $config['pager_path'] = isset($c['pager_path']) ? $c['pager_path'] : 'entity_lister/pageturner';
    $config['cache'] = isset($c['cache']) ? $c['cache'] : '1';
    $config['get_total'] = isset($c['get_total']) ? $c['get_total'] : '0';
    $config['template'] = isset($c['template']) ? $c['template'] : '';
    $config['item_list'] = isset($c['item_list']) ? $c['item_list'] : '0';
    $config['tag'] = isset($c['tag']) ? $c['tag'] : '';
    $config['property_condition'] = isset($c['property_condition']) ? $c['property_condition'] : array();
    $config['field_condition'] = isset($c['field_condition']) ? $c['field_condition'] : array();
    $config['headers'] = isset($c['headers']) ? $c['headers'] : '';
    $config['table_sort'] = isset($c['table_sort']) ? $c['table_sort'] : array();

    $this->config = $config;

  }

  /**
   * Return the list as themed output.
   *
   * @param string $page
   *   The page we're on. For paged results, this will be '0' or an empty
   *   string for the first page of results, '1' for the second page, and so on.
   *
   * @return string
   *   The themed output.
   */
  public function getList($page = '') {

    $this->defaults();

    if ('0' === $this->config['numitems']) {
      return '';
    }

    $type = $this->config['type'];

    // Check if the user can view the items.  See the accessCheck() for notes
    // on supported entity types.  The base class provides checks for nodes,
    // users and comments.
    $access = $this->accessCheck();
    if (!$access[0]) {
      // User can't view this entity type at all, so return an empty string.
      return '';
    }
    // Bundles the user can view.
    $this->config['bundle'] = $access[1];

    if ('1' === $this->config['cache']) {
      $cache_config = $this->config;
      $cache_config['page'] = $page;
      // Determine the cache ID.
      $cid = $this->cid($cache_config);
      // Return the cached data if we have any.
      if ($cachedata = cache_get($cid)) {
        if ('2' === $this->config['pager']) {
          $this->pager();
        }
        return $cachedata->data;
      }
    }

    $this->bundleFields = array();
    // Build an array containing all fields in all the bundles.
    foreach ($this->config['bundle'] as $value) {
      $this->bundleFields += field_info_instances($type, $value);
    }

    /* Tell EntityFieldQuery and the entity's view function what page
     * we're on. We can set this any time before we execute the query.
     */
    $_GET['page'] = $page;

    $query = $this->query();
    $query = $this->alterQuery($query);
    $result = $query->execute();

    if (!$result || !isset($result[$type])) {
      return '';
    }
    else {
      $ids = array_keys($result[$type]);
      $entities = entity_load($type, $ids);
    }

    if ($entities) {
      $entities = $this->alterEntities($entities);
      $output = $this->theme($entities);
    }
    else {
      $output = '';
    }

    if ('1' === $this->config['cache']) {
      cache_set($cid, $output, 'cache', CACHE_TEMPORARY);
    }

    return $output;

  }

  /**
   * For the standard pager, determine what page we're on.
   *
   * @return string
   *   The page number, e.g. '2'.
   */
  public function getPageNum() {

    return isset($_GET['page']) ? $_GET['page'] : '';

  }

  /**
   * Build the query.
   *
   * @return object
   *   The query.
   */
  protected function query() {

    extract($this->config);

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', $type);

    if (!empty($tag)) {
      $query->addTag($tag);
    }

    $prop_info = entity_get_property_info($type);
    $props = $prop_info['properties'];

    $query = $this->bundleCondition($query, $props);

    // Not all entity types have a status property.
    if (isset($props['status']) && 'all' !== $status) {
      $query->propertyCondition('status', $status);
    }

    // For any property conditions besides type and status.
    if (!empty($property_condition)) {
      foreach ($property_condition as $key => $val) {
        $column = isset($val['column']) ? $val['column'] : NULL;
        $value = isset($val['value']) ? $val['value'] : NULL;
        $operator = isset($val['operator']) ? $val['operator'] : NULL;
        $query->propertyCondition($column, $value, $operator);
      }
    }

    if (!empty($field_condition)) {
      foreach ($field_condition as $key => $val) {
        $field = $val['field'];
        $column = isset($val['column']) ? $val['column'] : NULL;
        $value = isset($val['value']) ? $val['value'] : NULL;
        $operator = isset($val['operator']) ? $val['operator'] : NULL;
        $delta_group = isset($val['delta_group']) ? $val['delta_group'] : NULL;
        $language_group = isset($val['language_group']) ? $val['language_group'] : NULL;
        $query->fieldCondition($field, $column, $value, $operator, $delta_group, $language_group);
      }
    }

    if ($this->config['get_total']) {
      // Count the total number of items matching the conditions.
      $result = $query->execute();
      if ($result && isset($result[$type])) {
        $this->total = count($result[$type]);
      }
      else {
        $this->total = NULL;
      }
    }

    if (!empty($table_sort)) {
      $query->tableSort($table_sort);
    }
    else {
      foreach ($sort as $value) {
        $method = $value['method'];
        $args = $value['args'];
        $direction = isset($args['direction']) ? $args['direction'] : 'ASC';
        if ('propertyOrderBy' == $method) {
          $query->propertyOrderBy($args['property'], $direction);
        }
        elseif ('fieldOrderBy' == $method) {
          $query->fieldOrderBy($args['field'], $args['column'], $direction);
        }
      }
    }

    if ('0' !== $pager) {
      $query->pager($numitems, $this->delta);
    }
    else {
      $query->range(0, $numitems);
    }

    return $query;

  }

  /**
   * Add the bundle condition.
   *
   * @param object $query
   *   The query object.
   * @param array $properties
   *   The properties returned by entity_get_property_info().
   *
   * @return object
   *   The query object.
   */
  protected function bundleCondition($query, $properties) {

    if (!empty($this->config['bundle'])) {

      // Deal with inconsistencies re: how entities store bundle data.
      // Checking the type property works seemingly for everything except
      // taxonomy_term bundles.
      if (isset($properties['type'])) {
        $query->propertyCondition('type', $this->config['bundle']);
      }
      // This entityCondition works in some cases, taxonomy_term being one
      // of those cases.
      elseif ('taxonomy_term' == $this->config['type']) {
        $query->entityCondition('bundle', $this->config['bundle']);
      }

    }

    return $query;

  }

  /**
   * Determine a cache ID.
   *
   * Use the list config and other data to build the cache ID.
   *
   * @param array $cache_config
   *   Data used to determine the cache ID.
   *
   * @return string
   *   The cid.
   */
  protected function cid($cache_config) {

    extract($cache_config);

    $arr[] = $type;

    // Besides the type, it doesn't matter in what order we add things
    // to $arr, as long as we keep it consistent.
    $bundle = implode(',', $bundle);
    $arr[] = $bundle;

    $arr[] = $view_mode;

    // A cache entry can only be shared by users who have the same set of roles.
    global $user;
    $roles = $user->roles;
    ksort($roles);
    $arr[] = implode(',', $roles);

    $len = strlen(implode(':', $arr));
    // Hold the total length of the cid to a max of 255 chars.  Later we add 47
    // more characters, so by this point we can't have more than 208.
    if ($len > 208) {
      // Add the bundles, view_mode and roles to the hash.
      $hash[] = $arr[1];
      $hash[] = $arr[2];
      $hash[] = $arr[3];
      // Start the $arr over.
      $arr = array();
      $arr[0] = $type;
    }

    // To keep the cid brief we add the rest of the components to an array,
    // implode the array and the generate a 32-character hash.
    global $language;
    $hash[] = $language->language;
    $hash[] = $numitems;
    $hash[] = json_encode($sort);
    $hash[] = $status;
    $hash[] = $pager;
    $hash[] = $pager_position;
    $hash[] = $pager_st;
    $hash[] = $pager_st_offset;
    $hash[] = $pager_st_time;
    $hash[] = $pager_path;
    $hash[] = $get_total;
    $hash[] = $template;
    $hash[] = $item_list;
    $hash[] = $tag;
    $hash[] = $headers;
    $hash[] = json_encode($table_sort);
    $hash[] = json_encode($property_condition);
    $hash[] = json_encode($field_condition);
    $hash[] = $page;

    // The final ingredient in the cid is this hash.
    $arr[] = hash('md5', implode(',', $hash));

    $cid = 'entity_lister:' . implode(':', $arr);

    return $cid;

  }

  /**
   * Theme the list.
   *
   * @param array $entities
   *   The entities to list.
   *
   * @return string
   *   The themed output.
   */
  protected function theme($entities) {

    $pager = $this->config['pager'];
    $pager_position = $this->config['pager_position'];

    if ('0' !== $pager || !empty($this->config['headers'])) {
      // We need this js for paged or tabular lists.
      drupal_add_js(drupal_get_path('module', 'entity_lister') . '/parse-qs.js');
    }

    if ('2' === $pager) {
      $this->pager();
    }
    if ('0' !== $pager) {
      $vars['above'] = '0' === $pager_position || '2' == $pager_position;
      $vars['below'] = '1' === $pager_position || '2' == $pager_position;
    }
    else {
      $vars['above'] = $vars['below'] = FALSE;
    }

    $items = $this->renderItems($entities);

    if ('1' == $this->config['item_list']) {
      $vars['list'] = theme('item_list', array('items' => $items));
    }
    else {
      $vars['list'] = implode("", $items);
    }

    $vars['headers'] = $this->config['headers'];

    $vars['pager'] = theme('pager', array('element' => $this->delta));
    $vars['total'] = $this->config['get_total'] ? $this->total : NULL;
    // We might want access to the config vars in the template.
    $vars['config'] = $this->config;

    $output = theme('entity_lister_list', $vars);

    return $output;

  }

  /**
   * Render list items.
   *
   * @param array $entities
   *   The entities to list.
   *
   * @return array
   *   The rendered items.
   */
  protected function renderItems($entities) {

    $type = $this->config['type'];
    $viewfunc = $type . '_view';

    if (function_exists($viewfunc)) {
      foreach ($entities as $entity) {
        $items[] = drupal_render($viewfunc($entity, $this->config['view_mode']));
      }
    }
    elseif (function_exists('theme_entity_lister_' . $type)) {
      // How to provide a custom theme function in cases when an entity type
      // has no view function, as is the case with the taxonomy_vocabulary type.
      // See theme_entity_lister_taxonomy_vocabulary() for an example.
      $vars['entities'] = $entities;
      // Make the view mode available in the theme function.
      $vars['view_mode'] = $this->config['view_mode'];
      $items = theme('entity_lister_' . $type, $vars);
    }
    else {
      $msg = 'No view function or theme function in EntityLister::renderItems().';
      watchdog('entity_lister', $msg);
      // No view function and no theme function, so just list the titles.
      foreach ($entities as $entity) {
        $items[] = $entity->name;
      }
    }

    return $items;

  }

  /**
   * Activate the ajax pager.
   */
  protected function pager() {

    $path = drupal_get_path('module', 'entity_lister');
    drupal_add_js($path . '/ajax-pager.js');

    $js_settings[$this->delta] = array(
      'config' => $this->config,
      'id_prefix' => $this->idPrefix,
      'list_class' => get_class($this),
      /* Classes that override EntityLister may want to make use of this
       * 'extra' setting.  Here we pass an empty array so we don't get an error
       * in the js.
       */
      'extra' => array(),
    );
    drupal_add_js(array('entity_lister' => $js_settings), 'setting');

  }

  /**
   * Wrap the list in an element for rendering.
   *
   * @param string $list
   *   The themed list output.
   * @param string $htmltag
   *   The HTML tag to use for wrapping the list.
   *
   * @return array
   *   The element.
   */
  public function element($list, $htmltag = 'div') {

    // The AJAX pager uses this id.
    $id = check_plain($this->idPrefix . '-' . $this->delta);

    $element[$id] = array(
      '#prefix' => '<' . $htmltag . ' id="' . $id . '" class="entity-lister">',
      '#markup' => $list,
      '#suffix' => '</' . $htmltag . '>',
    );

    return $element;

  }

  /**
   * Check if the user can view the list items.
   *
   * Provides access checks for nodes, users and comments.  To provide access
   * checks for other entity types, extend the class.
   *
   * @return array
   *   Info about the user's access to this type and its bundles.
   *   $info[0] bool -- Can we view the type?
   *   $info[1] array -- For nodes, the bundles we can view.
   */
  protected function accessCheck() {

    // Begin with the assumption that we can view the type and all its bundles.
    $info = array(TRUE, $this->config['bundle']);

    if ('node' == $this->config['type']) {

      if (!user_access('access content')) {
        $info[0] = FALSE;
      }
      else {
        // Add the node_view_permissions integration.
        $info[1] = $this->nodeViewPermissions();
      }

    }
    elseif ('user' == $this->config['type']) {
      if (!user_access('access user profiles')) {
        $info[0] = FALSE;
      };
    }
    elseif ('comment' == $this->config['type']) {
      if (!user_access('access comments')) {
        $info[0] = FALSE;
      };
    }
    return $info;

  }

  /**
   * Add the node_view_permissions integration.
   *
   * @return array
   *   Bundles the user is allowed to view.
   */
  protected function nodeViewPermissions() {

    $bundle = $this->config['bundle'];

    if (module_exists('node_view_permissions')) {
      $nvp_bundles = node_view_permissions_get_configured_types();
      foreach ($nvp_bundles as $key => $value) {
        $perm = 'view any ' . $value . ' content';
        if (!user_access($perm)) {
          $key = array_search($value, $bundle);
          unset($bundle[$key]);
        };
      }
    }
    return $bundle;

  }

  /**
   * Additional processing of the selected entities.
   *
   * Classes that extend EntityLister may need to perform additional processing
   * on the selected entities prior to theming.
   *
   * @param array $entities
   *   The entities in the list.
   *
   * @return array
   *   Entities.
   */
  protected function alterEntities($entities) {

    return $entities;

  }

  /**
   * Provide an opportunity to alter the query object.
   *
   * Allow classes that extend EntityLister to perform additional processing
   * on the query object without having to override the entire query() method.
   *
   * @param object $query
   *   The query object.
   *
   * @return object
   *   The query object.
   */
  protected function alterQuery($query) {

    return $query;

  }

  /**
   * Helper function for adding a class to the allowed classes array.
   *
   * This is necessary for the AJAX pager to function, but is harmless in any
   * case.  If you are sure the AJAX pager will never be used on your site, you
   * don't have to do this.
   *
   * Modules extending EntityLister should add their class to the allowed
   * classes array, like so:
   *
   * EntityLister::addAllowedClass('MyClass');
   *
   * A good place to do this is in your hook_enable() call in your install file.
   *
   * @param string $class
   *   The class to allow.
   */
  public function addAllowedClass($class) {

    $allowed_classes = variable_get('entity_lister_allowed_classes', array());
    $allowed_classes[$class] = $class;
    variable_set('entity_lister_allowed_classes', $allowed_classes);

  }

  /**
   * Helper function for removing a class from the allowed classes array.
   *
   * Modules extending EntityLister should remove their class in hook_disable
   * like so:
   *
   * EntityLister::removeAllowedClass('MyClass');
   *
   * @param string $class
   *   The class to disallow.
   */
  public function removeAllowedClass($class) {

    $allowed_classes = variable_get('entity_lister_allowed_classes', array());
    if (isset($allowed_classes[$class])) {
      unset($allowed_classes[$class]);
    }
    variable_set('entity_lister_allowed_classes', $allowed_classes);

  }

}
