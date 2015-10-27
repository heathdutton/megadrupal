<?php

/**
 * Provides a selection handler for fields referencing issues.
 *
 * This allows matching on any of the following:
 *  - a full URL to an issue node, e.g. 'http://example.com/node/1234#anchor'.
 *  - a node ID prefixed with a #, e.g. '#1234' (this ties in with the intput
 *    format in use on drupal.org).
 *  - a node title.
 */
class ProjectIssue_SelectionHandler_Issues extends EntityReference_SelectionHandler_Generic implements EntityReference_SelectionHandler {

  /**
   * {@inheritdoc}
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $target_entity_type = $field['settings']['target_type'];

    // Check if the entity type does exist and has a base table.
    $entity_info = entity_get_info($target_entity_type);
    // We only work with a target of 'node', as issues must be nodes.
    if (empty($entity_info['base table']) || $target_entity_type != 'node') {
      return EntityReference_SelectionHandler_Broken::getInstance($field, $instance);
    }

    return new ProjectIssue_SelectionHandler_Issues($field, $instance, $entity_type, $entity);
  }

  /**
   * {@inheritdoc}
   */
  public static function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);

    $issue_node_types_options = array();
    $node_type_names = node_type_get_names();
    foreach ($node_type_names as $machine_name => $label) {
      if (project_issue_node_type_is_issue($machine_name)) {
        $issue_node_types[$machine_name] = $label;
      }
    }
    $form['target_bundles']['#options'] = $issue_node_types;

    return $form;
  }

  /**
   * Implements EntityReferenceHandler::getReferencableEntities().
   */
  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 25) {
    $options = array();
    $target_node_types = $this->field['settings']['handler_settings']['target_bundles'];
    // No target node types means all issue types may be selected.
    if (empty($target_node_types)) {
      $target_node_types = project_issue_issue_node_types();
    }

    global $base_url;

    // Early return if the short match string would generate too many results.
    // @todo: Make this configurable? Return even if user has ebereted #nid.
    // It's unlikely they're trying to reference a two digit nid issue.
    if (strlen($match) < 4) {
      return $options;
    }

    // If the given string begins with the site domain, try to match it to the
    // URL of an issue node.
    if (substr($match, 0, strlen($base_url)) == $base_url) {
      $matches = array();
      // Extract the node ID from the URL, allowing for an anchor tag.
      preg_match("@^$base_url/node/(\d+)(?:#\S+)?\$@", $match, $matches);

      if (isset($matches[1])) {
        $nid = $matches[1];
        $node = node_load($nid);

        if ($node) {
          // Only allow the node if it's of the right type and the user has
          // access to view it.
          if (in_array($node->type, $target_node_types) && node_access('view', $node)) {
            $options[$node->type][$nid] = check_plain($this->getLabel($node));

            // Don't return yet, as there is a slim chance that the URL is part
            // of the title of an issue which starts with the domain name or
            // even the full URL, as in 'http://example.com/node/1 is broken'.
          }
        }
      }
    }

    // If the given string is of the form '#1234' then try to match that as a
    // nid.
    if (strpos($match, '#') === 0) {
      if (preg_match("@^#(\d+)\$@", $match)) {
        $nid = substr($match, 1);
        $node = node_load($nid);

        if ($node) {
          // Only allow the node if it's of the right type and the user has
          // access to view it.
          if (in_array($node->type, $target_node_types) && node_access('view', $node)) {
            $options[$node->type][$nid] = check_plain($this->getLabel($node));

            // Don't return, same reason as above.
          }
        }
      }
    }

    // Build a query for the nodes. We can't use buildEntityFieldQuery() because
    // we have to use a SelectQuery rather than an EntityFieldQuery to have an
    // OR condition.
    $query = db_select('node', 'n');

    if (isset($match)) {
      // Try to match on the title or nid.
      $query->condition(db_or()
        ->condition('n.title', '%' . db_like($match) . '%', 'LIKE')
        ->condition('n.nid', $match)
      );
    }

    // Set the node type.
    $query->condition('type', $target_node_types);

    if (!user_access('bypass node access')) {
      // Restrict the query to published nodes.
      $query->condition('n.status', NODE_PUBLISHED);
    }

    // Restrict the number of returned rows. jQuery UI autocomplete defaults to
    // showing 10 only.
    if (!empty($limit)) {
      $query->range(0, $limit);
    }

    // Order the returned nodes by some sort of relevancy.
    $query->orderBy('n.changed', 'DESC');

    $node_data = $query
      ->fields('n', array('nid', 'title', 'type'))
      ->addTag('node_access')
      ->execute()
      ->fetchAll();

    foreach ($node_data as $item) {
      $options[$item->type][$item->nid] = check_plain($item->title);
    }

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    // We use the same settings as the parent class, therefore it's fine for
    // this to call the parent, as it will produce correct results. It's also
    // fine for methods we don't override such as countReferencableEntities() to
    // call this.
    return parent::buildEntityFieldQuery($match, $match_operator);
  }

}
