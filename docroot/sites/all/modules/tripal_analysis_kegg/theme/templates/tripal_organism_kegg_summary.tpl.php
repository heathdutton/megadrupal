<?php
$organism = $node->organism;
$form = $organism->tripal_analysis_kegg->select_form['form'];
$has_results = $organism->tripal_analysis_kegg->select_form['has_results'];

if($has_results) {
  print drupal_render($form); ?>
  <div id="tripal_analysis_kegg_org_report"></div> <?php
}
  
$message = 'Administrators, if there are no KEGG reports available, you must:
    <ul>
      <li>Create a ' . l('KEGG analysis page', 'node/add/chado-analysis-kegg', array('attributes' => array('target' => '_blank'))) . '</li>
      <li>Populate the ' . l('kegg_by_organism','admin/tripal/schema/mviews', array('attributes' => array('target' => '_blank'))) . ' materialized view</li>
      <li>Ensure the user ' . l('has permission','admin/people/permissions', array('attributes' => array('target' => '_blank'))) . 'to view the KEGG analysis content</li>
      <li>Refresh this page</li>
    </ul>';

print tripal_set_message($message, TRIPAL_INFO, array('return_html' => 1)); 
