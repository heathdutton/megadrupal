<?php

/**
 * Define SKOS Vocabulary
 */
class SkosVoc {

  public $sparql_client;

  function __construct(EasyRdf_Sparql_Client $sparql_client) {
    $this->sparql_client = $sparql_client;
  }

  function query($sparql_query) {
    return $this->sparql_client->query($sparql_query);
  }

  function getSearchTermsbyLabel($search_string, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::SearchTermsbyLabel($search_string, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getTopAllConceptOf($lang = 'en') {
    $sparql_query = SkosSparqlQuery::topAllConceptOf($lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getTopConceptOf($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::topConceptOf($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getBottomConceptOf($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::bottomConceptOf($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getBroaderTermsByURI($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::broaderTermsByURI($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getNarrowerTermsByURI($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::narrowerTermsByURI($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getRelatedTermsByURI($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::relatedTermsByURI($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getRelatedTermsBySubPropertyOfByURI($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::relatedTermsBySubPropertyOfByURI($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }

  function getDefinitionByURI($uri, $lang = 'en') {
    $sparql_query = SkosSparqlQuery::definitionByURI($uri, $lang);
    $rows = self::query($sparql_query);
    return $rows->numRows() > 0 ? $rows : NULL;
  }


}

/**
 * Just define SKOS SPARQL Queries
 */
class SkosSparqlQuery {

  static function SearchTermsbyLabel($search_string, $lang) {
    $query =
     'SELECT DISTINCT ?uri ?label
      WHERE {
        {?uri skos:prefLabel ?label .
        FILTER( (regex(str(?label), "^%s", "i")) && (lang(?label)= "%s"))
        }
        UNION
        {?uri skos:altLabel ?label .
        FILTER( (regex(str(?label), "^%s", "i")) && (lang(?label)= "%s"))
        }
      }
      ORDER BY DESC(?label)';
    return sprintf($query, $search_string, $lang, $search_string, $lang);
  }

  static function topAllConceptOf($lang = 'en') {
    $sparql_query =
      "SELECT ?uri ?label {
        ?uri skos:topConceptOf ?o.
        ?o rdf:type skos:ConceptScheme.
        ?uri skos:prefLabel ?label
        FILTER(lang(?label) = '%s')
      }";
    return sprintf($sparql_query, $lang);;
  }

  static function topConceptOf($uri, $lang = 'en') {
    $sparql_query =
      "SELECT ?uri ?label {
       ?s skos:broader ?uri.
       ?uri skos:prefLabel ?label.
       FILTER(str(?s) = '%s' && lang(?label) = '%s')
      }";
     return sprintf($sparql_query, $uri, $lang);
  }

  static function bottomConceptOf($uri, $lang = 'en') {
    $sparql_query =
      "SELECT ?uri ?label {
      ?uri skos:broader ?o.
      ?uri skos:prefLabel ?label
      FILTER (str(?o) = '%s' && lang(?label) = '%s')
      }
      ";
     return sprintf($sparql_query, $uri, $lang);
  }

  static function broaderTermsByURI($uri, $lang = 'en') {
    $sparql_query =
      "SELECT DISTINCT ?uri ?label
        WHERE {
        ?termuri skos:broader ?uri .
        ?uri skos:prefLabel ?label .
        FILTER (str(?termuri) = '%s' && (lang(?label) = '%s') )
      }";
     return sprintf($sparql_query, $uri, $lang);
  }

  static function narrowerTermsByURI($uri, $lang = 'en') {
    $sparql_query =
      "SELECT DISTINCT ?uri ?label
        WHERE {
        ?termuri skos:narrower ?uri .
        ?uri skos:prefLabel ?label .
        FILTER (str(?termuri) = '%s' && (lang(?label) = '%s') )
      }";
     return sprintf($sparql_query, $uri, $lang);
  }

  static function relatedTermsByURI($uri, $lang = 'en') {
    $sparql_query =
      "SELECT DISTINCT ?uri ?label
        WHERE {
        ?termuri skos:related ?uri .
        ?uri skos:prefLabel ?label .
        FILTER (str(?termuri) = '%s' && (lang(?label) = '%s') )
      }";
     return sprintf($sparql_query, $uri, $lang);
  }

  static function definitionByURI($uri, $lang = 'en') {
    $sparql_query =
      "SELECT ?uri ?label
       WHERE {
       ?uri skos:definition ?definitionUri .
       ?definitionUri rdf:value ?label .
       FILTER (str(?uri) = '%s' && (lang(?label)= '%s') ) }";
     return sprintf($sparql_query, $uri, $lang);
  }

  /**
   * Includes a special query to retrieve agront properties that are subproperties of skos:related
   * It works mainly for AGROVOC
   * @param  [type] $uri  [description]
   * @param  string $lang [description]
   * @return [type]       [description]
   */
  static function relatedTermsBySubPropertyOfByURI($uri, $lang = 'en') {
    $sparql_query =
      "SELECT DISTINCT ?uri ?subproperty ?label
        WHERE {
        ?subproperties rdfs:subPropertyOf skos:related.
        ?subproperty ?p ?subproperties.
        ?s ?subproperty ?uri.
        ?uri skos:prefLabel ?label.
        FILTER (str(?s) = '%s' && (lang(?label) = '%s') )
      }";
     return sprintf($sparql_query, $uri, $lang);
  }
}











