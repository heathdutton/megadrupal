<?php
/**
 * @file
 * Classes that transforms a given url to the non-alias, non-redirected version.
 *
 * We have a class to fix a specific url, and one that finds urls in nodes
 * from migration groups, specific migrations, or given set of nids.
 */

class MigrationNodesLegacyLinksFixer {
  private $nids = array();
  private $debugInfo = array();

  /**
   * Setter for nids.
   *
   * @param array $nids
   *   An array of nids.
   *
   * @throws Exception
   *   If nids is not an array.
   */
  public function setNids($nids) {
    if (is_array($nids)) {
      $this->nids = $nids;
    }
    else {
      throw new Exception("nids should be an array of node ids");
    }
  }

  /**
   * Get all the nids from this specific migration.
   *
   * @param string $migration_name
   *   A migration name.
   *
   * @throws Exception
   *   If a migration does not seem to be valid, or it is not a Page,
   *   PressRelease or Case migration.
   */
  public function getNidsFromMigration($migration_name) {
    if (substr_count($migration_name, "Page") > 0 || substr_count($migration_name, "PressRelease") > 0 || substr_count($migration_name, "Case") > 0) {
      // Validate the migration by checking for the existance of the migration
      // map table.
      $table_name = "migrate_map_" . strtolower($migration_name);
      if (!db_table_exists($table_name)) {
        throw new Exception("The migration map table does not exist for this migration");
      }

      drush_migration_tools_debug_output("Getting nids from the {$migration_name} migration.");

      // Lets get all the nodes relevant to this migration.
      $query = db_select($table_name, "t");
      $query->fields("t", array('destid1'));
      $results = $query->execute();
      foreach ($results as $result) {
        $this->nids[] = $result->destid1;
      }
    }
    else {
      throw new Exception("Only Page, PressRelease, and Case Migrations are supported");
    }
  }

  /**
   * Get all of the nodes that belong to the supported migration in this group.
   *
   * @param string $group
   *   A migration group.
   */
  public function getNidsFromMigrationGroup($group) {
    // Check how to get the nids by level of specificity.
    $migrations_info = migrate_get_module_apis();
    $migrations = array();

    foreach ($migrations_info as $implementor => $info) {
      if (!empty($info['migrations'])) {
        foreach ($info['migrations'] as $migration_name => $migration_info) {
          if ($migration_info['group_name'] == $group) {
            $migrations[] = $migration_name;
          }
        }
      }
    }

    foreach ($migrations as $migration_name) {
      try {
        $this->getNidsFromMigration($migration_name);
      }
      catch(Exception $e) {
        // Some migrations are not supported but since we know this, and
        // are getting data from the system and not the user, we can
        // simply ignore this exception.
      }
    }
  }

  /**
   * Get the body from a node.
   *
   * @param object $node
   *   A node object.
   *
   * @return mixed
   *   The body string or false.
   */
  private function getBodyFromNode($node) {
    $items = FALSE;
    try {
      $type = $node->type;

      // Check the node is of the supported types.
      if ($type != "page" && $type != "press_release" && $type != "case") {
        drush_migration_tools_debug_output("Ignored node {$node->nid} of unsupported type {$type}.");
        return FALSE;
      }

      if ($type == "press_release") {
        $type = "pr";
      }
      $items = field_get_items("node", $node, "field_{$type}_body");
    }
    catch(Exception $e) {
      // This node is malformed, lets not process it.
      drush_migration_tools_debug_output("Node {$node->nid} is malformed.");
    }

    if ($items == FALSE) {
      $items = array();
    }
    $item = reset($items);

    if ($item != FALSE) {
      return $item['value'];
    }
    return FALSE;
  }

  /**
   * Get the html body from the query path object.
   *
   * @param QueryPath $qp
   *   A query path object.
   *
   * @return string
   *   The body from the query path object.
   */
  private function getNewBody($qp) {
    $body = $qp->html();
    // Need to remove the extras querypath added.
    $body = str_replace('<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">', "", $body);
    $body = str_replace('<html><body>', "", $body);
    $body = str_replace('</body></html>', "", $body);
    return $body;
  }

  /**
   * Fix all the links with anchors from the given body.
   *
   * @param string $body
   *   A string with the body from an html page.
   *
   * @return mixed
   *   Either the new body with the fixed links, or false if no links were
   *   changed.
   */
  private function fixAnchorsFromBody($body) {
    $changed = FALSE;
    require_once DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';
    $qp = htmlqp($body);
    // Check all the links.
    foreach ($qp->find("a") as $a) {
      $href = $a->attr('href');
      $fixer = NULL;
      try {
        $fixer = new LegacyUrlWithAnchorFixer($href);
      }
      catch(Exception $e) {
        // Here we can see what is being ignored.
        // drush_migration_tools_debug_output("URL IGNORED: {$href} \n");
      }

      $new_href = NULL;
      if (isset($fixer)) {
        try {
          $new_href = $fixer->fix();
        }
        catch(Exception $e) {
          drush_migration_tools_debug_output("URL WITHOUT REDIRECT: {$href}");
        }
      }

      if (isset($new_href)) {
        $a->attr('href', $new_href);
        $this->debugInfo[$href] = $new_href;
        $changed = TRUE;
      }
    }

    if ($changed) {
      return $this->getNewBody($qp);
    }
    return FALSE;
  }

