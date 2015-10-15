<?php

/**
 * @file
 * Plugin file for the DBpedia Web Taxonomy
 */

class WtDbpedia extends WebTaxonomy {

  /**
   * Implements WebTaxonomy::autocomplete().
   *
   * Uses the DBpedia lookup service.
   * API documentation see http://wiki.dbpedia.org/lookup/.
   */
  public function autocomplete($string = '') {
    // Build the query.
    $url = 'http://lookup.dbpedia.org/api/search.asmx/PrefixSearch';
    // QueryClass: a DBpedia class from the ontology that the results should
    // have. (CAUTION: specifying any values that do not represent a DBpedia
    // class will lead to no results.)
    // MaxHits: the maximum number of returned results.
    $class = '';
    $params = array(
      'QueryClass' => $class,
      'MaxHits' => '10',
      'QueryString' => $string,
    );

    // Get and parse the XML query result.
    $http = http_client();
    $data = $http->get($url, $params);
    $result_array = new SimpleXMLElement($data);

    // Build the data structure WebTaxonomy::autocomplete() expects.
    $term_info = array();
    foreach ($result_array->Result as $result) {
      $term_name = (string) $result->Label;
      $uri = (string) $result->URI;
      $term_info[$term_name] = array(
        'name' => $term_name,
        'web_tid' => $uri,
      );
    }
    return $term_info;
  }

  /**
   * Implements WebTaxonomy::fetchTerm().
   *
   * Reads label information from LOD representation in JSON format.
   *
   * For resource http://dbpedia.org/resource/Verona, the JSON file
   * is http://dbpedia.org/data/Verona.json.
   * The json data contains the properties of the resource under the
   * resource uri as a key.
   */
  public function fetchTerm($term) {
    $web_tid = $term->web_tid[LANGUAGE_NONE][0]['value'];
    if (!empty($web_tid)) {
      $entity = substr($web_tid, strrpos($web_tid, '/') + 1);
      $url = 'http://dbpedia.org/data/' . $entity . '.json';
    }
    else {
      // @todo If there is no Web Tid, check by name.
    }

    $http = http_client();
    $data = $http->get($url);

    $result = json_decode($data);
    $labels = $result->$web_tid->{'http://www.w3.org/2000/01/rdf-schema#label'};

    $term_info = array();
    foreach ($labels as $label) {
      if ($label->lang == 'en') {
        $term_name = $label->value;
        $term_info[$term_name] = array(
          'name' => $term_name,
          'web_tid' => $web_tid,
        );
      }
    }
    return $term_info;
  }
}
