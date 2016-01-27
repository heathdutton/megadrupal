<?php

/**
 * Implements preprocess hook for the tripal_organism_unigenes template.
 */
function tripal_analysis_unigene_preprocess_tripal_organism_unigenes(&$variables) {
  $node = $variables['node'];
  $organism = $node->organism;

  // get information about this assemblies and add it to the items in this node
  $sql = "
     SELECT *
     FROM {organism_unigene_mview} OUM
       INNER JOIN {analysis} A  ON A.analysis_id = OUM.analysis_id
     WHERE OUM.organism_id = :organism_id
     ORDER BY A.timeexecuted DESC
   ";
  $results = chado_query($sql, array(':organism_id' => $organism->organism_id));

  $unigenes = array();
  $i=0;
  $sql = "SELECT nid FROM {chado_analysis} WHERE analysis_id = :analysis_id";
  while ($unigene = $results->fetchObject()) {
    $analysis_id = $unigene->analysis_id;
    $c_node = db_query($sql, array(':analysis_id' => $analysis_id))->fetchObject();
    if ($c_node) {
      $unigene->nid = $c_node->nid;
    }
    // add in the properties
    $unigene_name = tripal_analysis_get_property($analysis_id, 'analysis_unigene_name');
    $num_contigs  = tripal_analysis_get_property($analysis_id, 'analysis_unigene_num_contigs');
    $num_reads    = tripal_analysis_get_property($analysis_id, 'analysis_unigene_num_reads');
    $num_clusters = tripal_analysis_get_property($analysis_id, 'analysis_unigene_num_clusters');
    $num_singlets = tripal_analysis_get_property($analysis_id, 'analysis_unigene_num_singlets');

    $unigene->unigene_name = is_object($unigene_name) ? $unigene_name->value : '';
    $unigene->num_reads = is_object($num_reads) ? $num_reads->value : '';
    $unigene->num_clusters = is_object($num_clusters) ? $num_clusters->value : '';
    $unigene->num_contigs = is_object($num_contigs) ? $num_contigs->value : '';
    $unigene->num_singlets = is_object($num_singlets) ? $num_singlets->value : '';

    $unigenes[$i++] = $unigene;
  }
   
  $node->organism->tripal_analysis_unigene->unigenes = $unigenes;
}

/**
 * Implements preprocess hook for the tripal_feature_unigenes template.
 */
function tripal_analysis_unigene_preprocess_tripal_feature_unigenes(&$variables) {
  $node = $variables['node'];
  $feature = $node->feature;
   
  // first get all the unigene analyses for this organism
  $sql = "
    SELECT * FROM {organism_unigene_mview} OUM 
      INNER JOIN {analysis} A  ON A.analysis_id = OUM.analysis_id 
    WHERE OUM.organism_id = :organism_id 
    ORDER BY A.timeexecuted DESC
  ";
  $results = chado_query($sql, array(':organism_id' => $feature->organism_id->organism_id));

  // iterate through the unigenes and find those that use this feature
  $unigenes = array();
  $i=0;
  $sql = "SELECT nid FROM {chado_analysis} WHERE analysis_id = :analysis_id";
  while ($unigene = $results->fetchObject()) {
    $analysis_id = $unigene->analysis_id;

    // check if this feature is present in the unigene
    $values = array(
      'feature_id' => $feature->feature_id,
      'analysis_id' => $analysis_id,
    );
    $analysisfeature = chado_select_record('analysisfeature', array('*'), $values);

    // if the feature is present then get information about it
    if (sizeof($analysisfeature) > 0) {
      // see if there is a drupal node for this unigene
      $c_node = db_query($sql, array(':analysis_id' => $analysis_id))->fetchObject();
      if ($c_node) {
        $unigene->nid = $c_node->nid;
      }
      // add in the properties
      $unigene_name = chado_get_property(array('table' => 'analysis', 'id' => $analysis_id), array('type_name' => 'analysis_unigene_name', 'cv_name' => 'tripal'));
      $singlet = chado_get_property(array('table' => 'analysisfeature', 'id' => $analysisfeature[0]->analysisfeature_id), array('type_name' => 'singlet', 'cv_name' => 'tripal'));
       
      if ($unigene_name) {
        $unigene->unigene_name = $unigene_name->value;
      }
      if ($singlet) {
        $unigene->singlet = $singlet->value;
      }

      $unigenes[$i++] = $unigene;
    }
  }

  $node->feature->tripal_analysis_unigene->unigenes = $unigenes;
}
