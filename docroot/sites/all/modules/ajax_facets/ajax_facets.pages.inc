<?php

/**
 * @file
 * User page callbacks for the ajax_facets module.
 */

/**
 * Callback to update facets content by AJAX.
 */
function ajax_facets_refresh_facets_content() {
  if (!empty($_POST['ajax_facets'])) {
    // Dirty hack to avoid error 414 in $_GET query.
    // @see class FacetapiUrlProcessorStandard->fetchParams();
    $_GET = array_merge($_GET, $_POST['ajax_facets']);

    // Reset $_POST, because html ids, can break ids of facets for update.
    unset($_POST['ajax_html_ids']);
    unset($_POST['ajax_facets']);

    $name = $_GET['view_name'];
    $display_id = $_GET['view_display_id'];
    $view_args = isset($_GET['view_args']) && $_GET['view_args'] !== '' ? explode('/', $_GET['view_args']) : array();
    $view_path = isset($_GET['view_path']) ? rawurldecode($_GET['view_path']) : NULL;

    // Execute view query to fill facets.
    $view = views_get_view($name);
    // Fix 'q' for paging.
    if (!empty($view_path)) {
      $_GET['q'] = $view_path;
    }
    // Reuse dom id.
    $view->dom_id = isset($_GET['view_dom_id']) ? preg_replace('/[^a-zA-Z0-9_-]+/', '-', $_GET['view_dom_id']) : NULL;
    $page = $view->preview($display_id, $view_args);

    $searcher = 'search_api@' . $_GET['index_id'];
    $adapter = facetapi_adapter_load($searcher);
    $blocks = ajax_facets_process_facet_blocks($searcher);
    $facet_query = !empty($_GET['f']) ? $_GET['f'] : '';

    $result = array(
      'applyUrl' => ajax_facets_facet_build_apply_path($adapter),
      'resetUrls' => $blocks['reset_urls'],
      'newContent' => $blocks['facet_blocks'],
      'hideBlocks' => $blocks['hide_blocks'],
      'activeItems' => $blocks['active_items'],
      'views_content' => $page,
      'display_id' => $display_id,
      'views_name' => $name,
      'facets' => $facet_query,
      'update_results' => $blocks['update_results'],
      'total_results' => $view->total_rows,
      'settings' => array(),
    );

    // @see template_preprocess_views_view();
    // Update settings when view will be reloaded. Specifically $view->dom_id.
    if ($view->use_ajax) {
      $result['settings'] = array(
        'views' => array(
          'ajax_path' => url('views/ajax', array('query' => array('f' => $facet_query))),
          'ajaxViews' => array(
            'views_dom_id:' . $view->dom_id => array(
              'view_name' => $view->name,
              'view_display_id' => $view->current_display,
              'view_args' => check_plain(implode('/', $view->args)),
              'view_path' => $view_path,
              // Pass through URL to ensure we get e.g. language prefixes.
              'view_base_path' => $view->get_path(),
              'view_dom_id' => $view->dom_id,
              // To fit multiple views on a page, the programmer may have
              // overridden the display's pager_element.
              'pager_element' => isset($view->query->pager) ? $view->query->pager->get_pager_id() : 0,
            ),
          ),
        ),
      );
    }

    $commands = array();

    // Render exposed form separately if needed. @see https://www.drupal.org/node/2034029#comment-10211699
    if (!empty($view->display[$view->current_display]->display_options['exposed_block'])) {
      $exposed_form = $view->display_handler->get_plugin('exposed_form');
      $result['exposed_form'] = $exposed_form->render_exposed_form(TRUE);
    }

    $commands[] = array(
      'command' => 'ajax_facets_update_content',
      'data' => $result,
    );

    // Allow other modules to add, change or delete commands.
    drupal_alter('ajax_facets_data', $commands, $view);
    print ajax_render($commands);
    drupal_exit();
  }
}
