<?php

/**
 * @file
 * Unit and Web testcases.
 */

/**
 * @class
 *
 * The actual testsuite which implements the DrupalWebTestcase.
 * This class is only used to add extra assertion methods that
 * are not present in the default DrupalWebTestCase.
 */
class ViewsFastForwardTestCase extends DrupalWebTestCase {
  static public function getInfo () {
    return array(
      'name' => 'Views Fast Forward Unit Tests',
      'description' => 'Views Fast Forward DrupalWebTestCase implementation.',
      'group' => 'Views Fast Forward',
    );
  }

  /**
   * Setup
   *
   * Load the necessary modules.
   */
  public function setUp ($modules = array()) {
    parent::setUp(array('views_fast_forward', 'views', 'views_ui'));
  }

  /**
   * Teardown
   *
   * Cleanup our mess.
   */
  public function tearDown() {
    parent::tearDown();
  }

  /**
   * Assert if a checkbox is checked or not.
   */
  public function assertCheckbox($id, $state) {
    $state = $state ? 'checked' : $state;
    $elements = $this->xpath('//input[@type="checkbox" and @id=:id]', array(':id' => $id));
    $this->assertEqual($elements[0]['checked'],
                       $state, t('Checkbox @id is @state.',
                       array('@id' => $id, '@state' => ($state ? 'checked' : 'unchecked'))),
                       t('Browser'));
  }

  /**
   * Assert if a Textbox is empty or contains a certain value.
   */
  public function assertTextbox($id, $value) {
    $elements = $this->xpath('//input[@type="text" and @id=:id]', array(':id' => $id));
    $this->assertEqual($elements[0]['value'],
                       $value,
                       t('Textbox @id @value.', array('@id' => $id, '@value' => ($value == '') ? 'is empty' : 'contains "'.$value.'"')),
                       t('Browser'));
  }

  /**
   * Create a given amount of nodes (actual, maximum 26, then we run out of letters...).
   */
  public function create_nodes ($amount) {
    $nids = array();
    $alphabet = 'abcdefghijklmnopqrstuvwxyz';
    while ($amount != 0) {
      $node = new stdClass();

      $node->title = substr($alphabet, 0, $amount);
      $node->type = "page"; // Page is always present in default D7
      node_object_prepare($node);
      $node->language = LANGUAGE_NONE;
      $node->body['und'][0]['value'] = 'Just some text.';
      $node->status = 1;
      $node->promote = 0;

      $node = node_submit($node);
      node_save($node);

      array_push($nids, $node->nid); // Add nid to our bookkeeping

      $amount = $amount - 1;
    }

    return $nids;
  }

  /**
   * Delete the previously created nodes.
   */
  public function remove_nodes ($nids) {
    foreach ($nids as $nid) {
      node_delete($nid);
    }
  }

  /**
   * Create a basic View with node as its base table,
   * a master display and a page to test on.
   */
  public function create_node_view () {
    $view = views_new_view();
    $view->name = 'auto-view';
    $view->description = 'Auto view for unit testing.';
    $view->tag = 'default';
    $view->base_table = 'node';
    $view->human_name = 'auto-view';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE;

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';

    views_save_view($view);

    return $view;
  }

  /**
   * Attach a pre-made page to an existing View (Fields style).
   */
  public function create_page_display_fields($view) {
    // Create a new page display.
    $handler = $view->new_display('page', 'Page', 'page_1');

    // Set its basic options
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['path'] = 'nodes';

    // Set the "No results" behavior
    $handler->display->display_options['defaults']['empty'] = FALSE;
    $handler->display->display_options['empty']['area']['id'] = 'area';
    $handler->display->display_options['empty']['area']['table'] = 'views';
    $handler->display->display_options['empty']['area']['field'] = 'area';
    $handler->display->display_options['empty']['area']['empty'] = TRUE;
    $handler->display->display_options['empty']['area']['content'] = 'The View did not return any results.';
    $handler->display->display_options['empty']['area']['format'] = 'filtered_html';

    // Set display style to fields
    $handler->display->display_options['defaults']['style_plugin'] = FALSE;
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['defaults']['style_options'] = FALSE;
    $handler->display->display_options['defaults']['row_plugin'] = FALSE;
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['defaults']['row_options'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;

    // Add a Title field
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = '';
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;

    // Set an exposed filter on the title field
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
    $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );

    return $handler;
  }

  /**
   * Attach a pre-made page to an existing View (Content style).
   */
  public function create_page_display_content($view) {
    // Create a new page display.
    $handler = $view->new_display('page', 'Page', 'page_1');

    // Setup the display defaults
    $handler->display->display_options['defaults']['style_plugin'] = FALSE;
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['defaults']['style_options'] = FALSE;
    $handler->display->display_options['defaults']['row_plugin'] = FALSE;
    $handler->display->display_options['row_plugin'] = 'node';
    $handler->display->display_options['defaults']['row_options'] = FALSE;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    $handler->display->display_options['path'] = 'nodes';

    // Set an exposed filter on the title field
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
    $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );

    return $handler;
  }

  /**
   * Setup Views Fast Forward into an existing page ($handler) of an existing View.
   */
  public function enable_views_fast_forward($handler, $manual = FALSE, $tokenize = FALSE, $path = '') {
    // Set the Views Fast Forward options (automatic)
    $handler->display->display_options['ff_enabled'] = TRUE;
    $handler->display->display_options['ff_manual'] = $manual;
    $handler->display->display_options['ff_tokenize'] = $tokenize;
    $handler->display->display_options['ff_path'] = $path;
  }

}
