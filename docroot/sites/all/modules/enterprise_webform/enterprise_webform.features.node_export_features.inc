<?php
/**
 * @file
 * enterprise_webform.features.node_export_features.inc
 */

/**
 * Implements hook_node_export_features_default().
 */
function enterprise_webform_node_export_features_default() {
  $node_export = array(
  'code_string' => 'array(
  array(
    \'vid\' => \'1\',
    \'uid\' => \'1\',
    \'title\' => \'Contact us\',
    \'log\' => \'\',
    \'status\' => \'1\',
    \'comment\' => \'1\',
    \'promote\' => \'0\',
    \'sticky\' => \'0\',
    \'vuuid\' => \'c647cb13-461b-402b-bd55-7adb5b868631\',
    \'nid\' => \'1\',
    \'type\' => \'webform\',
    \'language\' => \'und\',
    \'created\' => \'1330908207\',
    \'changed\' => \'1330909185\',
    \'tnid\' => \'0\',
    \'translate\' => \'0\',
    \'uuid\' => \'c0e632d3-aae6-4ebe-ae21-79756337c416\',
    \'revision_timestamp\' => \'1330909185\',
    \'revision_uid\' => \'1\',
    \'body\' => array(),
    \'rdf_mapping\' => array(
      \'rdftype\' => array(
        \'0\' => \'sioc:Item\',
        \'1\' => \'foaf:Document\',
      ),
      \'title\' => array(
        \'predicates\' => array(
          \'0\' => \'dc:title\',
        ),
      ),
      \'created\' => array(
        \'predicates\' => array(
          \'0\' => \'dc:date\',
          \'1\' => \'dc:created\',
        ),
        \'datatype\' => \'xsd:dateTime\',
        \'callback\' => \'date_iso8601\',
      ),
      \'changed\' => array(
        \'predicates\' => array(
          \'0\' => \'dc:modified\',
        ),
        \'datatype\' => \'xsd:dateTime\',
        \'callback\' => \'date_iso8601\',
      ),
      \'body\' => array(
        \'predicates\' => array(
          \'0\' => \'content:encoded\',
        ),
      ),
      \'uid\' => array(
        \'predicates\' => array(
          \'0\' => \'sioc:has_creator\',
        ),
        \'type\' => \'rel\',
      ),
      \'name\' => array(
        \'predicates\' => array(
          \'0\' => \'foaf:name\',
        ),
      ),
      \'comment_count\' => array(
        \'predicates\' => array(
          \'0\' => \'sioc:num_replies\',
        ),
        \'datatype\' => \'xsd:integer\',
      ),
      \'last_activity\' => array(
        \'predicates\' => array(
          \'0\' => \'sioc:last_activity_date\',
        ),
        \'datatype\' => \'xsd:dateTime\',
        \'callback\' => \'date_iso8601\',
      ),
    ),
    \'webform\' => array(
      \'nid\' => \'1\',
      \'confirmation\' => \'\',
      \'confirmation_format\' => \'filtered_html\',
      \'redirect_url\' => \'<confirmation>\',
      \'status\' => \'1\',
      \'block\' => \'1\',
      \'teaser\' => \'0\',
      \'allow_draft\' => \'0\',
      \'auto_save\' => \'0\',
      \'submit_notice\' => \'1\',
      \'submit_text\' => \'\',
      \'submit_limit\' => \'-1\',
      \'submit_interval\' => \'-1\',
      \'total_submit_limit\' => \'-1\',
      \'total_submit_interval\' => \'-1\',
      \'record_exists\' => TRUE,
      \'roles\' => array(
        \'0\' => \'1\',
        \'1\' => \'2\',
      ),
      \'emails\' => array(),
      \'components\' => array(
        \'1\' => array(
          \'nid\' => 1,
          \'cid\' => \'1\',
          \'pid\' => \'0\',
          \'form_key\' => \'name\',
          \'name\' => \'Name\',
          \'type\' => \'textfield\',
          \'value\' => \'%username\',
          \'extra\' => array(
            \'title_display\' => \'before\',
            \'private\' => 0,
            \'disabled\' => 0,
            \'unique\' => 0,
            \'conditional_operator\' => \'=\',
            \'width\' => \'\',
            \'maxlength\' => \'\',
            \'field_prefix\' => \'\',
            \'field_suffix\' => \'\',
            \'description\' => \'\',
            \'attributes\' => array(),
            \'conditional_component\' => \'\',
            \'conditional_values\' => \'\',
          ),
          \'mandatory\' => \'1\',
          \'weight\' => \'0\',
          \'page_num\' => 1,
        ),
        \'2\' => array(
          \'nid\' => 1,
          \'cid\' => \'2\',
          \'pid\' => \'0\',
          \'form_key\' => \'email\',
          \'name\' => \'Email\',
          \'type\' => \'email\',
          \'value\' => \'%useremail\',
          \'extra\' => array(
            \'title_display\' => \'before\',
            \'private\' => 0,
            \'disabled\' => 0,
            \'unique\' => 0,
            \'conditional_operator\' => \'=\',
            \'width\' => \'\',
            \'description\' => \'\',
            \'attributes\' => array(),
            \'conditional_component\' => \'\',
            \'conditional_values\' => \'\',
          ),
          \'mandatory\' => \'1\',
          \'weight\' => \'1\',
          \'page_num\' => 1,
        ),
        \'3\' => array(
          \'nid\' => 1,
          \'cid\' => \'3\',
          \'pid\' => \'0\',
          \'form_key\' => \'message\',
          \'name\' => \'Message\',
          \'type\' => \'textarea\',
          \'value\' => \'\',
          \'extra\' => array(
            \'title_display\' => 0,
            \'private\' => 0,
            \'resizable\' => 1,
            \'disabled\' => 0,
            \'conditional_operator\' => \'=\',
            \'cols\' => \'\',
            \'rows\' => \'\',
            \'description\' => \'\',
            \'attributes\' => array(),
            \'conditional_component\' => \'\',
            \'conditional_values\' => \'\',
          ),
          \'mandatory\' => \'1\',
          \'weight\' => \'2\',
          \'page_num\' => 1,
        ),
      ),
    ),
    \'cid\' => \'0\',
    \'last_comment_timestamp\' => \'1330908207\',
    \'last_comment_name\' => NULL,
    \'last_comment_uid\' => \'1\',
    \'comment_count\' => \'0\',
    \'name\' => \'admin\',
    \'picture\' => \'0\',
    \'data\' => \'b:0;\',
    \'path\' => FALSE,
    \'menu\' => array(
      \'link_title\' => \'Form\',
      \'mlid\' => 0,
      \'plid\' => \'0\',
      \'menu_name\' => \'main-menu\',
      \'weight\' => \'0\',
      \'options\' => array(),
      \'module\' => \'menu\',
      \'expanded\' => \'0\',
      \'hidden\' => \'0\',
      \'has_children\' => \'0\',
      \'customized\' => 0,
      \'parent_depth_limit\' => 8,
      \'description\' => \'\',
      \'enabled\' => 1,
    ),
    \'node_export_drupal_version\' => \'7\',
    \'#_export_node_encode_object\' => \'1\',
  ),
)',
);
  return $node_export;
}
