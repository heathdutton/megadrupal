<?php

$organism = $node->organism;
$analysis_select_form = $organism->tripal_analysis_go->select_form['form'];
$has_results = $organism->tripal_analysis_go->select_form['has_results'];

if ($has_results) {
  print $analysis_select_form; ?>
  <div id="tripal_analysis_go_org_charts"></div>    
  <div id="tripal_cv_cvterm_info_box">
    <a href="#" onclick="$('#tripal_cv_cvterm_info_box').hide()" style="float: right">Close [X]</a>
    <div>Term Information</div>
    <div id="tripal_cv_cvterm_info"></div>
  </div>
  <div id="tripal_ajaxLoading" style="display:none">
    <div id="loadingText">Loading...</div>
  </div> <?php
} 
else { 
  $message = 'Administrators, to view a GO report you must:
    <ul>
      <li>Load GO assignments. GO assignments can be provided within a GFF3 file and loaded using the ' .
          l('GFF loader', 'admin/tripal/loaders/gff3_load', array('attributes' => array('target' => '_blank'))) . '
          or in a GAF format file and loaded using the ' .
          l('GAF loader', 'admin/tripal/loaders/gaf_load', array('attributes' => array('target' => '_blank'))) . '.
          Tools such as Blast2GO are capable of assigning GO terms to features and generating GAF files. 
          Additionally, the Tripal InterProScan Extension module parses 
          InterProScan XML output and imports GO terms associated with features. 
          The Tripal InterProScan Extension Module is obtained separately from the 
          core Tripal package.</li>
      <li>Set the ' . l('cvtermpath', 'admin/tripal/tripal_cv/cvtermpath', array('attributes' => array('target' => '_blank'))) . '
          for the biological process, molecular_function and cellular_component vocabularies. 
          This should have been done automatically when the Gene Ontology was first loaded.</li>
      <li>Populate the ' . l('go_count_analysis', 'admin/tripal/schema/mviews', array('attributes' => array('target' => '_blank'))) . ' 
          materialized view</li>
      <li>Ensure the user ' . l('has permission', 'admin/people/permissions', array('attributes' => array('target' => '_blank'))) . '
          to view the GO analysis content</li>
    </ul>
  ';
  print tripal_set_message($message, TRIPAL_INFO, array('return_html' => 1));

}
