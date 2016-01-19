<?php
/**
 * @file
 * user_priority_ranking_example.features.node_export_features.inc
 */

/**
 * Implements hook_node_export_features_default().
 */
function user_priority_ranking_example_node_export_features_default() {
  $node_export = array(
  'code_string' => 'array(
  (object) array(
      \'vid\' => \'1\',
      \'uid\' => \'1\',
      \'title\' => \'Drupal Mousepad\',
      \'log\' => \'\',
      \'status\' => \'1\',
      \'comment\' => \'0\',
      \'promote\' => \'0\',
      \'sticky\' => \'0\',
      \'vuuid\' => \'d52abb06-b9e7-9694-fd35-43458352c5c5\',
      \'nid\' => \'1\',
      \'type\' => \'toys_user_priority_ranking\',
      \'language\' => \'und\',
      \'created\' => \'1340289173\',
      \'changed\' => \'1340289173\',
      \'tnid\' => \'0\',
      \'translate\' => \'0\',
      \'uuid\' => \'7ea93a49-0439-db24-8d68-b82707ef7319\',
      \'revision_timestamp\' => \'1340289173\',
      \'revision_uid\' => \'1\',
      \'body\' => array(
        \'und\' => array(
          array(
            \'value\' => "<p>The Drupal Mousepad is very thin, slippery and is especially suitable for optical mice.</p>\\r\\n",
            \'summary\' => \'\',
            \'format\' => \'uw_tf_standard\',
          ),
        ),
      ),
      \'rdf_mapping\' => array(
        \'rdftype\' => array(
          \'sioc:Item\',
          \'foaf:Document\',
        ),
        \'title\' => array(
          \'predicates\' => array(
            \'dc:title\',
          ),
        ),
        \'created\' => array(
          \'predicates\' => array(
            \'dc:date\',
            \'dc:created\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'changed\' => array(
          \'predicates\' => array(
            \'dc:modified\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'body\' => array(
          \'predicates\' => array(
            \'content:encoded\',
          ),
        ),
        \'uid\' => array(
          \'predicates\' => array(
            \'sioc:has_creator\',
          ),
          \'type\' => \'rel\',
        ),
        \'name\' => array(
          \'predicates\' => array(
            \'foaf:name\',
          ),
        ),
        \'comment_count\' => array(
          \'predicates\' => array(
            \'sioc:num_replies\',
          ),
          \'datatype\' => \'xsd:integer\',
        ),
        \'last_activity\' => array(
          \'predicates\' => array(
            \'sioc:last_activity_date\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
      ),
      \'cid\' => 0,
      \'last_comment_timestamp\' => \'1340289173\',
      \'last_comment_name\' => \'\',
      \'last_comment_uid\' => \'1\',
      \'comment_count\' => 0,
      \'name\' => \'admin\',
      \'picture\' => \'0\',
      \'data\' => \'b:0;\',
      \'path\' => FALSE,
      \'menu\' => array(
        \'link_title\' => \'\',
        \'mlid\' => 0,
        \'plid\' => 0,
        \'menu_name\' => \'main-menu\',
        \'weight\' => 0,
        \'options\' => array(),
        \'module\' => \'menu\',
        \'expanded\' => 0,
        \'hidden\' => 0,
        \'has_children\' => 0,
        \'customized\' => 0,
        \'parent_depth_limit\' => 8,
        \'description\' => \'\',
        \'enabled\' => 1,
      ),
      \'node_export_drupal_version\' => \'7\',
    ),
  (object) array(
      \'vid\' => \'2\',
      \'uid\' => \'1\',
      \'title\' => \'Drupal Plush Toy\',
      \'log\' => \'\',
      \'status\' => \'1\',
      \'comment\' => \'0\',
      \'promote\' => \'0\',
      \'sticky\' => \'0\',
      \'vuuid\' => \'ab3c3f8e-d6ea-68b4-15a7-86f473e75f05\',
      \'nid\' => \'2\',
      \'type\' => \'toys_user_priority_ranking\',
      \'language\' => \'und\',
      \'created\' => \'1340289173\',
      \'changed\' => \'1340289173\',
      \'tnid\' => \'0\',
      \'translate\' => \'0\',
      \'uuid\' => \'b763b389-14b2-b4d4-856d-b58bd0d28b5c\',
      \'revision_timestamp\' => \'1340289173\',
      \'revision_uid\' => \'1\',
      \'body\' => array(
        \'und\' => array(
          array(
            \'value\' => "<p>The druplicon as a plush toy for small and big kids.<br />\\r\\n\\tPut it on your desk or give it to your customers as a Christmas gift.</p>\\r\\n",
            \'summary\' => \'\',
            \'format\' => \'uw_tf_standard\',
          ),
        ),
      ),
      \'rdf_mapping\' => array(
        \'rdftype\' => array(
          \'sioc:Item\',
          \'foaf:Document\',
        ),
        \'title\' => array(
          \'predicates\' => array(
            \'dc:title\',
          ),
        ),
        \'created\' => array(
          \'predicates\' => array(
            \'dc:date\',
            \'dc:created\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'changed\' => array(
          \'predicates\' => array(
            \'dc:modified\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'body\' => array(
          \'predicates\' => array(
            \'content:encoded\',
          ),
        ),
        \'uid\' => array(
          \'predicates\' => array(
            \'sioc:has_creator\',
          ),
          \'type\' => \'rel\',
        ),
        \'name\' => array(
          \'predicates\' => array(
            \'foaf:name\',
          ),
        ),
        \'comment_count\' => array(
          \'predicates\' => array(
            \'sioc:num_replies\',
          ),
          \'datatype\' => \'xsd:integer\',
        ),
        \'last_activity\' => array(
          \'predicates\' => array(
            \'sioc:last_activity_date\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
      ),
      \'cid\' => 0,
      \'last_comment_timestamp\' => \'1340289173\',
      \'last_comment_name\' => \'\',
      \'last_comment_uid\' => \'1\',
      \'comment_count\' => 0,
      \'name\' => \'admin\',
      \'picture\' => \'0\',
      \'data\' => \'b:0;\',
      \'path\' => FALSE,
      \'menu\' => array(
        \'link_title\' => \'\',
        \'mlid\' => 0,
        \'plid\' => 0,
        \'menu_name\' => \'main-menu\',
        \'weight\' => 0,
        \'options\' => array(),
        \'module\' => \'menu\',
        \'expanded\' => 0,
        \'hidden\' => 0,
        \'has_children\' => 0,
        \'customized\' => 0,
        \'parent_depth_limit\' => 8,
        \'description\' => \'\',
        \'enabled\' => 1,
      ),
      \'node_export_drupal_version\' => \'7\',
    ),
  (object) array(
      \'vid\' => \'3\',
      \'uid\' => \'1\',
      \'title\' => \'Drupal Pin\',
      \'log\' => \'\',
      \'status\' => \'1\',
      \'comment\' => \'0\',
      \'promote\' => \'0\',
      \'sticky\' => \'0\',
      \'vuuid\' => \'f437aa9f-d12f-5644-2dce-5ef457589e3a\',
      \'nid\' => \'3\',
      \'type\' => \'toys_user_priority_ranking\',
      \'language\' => \'und\',
      \'created\' => \'1340289173\',
      \'changed\' => \'1340289173\',
      \'tnid\' => \'0\',
      \'translate\' => \'0\',
      \'uuid\' => \'df97072e-1bdd-abd4-91c0-4f6eb0d90b85\',
      \'revision_timestamp\' => \'1340289173\',
      \'revision_uid\' => \'1\',
      \'body\' => array(
        \'und\' => array(
          array(
            \'value\' => "<p>With the druplicon pin, you can show everyone that you are a part of the Drupal community.</p>\\r\\n",
            \'summary\' => \'\',
            \'format\' => \'uw_tf_standard\',
          ),
        ),
      ),
      \'rdf_mapping\' => array(
        \'rdftype\' => array(
          \'sioc:Item\',
          \'foaf:Document\',
        ),
        \'title\' => array(
          \'predicates\' => array(
            \'dc:title\',
          ),
        ),
        \'created\' => array(
          \'predicates\' => array(
            \'dc:date\',
            \'dc:created\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'changed\' => array(
          \'predicates\' => array(
            \'dc:modified\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'body\' => array(
          \'predicates\' => array(
            \'content:encoded\',
          ),
        ),
        \'uid\' => array(
          \'predicates\' => array(
            \'sioc:has_creator\',
          ),
          \'type\' => \'rel\',
        ),
        \'name\' => array(
          \'predicates\' => array(
            \'foaf:name\',
          ),
        ),
        \'comment_count\' => array(
          \'predicates\' => array(
            \'sioc:num_replies\',
          ),
          \'datatype\' => \'xsd:integer\',
        ),
        \'last_activity\' => array(
          \'predicates\' => array(
            \'sioc:last_activity_date\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
      ),
      \'cid\' => 0,
      \'last_comment_timestamp\' => \'1340289173\',
      \'last_comment_name\' => \'\',
      \'last_comment_uid\' => \'1\',
      \'comment_count\' => 0,
      \'name\' => \'admin\',
      \'picture\' => \'0\',
      \'data\' => \'b:0;\',
      \'path\' => FALSE,
      \'menu\' => array(
        \'link_title\' => \'\',
        \'mlid\' => 0,
        \'plid\' => 0,
        \'menu_name\' => \'main-menu\',
        \'weight\' => 0,
        \'options\' => array(),
        \'module\' => \'menu\',
        \'expanded\' => 0,
        \'hidden\' => 0,
        \'has_children\' => 0,
        \'customized\' => 0,
        \'parent_depth_limit\' => 8,
        \'description\' => \'\',
        \'enabled\' => 1,
      ),
      \'node_export_drupal_version\' => \'7\',
    ),
)',
);
  return $node_export;
}
