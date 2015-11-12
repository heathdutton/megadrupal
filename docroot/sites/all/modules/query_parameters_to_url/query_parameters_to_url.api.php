<?php
/**
 * @file
 * API documentation for Query Parameters to URL module.
 */

/**
 * Given path and contextual data, decide if path rewriting should be done.
 *
 * Might be called from hook_url_inbound_alter() or hook_url_outbound_alter().
 * Depending on that, passed contextual data can differ.
 *
 * @param $path String The path to rewrite.
 * @param $options Array An array of url options in outbound alter.
 * @param $original_path String|Null The original path before altering.
 * @param $path_language String|Null The path language in inbound alter.
 *
 * @return string
 */
function hook_query_parameters_to_url_rewrite_access($path, $options, $original_path, $path_language) {
  // Allow path rewriting on the special dashboard page.
  if ($path == 'special-dashboard') {
    return QUERY_PARAMETERS_TO_URL_REWRITE_ALLOW;
  }

  // Deny rewriting on the events page, if a special query parameter is set.
  if ($path == 'events' && isset($options['query']['special_parameter']) && $options['query']['special_argument'] == TRUE) {
    return QUERY_PARAMETERS_TO_URL_REWRITE_DENY;
  }

  // Ignore otherwise.
  return QUERY_PARAMETERS_TO_URL_REWRITE_IGNORE;
}

/**
 * Allow rewriting the final URLs containing encoded query parameters.
 *
 * Called from hook_url_inbound_alter() and from hook_url_outbound_alter().
 * If you want to modify the final urls and query parameters, make sure to
 * check for both inbound and outbound direction.
 *
 * In the outbound case you will most probably want to modify the
 * $context['options']['query'] array, which contains the query parameters
 * that will be encoded into the URL. You can alter both the values and the key.
 *
 * In the inbound case you will probably want to modify the
 * $context['extracted_query_parameters'] array, which contains the decoded
 * query parameters from the initial inbound URL.
 *
 * @param $path String The path that is being rewritten depending on the hook:
 *  - hook_url_outbound_alter(): The original path before it will have the query
 *    parameters appended to it in encoded form.
 *  - hook_url_inbound_alter(): The final decoded path, which does not contain
 *    the encoded query parameters.
 * @param $context Array An associative array with additional options depending
 *  on the called hook:
 *  - called from hook_url_outbound_alter(), will have the following keys:
 *    - direction - the string 'outbound' so you can use it in conditionals.
 *    - options - the options array passed to the outbound hook. This contains
 *      the 'query' key with the parameters which you will want to change.
 *    - original_path - the original_path passed to the outbound hook.
 *    - new_path_without_parameters - the new path prefix that will be used
 *      together with the encoded parameters string.
 *  - called from hook_url_inbound_alter(), will have the following keys:
 *    - direction - the string 'inbound' so you can use it in conditionals.
 *    - original_path - the original_path passed to the inbound hook.
 *    - path_language - the path_language passed to the inbound hook.
 *    - extracted_query_parameters - the array of query parameters that were
 *      decoded from the inbound url.
 *    - request_uri_path - the request uri path which will be assigned to the
 *      $SERVER['REQUEST_URI'] variable.
 *    - initial_url_components - an array of url components exploded by '/',
 *      of the original inbound url, for informational purposes.
 */
function hook_query_parameters_to_url_rewrite_alter(&$path, &$context) {
  // On Apache Solr Search page, replace facet query parameter, to a shorter
  // string.
  if (strpos($path, 'search/site') !== FALSE) {
    if ($context['direction'] == 'outbound') {
      if (isset($context['options']['query']['f'])) {
        foreach ($context['options']['query']['f'] as &$value) {
          if ($value == 'bundle:event') {
            $value = 'event';
          }
        }
      }
    }
    elseif ($context['direction'] == 'inbound') {
      if (isset($context['extracted_query_parameters']['f'])) {
        foreach ($context['extracted_query_parameters']['f'] as &$value) {
          if ($value == 'event') {
            $value = 'bundle:event';
          }
        }
      }
    }
  }

  // On an events views page, replace exposed filter node ids with cleaned node
  // names. This might not work if nodes contain any characters except
  // alpha-numeric ones. This will also affect performance negatively, because
  // of node_loads on each outbound hook invocation.
  // This is just a proof of concept how to replace ids with node titles, and is
  // not full proof. You should develop your own, based on your own needs.
  if (strpos($path, 'events') !== FALSE) {
    if ($context['direction'] == 'outbound') {
      if (isset($context['options']['query']['og_group_ref_target_id'])) {
        // Load nodes by ids.
        $nodes = node_load_multiple($context['options']['query']['og_group_ref_target_id']);
        foreach ($context['options']['query']['og_group_ref_target_id'] as $key => $value) {
          if (isset($nodes[$value])) {
            // Replace the id with a node title.
            $value = str_replace(' ', '-', trim($nodes[$value]->title));

            // Rename the query parameter name into something nicer and shorter.
            $context['options']['query']['ref'][$key] = $value;
            unset($context['options']['query']['og_group_ref_target_id']);
          }
        }
      }
    }
    elseif ($context['direction'] == 'inbound') {
      if (isset($context['extracted_query_parameters']['ref'])) {
        // Replace all the title dashes with spaces, to restore to actual node
        // title.
        $node_titles = array();
        foreach ($context['extracted_query_parameters']['ref'] as &$value) {
          $node_titles[] = str_replace('-', ' ', $value);
        }

        // Load the nodes by title.
        $conditions = array('title' => $node_titles);
        $nodes = node_load_multiple(NULL, $conditions);

        if (!empty($nodes)) {
          $nodes_by_url = array();

          // Make an associative array indexed by the node title url components
          // and with the value being set to the node id.
          foreach ($nodes as $key => $node) {
            $nodes_by_url[str_replace(' ', '-', trim($node->title))] = $node->nid;
          }

          // Replace the query parameter values with actual node ids.
          foreach ($context['extracted_query_parameters']['ref'] as $key => &$value) {
            if (isset($nodes_by_url[$value])) {
              $context['extracted_query_parameters']['og_group_ref_target_id'][$key] = $nodes_by_url[$value];
              unset($context['extracted_query_parameters']['ref'][$key]);
            }
          }
        }
      }
    }
  }
}
