<?php

/**
 * @file
 * Module API documentation.
 */

/**
 * @defgroup dsb_portal_example dsb Portal Customization Example
 * @{
 * The dsb Portal module provides sensible defaults to searching the national
 * catalog. However, many content partners actually require different behaviors.
 * This module provides a concrete example of how content partners can alter
 * the search form and description viewing to suite their own needs. The
 * alterations are based on actual requests received from content partners.
 *
 * @section search_form Altering the search form
 *
 * By default, the dsb Portal module only requests the learningResourceType
 * facets. Many portals actually want more facets than that. dsb Portal
 * Customization Example adds two more, which are very common:
 * - educaSchoolLevels
 * - educaSchoolSubjects
 *
 * This is done through dsb_portal_example_dsb_portal_rest_api_facets_alter(),
 * which is an implementation of hook_dsb_portal_rest_api_facets_alter().
 *
 * Another frequent request is to alter the name of the facet group. By default,
 * dsb Portal retrieves the human-readable names of the facets from the
 * Ontology Server. This is a central repository of all human interface text and
 * labels. This is the official source for all facet labels as well. However,
 * some of the labels are overly verbose. Many content partners prefer shorter
 * labels. dsb Portal Customization Example changes these facet labels through
 * dsb_portal_example_form_dsb_portal_rest_api_search_form_alter().
 *
 * @section search_behavior Search behavior
 *
 * A common request is to only show the descriptions for certain content
 * partners. This can be done by altering the filters that are sent to the
 * REST API. It is important to distinguish between filters that should always
 * be active, and filters that should be enabled only if the user does not
 * provide a value. For instance, if the search should only show results for
 * specific content partners, and a user wishes to filter by only one content
 * partner, the filter should not be expanded to include other content partners,
 * as the API treats multiple values as OR values, not AND.
 *
 * In dsb_portal_example_dsb_portal_rest_api_filters_alter(), which implements
 * hook_dsb_portal_rest_api_filters_alter(), a check is made. If there's no
 * ownerUsername filter set (content partner filter), it adds a list of
 * content partners. Only results for these content partners will show up.
 *
 * Another use-case would be to limit results to a certain school context. For
 * instance, only allow descriptions that relate to Compulsory Education. This
 * is achieved in 2 parts: first, all checkboxes that relate to different
 * educational contexts have to be disabled. This is also done through
 * dsb_portal_example_form_dsb_portal_rest_api_search_form_alter(). The second
 * step is to always enable at least the compulsory education filter for all
 * requests, while making sure we don't add it twice. This is also done in
 * dsb_portal_example_dsb_portal_rest_api_filters_alter().
 *
 * @}
 */
