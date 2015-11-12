<?php
$feature = $variables['node']->feature;

/* The results from an InterProScan analysis for this feature are avaialble to
 * this template in a array of the following format:
 *
 *     $results[$analysis_id]['iprterms']
 *     $results[$analysis_id]['goterms']
 *     $results[$analysis_id]['analysis']
 *     $results[$analysis_id]['format']
 *
 * Because there may be multiple InterProScan analyses for this feature, they
 * are separated in the array by the $analysis_id key.  The deeper array 
 * structure is as follows
 *
 *     An arrray containing all of the IPR terms mapped to this feature. Each
 *     IPR term is an array with 3 elements. The first element is the IPR
 *     accession, the second is the name and the third is the description
 *       $results[$analysis_id]['iprterms']
 *
 *     A string indicating the XML format from which the original results 
 *     were obtained. Valid values are XML4 or XML5
 *       $results[$analysis_id]['format']
 *
 *     An array of terms, where GO:XXXXXXXX idicates a GO accession that
 *     is used as a key for the array.  All GO terms for all matches are stored here.
 *       $results[$analysis_id]['goterms']['GO:XXXXXXX']['category']
 *       $results[$analysis_id]['goterms']['GO:XXXXXXX']['name']
 *
 *     An array of terms. The variable IPRXXXXXX indicates an IPR accession
 *     that is used as a key for the array.
 *       $results[$analysis_id]['iprterms']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['ipr_name']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['ipr_desc']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['ipr_type']
 *
 *
 *     Each term may have one or more matches.  The variable $j indicates
 *     an index variable for iterating through the matches.
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['match_id']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['match_name']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['match_desc']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['match_dbname']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['evalue']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['score']
 *
 *     An array of terms, where GO:XXXXXXXX idicates a GO accession that
 *     is used as a key for the array.  GO terms are stored a second time
 *     here to associate them with the proper IPR.
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['goterms']['GO:XXXXXXX']['category']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['goterms']['GO:XXXXXXX']['name']
 *
 *     Each match can have multiple start and stop locations. The variable $k
 *     indicates an index variable for iterating through the locations
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_start']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_end']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_score']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_status']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_evalue']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_level']
 *       $results[$analysis_id]['iprterms']['IPRXXXXXX']['matches'][$j]['locations'][$k]['match_evidence']
 */


