<?php

/**
 * Class FacetapiUrlProcessorValidator.
 */
class FacetapiUrlProcessorValidator extends FacetapiUrlProcessorStandard {
  /**
   * Implements FacetapiUrlProcessor::normalizeParams().
   *
   * Strips the "q" and "page" variables from the params array.
   */
  public function normalizeParams(array $params, $filter_key = 'f') {
    $params = parent::normalizeParams($params, $filter_key);

    if (isset($params[$filter_key]) && is_array($params[$filter_key])) {
      $params[$filter_key] = $this->validateParams($params[$filter_key]);
    }

    return $params;
  }

  protected function validateParams(array $params) {
    // $params has come from the URL, so we'll assume that it's broken.
    $type_map = $this->computeFacetAliasesAndTypes();
    $validators = facetapi_url_validator_type_validation_map();
    foreach ($params as $key => $param) {
      // Is this in the format that we expect.
      if (strpos($param, ':')) {
        // Try to parse as a facet.
        list($facet_alias, $value) = explode(':', $param, 2);
        $facet_alias = rawurldecode($facet_alias);
        // Now do some validation.
        if (isset($type_map[$facet_alias]) && isset($validators[$type_map[$facet_alias]])) {
          // Detect and handle range queries differently.
          if (strpos($value, ' TO ') && preg_match(FACETAPI_REGEX_RANGE, $value, $matches)) {
            // This is a range query.
            $start = $matches[1];
            $end = $matches[2];
            if (
            ($start != '*' && !$validators[$type_map[$facet_alias]]($start))
            ||
            ($end != '*' && !$validators[$type_map[$facet_alias]]($end))
            ) {
              unset($params[$key]);
            }
          }
          // Simple values are easier.
          else {
            if (!($validators[$type_map[$facet_alias]]($value))) {
              unset($params[$key]);
            }
          }

        }
      }

    }
    return $params;
  }

  protected $aliasesTypesMap;
  protected function computeFacetAliasesAndTypes() {
    if (is_null($this->aliasesTypesMap)) {
      $this->aliasesTypesMap = array();
      $facets = $this->adapter->getEnabledFacets();
      foreach ($facets as $facet) {
        if (isset($facet['url validation type'])) {
          $this->aliasesTypesMap[$facet['field alias']] = $facet['url validation type'];
        }
        else if (isset($facet['field type'])) {
          $this->aliasesTypesMap[$facet['field alias']] = $facet['field type'];
        }
      }
    }
    return $this->aliasesTypesMap;
  }

}