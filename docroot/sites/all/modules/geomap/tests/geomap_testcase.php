<?php
/**
 * @file
 * Common functions for Geomap tests. 
 *
 * @author Jeremy J. Chinquist
 * @internal The structure of these test cases is due to the Location module. Their work
 *   is impressive.
 */

class GeomapTestCase extends DrupalWebTestCase {

  /**
   * Flatten a post settings array because drupalPost isn't smart enough to.
   * Kept from location module
   */
  function flattenPostData(&$edit) {
    do {
      $edit_flattened = TRUE;
      foreach ($edit as $k => $v) {
        if (is_array($v)) {
          $edit_flattened = FALSE;
          foreach ($v as $kk => $vv) {
            $edit["{$k}[{$kk}]"] = $vv;
          }
          unset($edit[$k]);
        }
      }
    } while (!$edit_flattened);
  }

  function addGeomapContentType(&$settings, $add = array()) {
    // find a non-existent random type name.
    do {
      $name = strtolower($this->randomName(3, 'test_'));
    } while (node_get_types('type', $name));

    $settings = array(
      'name' => $name,
      'type' => $name,
      'location_settings' => array(
        'multiple' => array(
          'max' => 1,
          'add' => 1,
        ),
        'form' => array(
          'fields' => $defaults,
        ),
      ),
    );

    /*
     * Note: use this information for Geomap_CCK tests
     */ 
    //$settings['location_settings'] = array_merge_recursive($settings['location_settings'], $add);
    //$this->flattenPostData($settings);
    //$add = array('location_settings' => $add);
    //$this->flattenPostData($add);
    //$settings = array_merge($settings, $add);
    //$this->drupalPost('admin/content/types/add', $settings, 'Save content type');
    $this->refreshVariables();
    //$settings = variable_get('location_settings_node_'. $name, array());
    return $name;
  }

  /**
   * Delete a node.
   */
  function deleteNode($nid) {
    // Implemention taken from node_delete, with some assumptions regarding
    // function_exists removed.

    $node = node_load($nid);
    db_query('DELETE FROM {node} WHERE nid = %d', $node->nid);
    db_query('DELETE FROM {node_revisions} WHERE nid = %d', $node->nid);

    // Call the node-specific callback (if any):
    node_invoke($node, 'delete');
    node_invoke_nodeapi($node, 'delete');

    // Clear the page and block caches.
    cache_clear_all();
  }

  /**
   * Creates a node based on default settings. This uses the internal simpletest
   * browser, meaning the node will be owned by the current simpletest _browser user.
   *
   * Taken over from Location Testcase
   *
   * Code modified from #212304.
   * This is mainly for testing for differences between node_save() and
   * submitting a node/add/* form.
   *
   * @param values
   *   An associative array of values to change from the defaults, keys are
   *   node properties, for example 'body' => 'Hello, world!'.
   * @return object Created node object.
   */
  function drupalCreateNodeViaForm($values = array()) {
    $defaults = array(
      'type' => 'page',
      'title' => $this->randomName(8),
     );

    $edit = ($values + $defaults);

    if (empty($edit['body'])) {
      $content_type = db_fetch_array(db_query("select name, has_body from {node_type} where type='%s'", $edit['type']));

      if ($content_type['has_body']) {
        $edit['body'] = $this->randomName(32);
      }
    }
    $type = $edit['type'];
    unset($edit['type']); // Only used in URL.
    $this->flattenPostData($edit); // Added by ...
    $this->drupalPost('node/add/'. str_replace('_', '-', $type), $edit, t('Save'));

    $node = node_load(array('title' => $edit['title']));
    $this->assertRaw(t('@type %title has been created.', array('@type' => node_get_types('name', $node), '%title' => $edit['title'])), t('Node created successfully.'));

    return $node;
  }

}
