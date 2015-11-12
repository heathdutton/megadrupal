<?php
/**
 * @file
 *
 * Filter to exclude specified facet items from being shown on the facet
 *
 * This is heavily inspired by the excellent blog post from Trellon
 * http://www.trellon.com/content/blog/apachesolr-and-facetapi
 */

/**
 * @see: facetapi_realm_settings_form_submit
 */

/**
 * Plugin that excludes specified facet items.
 */
class FacetapiFilterExcludeDomains extends FacetapiFilter {

  /**
   * Filters facet items.
   */
  public function execute(array $build) {
    // FacetAPI just parses results returned by Solr and therefore doesn't
    // have a lot of knowledge about the data-structures that are indexed
    // or the Query itself.
    // Therefore we first load the user's accessible Domains with their
    // properties.
    // Then we use those to reduce the facets.

    // The domain property exposed to SearchAPI/FacetAPI has a 'domain_'
    // prefix. Get the original property name use by Domain Access.
    $domain_property_key = $this->settings->facet;
    if (strpos($domain_property_key, 'domain_') === FALSE) {
      // Return original facet if 'domain_' property is not found in build.
      // This way it makes it save to enable this filter on all facets without
      // throwing error when executing this filter.
      return $build;
    }
    if ($domain_property_key == 'domain_domain_site') {
      // Return original facet for the 'domain SendToAll' field.
      return $build;
    }
    $domain_property_key = preg_replace('/domain_/', '', $domain_property_key, 1);


    // Get domains the user is assigned to.
    global $user;
    $user_domains = domain_get_user_domains($user);

    // Always add the current_domain.
    $current_domain = domain_get_domain();
    $current_domain = array(
      $current_domain['domain_id'] => $current_domain['domain_id']
    );

    // Normalize to only unique values.
    $user_accessible_domains = array_unique($user_domains + $current_domain);
    foreach ($user_accessible_domains as &$id) {
      $id = domain_load($id);
    }


    // Create an array with values that match the facet keys.
    $user_accessible_domains_by_property_value = array();
    foreach ($user_accessible_domains as $domain_id => $domain_properties) {
      $user_accessible_domains_by_property_value[$domain_id] = $domain_properties[$domain_property_key];
    }


    // Reduce results.
    $filtered_build = array();
    foreach ($build as $key => $item) {
      if (in_array($key, $user_accessible_domains_by_property_value)) {
        $filtered_build[$key] = $item;
      }
    }

    return $filtered_build;
  }
}