if (property_exists($feature, 'tripal_analysis_interpro')) {
  if (property_exists($feature->tripal_analysis_interpro->results, 'xml')) {
    
    // iterate through the results. They are organized by analysis id
    $results = $feature->tripal_analysis_interpro->results->xml;
    
    if(count($results) > 0){

      foreach($results as $analysis_id => $details){
        $analysis   = $details['analysis'];
        $iprterms   = $details['iprterms'];
        $format     = $details['format'];

        // ANALYSIS DETAILS
        $aname = $analysis->name;
        if (property_exists($analysis, 'nid')) {
          $aname = l($aname, 'node/' . $analysis->nid, array('attributes' => array('target' => '_blank')));
        }
        $date_performed = preg_replace("/^(\d+-\d+-\d+) .*/", "$1", $analysis->timeexecuted);
        print "
          Analysis Name: $aname
          <br>Date Performed: $date_performed
        ";

        // ALIGNMENT SUMMARY
        $headers = array(
          'IPR Term', 
          'IPR Description', 
          'Source', 
          'Source Term', 
          'Source Description',
          'Alignment'
        );
        
        $rows = array();
        foreach ($iprterms as $ipr_id => $iprterm) {
          
          $matches  = $iprterm['matches'];
          $ipr_name = $iprterm['ipr_name'];
          $ipr_desc = $iprterm['ipr_desc'];
          $ipr_type = $iprterm['ipr_type'];

          // iterate through the evidence matches
          foreach ($matches as $match) {
            $match_id     = $match['match_id'];
            $match_name   = $match['match_name'];
            $match_dbname = $match['match_dbname'];           
          
            $locations = $match['locations'];
            $loc_details = '';            
            foreach($locations as $location){
              if ($format == 'XML4') {
                $loc_details .= 'coord: ' . $location['match_start'] . ".." . $location['match_end'];
                if($location['match_score']) {
                  $loc_details .= '<br>score: ' . $location['match_score'];
                }
              }
              if ($format == 'XML5') {
                $loc_details .= 'coord: ' . $location['match_start'] . ".." . $location['match_end'];
                if($location['match_evalue']) {
                  $loc_details .= '<br>e-value: ' . $location['match_evalue'];
                }
                if($location['match_score']) {
                  $loc_details .= '<br>score: ' . $location['match_score'];
                }
                $loc_details .= '<br>';
              }
              //$match_evidence =  $location['match_evidence'];
            }
            // remove the trailing <br>
            $loc_details = substr($loc_details, 0, -4); 
            
            if ($ipr_id == 'noIPR') {
              $ipr_id_link = 'None';
              $ipr_desc = 'No IPR available';
            }
            else {
              // we want to use the URL for the database
              $ipr_db = tripal_db_get_db(array('name' => 'INTERPRO'));
              $ipr_id_link = $ipr_id;
              if ($ipr_db and $ipr_db->urlprefix) {
                $ipr_id_link = l($ipr_id, $ipr_db->urlprefix . $ipr_id, array('attributes' => array('target' => '_blank')));
              }
            }
            
            // the Prosite databases are split into two libraries for InterProScan. But
            // we can just use the PROSITE database for both of them, so rename it here.
            $match_dbname = preg_replace('/(PROSITE)_.*/', '\1', $match_dbname);
            
            // get links for the matching databases
            $match_db = tripal_db_get_db(array('name' => strtoupper($match_dbname)));
            if ($match_db and $match_db->url) {
              // some databases need a prefix removed
              if ($match_dbname == "GENE3D") {
                $fixed_id = preg_replace('/G3DSA:/','', $match_id);
                $match_id = l($fixed_id, $match_db->urlprefix . $fixed_id, array('attributes' => array('target' => '_blank')));
              }
              elseif ($match_dbname == "SUPERFAMILY") {
                $fixed_id = preg_replace('/SSF/','', $match_id);
                $match_id = l($fixed_id, $match_db->urlprefix . $fixed_id, array('attributes' => array('target' => '_blank')));
              }
              // for all others, just link using the URL prefix
              else {
                $match_id = l($match_id, $match_db->urlprefix . $match_id, array('attributes' => array('target' => '_blank')));
              }
            }
            if ($match_db and $match_db->url) {
              $match_dbname = l($match_dbname, $match_db->url, array('attributes' => array('target' => '_blank')));
            }
            
            $rows[] = array(
              $ipr_id_link,
              $ipr_desc,
              $match_dbname,
              $match_id,
              $match_name,
              array(
                'data' => $loc_details,
                'nowrap' => 'nowrap'
              ),
            );
          } // end foreach ($matches as $match) {
        } // end foreach ($iprterms as $ipr_id => $iprterm) {

        if (count($rows) == 0) {
          $rows[] = array(
            array(
              'data' => 'No results',
              'colspan' => '6',
            ),
          );
        }
        $table = array(
          'header' => $headers,
          'rows' => $rows,
          'attributes' => array(),
          'sticky' => FALSE,
          'caption' => '',
          'colgroups' => array(),
          'empty' => '',
        );
        // once we have our table array structure defined, we call Drupal's theme_table()
        // function to generate the table.
        print theme_table($table);
        print "<br>";
      }
    }
  }
  // for backwards compatibility we want to ensure that if any results are stored
  // as HTML that they can still be displayed.  Although Tripal InterPro Analysis
  // v2.0 no longer supports importing of HTML results.
  if(property_exists($feature->tripal_analysis_interpro->results, 'html') and 
     $feature->tripal_analysis_interpro->results->html) {
    $resultsHTML = $feature->tripal_analysis_interpro->results->html;

    // ANALYSIS DETAILS
    $aname = $analysis_name;
    if (property_exists($analysis, 'nid')) {
      $aname = l($aname, 'node/' . $analysis->nid, array('attributes' => array('target' => '_blank')));
    }
    $date_performed = preg_replace("/^(\d+-\d+-\d+) .*/", "$1", $analysis->timeexecuted);
    print "
      Analysis Name: $analysis_name
      <br>Date Performed: $date_performed
    "; ?>
    
    <div class="tripal_feature-interpro_results_subtitle">Summary of Annotated IPR terms</div> <?php 
    print $resultsHTML;?>
    </div> <?php 
  }
}