  /**
   * Fix all urls with anchors for the node with the given nid.
   *
   * @param int $nid
   *   A noide id.
   */
  private function fixAnchorsFromNode($nid) {
    $node = node_load($nid);
    $body = $this->getBodyFromNode($node);
    if ($body != FALSE) {
      $new_body = $this->fixAnchorsFromBody($body);
      if ($new_body != FALSE) {
        $type = $node->type;
        if ($type == "press_release") {
          $type = "pr";
        }
        $wrapper = entity_metadata_wrapper('node', $node);
        $wrapper->{"field_{$type}_body"}->value->set($new_body);
        node_save($node);
        drush_migration_tools_debug_output("Modified Node: " . $node->nid);
      }
    }
  }

  /**
   * Print the original links followed by the new fixed links.
   */
  public function printExtraDebugging() {
    drush_migration_tools_debug_output("Changed links:");
    foreach ($this->debugInfo as $href => $new_href) {
      drush_migration_tools_debug_output("Legacy: " . $href);
      drush_migration_tools_debug_output("New: " . $new_href);
    }
  }

  /**
   * Fix any links with anchors in the already collected nids.
   */
  public function fix() {
    foreach ($this->nids as $nid) {
      $this->fixAnchorsFromNode($nid);
    }
  }
}

class LegacyUrlWithAnchorFixer {
  private $url;

  private $prefix;
  private $cleanUrl;
  private $anchor;
  private $redirect;

  /**
   * Constructor.
   */
  public function __construct($url) {
    $this->setUrl($url);
    $this->debug = FALSE;
  }

  /**
   * Url setter.
   */
  private function setUrl($url) {
    // URLs or URI that we want to work with should meet 2 conditions.
    // 1) They should have an anchor, 2) Can not be just an achor.
    if ($this->urlHasAnchor($url) && $this->urlIsNotJustAnAnchor($url)) {
      $this->url = $url;
    }
    else {
      throw new Exception("Url is just an anchor or does not contain an anchor");
    }
  }

  /**
   * Make usre the url has an anchor (ex. http://hello.com/boo#dah).
   */
  private function urlHasAnchor($url) {
    if (substr_count($url, "#") > 0) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Check that the url is not just an anchor (ex. #hello).
   */
  private function urlIsNotJustAnAnchor($url) {
    $first_char = substr($url, 0, 1);
    if (substr_count($first_char, "#") == 0) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Create a version of the given url, using drupals redirect.
   */
  public function fix() {
    $this->breakUrl();
    $this->redirect = $this->getRedirect();
    return $this->prefix . $this->redirect . '#' . $this->anchor;
  }

  /**
   * Break the url into prefix, uri, and anchor.
   */
  private function breakUrl() {
    $this->setPrefix();
    $this->setAnchor();
    $this->setCleanUrl();
  }

  /**
   * Isolate and store whatever is before the uri.
   */
  private function setPrefix() {
    // We could have a full url.
    if (substr_count($this->url, "somesite.gov") > 0) {
      $pieces = explode("somesite.gov/", $this->url);
      $this->prefix = $pieces[0] . "somesite.gov/";
    }
    // Else we might have a rooted url (/) or a relative url (../).
    else {
      $position = 0;
      do {
        $first_char = substr($this->url, $position, 1);
        $position++;
      } while ($first_char == "." || $first_char == "/");

      $this->prefix = substr($this->url, 0, $position - 1);
    }
  }

  /**
   * Get the anchor part from the url.
   */
  private function setAnchor() {
    $pieces = explode("#", $this->url);
    $this->anchor = $pieces[1];
  }

  /**
   * Set the clean url variable. Url without prefix or anchor.
   */
  private function setCleanUrl() {
    $prefix_length = strlen($this->prefix);
    $clean_url = substr($this->url, $prefix_length);
    $clean_url = str_replace("#{$this->anchor}", "", $clean_url);
    $this->cleanUrl = $clean_url;
  }

  /**
   * Get a redirect matching the clean url as source.
   */
  private function getRedirect() {
    $query = db_select('redirect', 't');
    $query->fields("t", array('redirect'));
    $query->condition("source", $this->cleanUrl, "=");
    $results = $query->execute();
    foreach ($results as $result) {
      return $result->redirect;
    }

    throw new Exception("No redirect was found for this URL");
  }
}
