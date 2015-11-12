<?php

class loc extends Metadata {

  // Regular expression to determine if an URL in a MARC 856 tag leads to a
  //   LOC page with metadata.
  // Example: $regex = "/loc.gov.catdir.toc|loc.gov.catdir.enhancements.*-t.html/";
  public $regex = "";

  // Header to show on node view
  public $viewHeader = ""; // Example: "Table of contents"

  // Weight of $node->content element on node view
  public $viewWeight = 0;

  // cacheLifetime: number of seconds after which data is considered stale.
  // -1 for "never", 0 = always
  protected $cacheLifetime = -1;

  // Number of maximum simultaneous connections for harvest
  protected $maxConnections = 20;

  public function calculateHarvestJobSingle($nid, $subject) {
    $harvestJob = array();

    // Get URLs from marc
    $url = $this->determineUrl($subject->millennium_biblio_data['marc']);

    if ($url) {
      $harvestJob[] = array(
        'nid' => $nid,
        'request' => array(
          'url' => $url,
          ),
        'callback' => 'extract'
      );
    }

    return $harvestJob;
  }

  // Hook into hook_nodeapi($op = 'view')
  public function nodeView(&$node, $view_mode) {
    if (!$node) {
      return;
    }
    if ($view_mode == 'full') {
      $metadata = $this->get($node->nid);
      #dpm($node);

      if ($metadata && $metadata->data) {
        $output = '<div class="millennium-enrich"><h4>' . t($this->viewHeader) . '</h4>';
        $output .= '<div class="content">' . $metadata->data . '</div></div>';
        $node->content['millennium_loc_data_' . $this->sourceId] = array(
          '#markup' => $output,
          '#weight' => $this->viewWeight,
        );
      }
    }
  }

  // Callback function to extract metadata
  // Generic extractor for LOC TOC, Bio, other pages
  public function extract($raw_respose) {
    $ok = preg_match('/<pre>(.*?)<\/pre>/si', $raw_respose, $matches);
    if (!$ok) {
      // Try another one
      $ok = preg_match('/<hr>(.*?)(<hr>|<\/body>)/si', $raw_respose, $matches);
      if (!$ok) {
        // TODO! Debug, ¿should be watchdog?
        #dpm("Could not parse!");
        #dpm(htmlspecialchars($raw_response));
        return FALSE;
      }
    }
    $result = trim(strip_tags(decode_entities(drupal_convert_to_utf8($matches[1], 'UTF-8'))));
    if (strlen($result) < 10) {
      #dpm("Could not parse!");
      #dpm($raw_response);
      return FALSE;
    }

    // Try to detect if this is preformatted text: more than 80% of newlines are inside "result"
    if (substr_count($result, "\n") > substr_count($raw_respose, "\n") * 0.8 && strip_tags($result) == $result) {
      $result = preg_replace("/[\r\n]+/", "\n", $result);
      $result = nl2br($result);
    }
    return $result;
  }

  // Returns an url to mine for metadata
  public function determineUrl($marc_text) {
    $url = "";

    $marc = millennium_parse_marc($marc_text);

    // Process links from 856
    $tmpfields = millennium_getFields($marc, "856..");
    if (!is_array($tmpfields)) {
      return;
    }
    foreach ($tmpfields as $field) {
      $tmp = millennium_getSubfields($field, "u");
      if (sizeof($tmp) == 0) {
        continue;
      }
      list($tmpsub) = $tmp;
      $link_url = preg_replace("/ /", "", $tmpsub["data"]);
      #list($tmpsub) = millennium_getSubfields($field, "z");
      #$link_text = $tmpsub["data"];
      #if ($link_text == "") {
      #  list($tmpsub) = millennium_getSubfields($field, "3");
      #  $link_text = $tmpsub["data"];
      #}

      if (preg_match($this->regex, $link_url)) {
        $url = $link_url;
        #drupal_set_message("Found URL: $url");
        return $url;
        break;
      }
    }
    return FALSE;
  }

}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function millennium_enrich_loc_load_biblio_data_alter(&$biblio, $node) {

}
