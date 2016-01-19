<?php
   
/**
* New display Page plugin to use in Views when querying a OSF instance.
* We do need that new display plugin to modify the default user interface
* to accomodate the specificities of a View that uses OSF as a backend.
*/
class osf_views_views_plugin_display_page extends views_plugin_display_page {
  /**
   * Provide the default summary for options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);
    
    // Get the OSF network URL to use for this view.
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "Select a OSF network");
    
    // Change the name of the setting in the Views user interface (avanced section).
    $options['query']['title'] = "OSF settings";
    $options['query']['value'] = "($network)";
  }

  function get_pager_text() {
    return array(
      'items per page title' => t('Items per page'),
      'items per page description' => t('The number of items to display per page. Enter 0 for no limit.')
    );
  }
}
