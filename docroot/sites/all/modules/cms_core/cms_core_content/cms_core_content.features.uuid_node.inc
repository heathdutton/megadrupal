<?php
/**
 * @file
 * cms_core_content.features.uuid_node.inc
 */

/**
 * Implements hook_uuid_features_default_content().
 */
function cms_core_content_uuid_features_default_content() {
  $nodes = array();

  $nodes[] = array(
  'title' => 'Glazed Drupal CMS Homepage',
  'log' => '',
  'status' => 1,
  'comment' => 1,
  'promote' => 0,
  'sticky' => 0,
  'type' => 'page',
  'language' => 'und',
  'created' => 1449755203,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '033f465c-cc7b-4404-b58f-b5b493f21a77',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<div class="az-element az-section  " data-az-id="b2" data-azat-style="margin-top:60px;" data-azb="az_section" id="b2" style="margin-top:60px;">
	<div class="az-ctnr container" data-azcnt="true">
		<div class="az-element az-row row" data-az-id="b3" data-azat-device="sm" data-azb="az_row" data-azcnt="true" style="">
			<div class="az-element az-ctnr az-column  col-sm-4" data-az-id="b4" data-azat-width="1/3" data-azb="az_column" data-azcnt="true" style="">
				<div class="az-element az-text" data-az-id="b6" data-azb="az_text" data-azcnt="true" style="">
					<div>
						<h1>Glazed Drupal CMS</h1>

						<h2>Clean, Modern Design. Powerful Bootstrap Tools</h2>
					</div>
				</div>

				<div class="az-element" data-az-id="b7" data-azat-style="margin-top:0px;margin-bottom:0px;padding-top:0px;padding-bottom:0px;" data-azb="az_unknown" data-azcnt="true"></div>

				<div class="az-element az-text" data-az-id="b8" data-azb="az_text" data-azcnt="true" style="">
					<div>
						<p>Enjoy a fully featured Drupal CMS with a modern theme. In the Glazed dashboard you can set the layout width and spacing. You can also tweak 20 colors to match your brand and make your Glazed site unique.</p>
					</div>
				</div>

				<div class="az-element az-template " data-az-id="b9" data-azat-style="margin-top:15px;" data-azb="button-3d" style="margin-top:15px;">
					<div data-azcnt="true"><a class="btn btn-lg stpe-3dbutton glazed-util-background-primary glazed-util-color-white glazed-util-hover-color-white az-editable" href="http://sooperthemes.com/pricing" style="" target="_self" type="button"><em>Join</em> to Download</a></div>
				</div>
			</div>

			<div class="az-element az-ctnr az-column  col-sm-8" data-az-id="b10" data-azat-style="padding-left:90px;" data-azat-width=" 2/3" data-azb="az_column" data-azcnt="true" style="padding-left:90px;">
				<div class="az-element az-image " data-az-id="b11" data-azat-height="" data-azat-image="http://i.imgur.com/GEomSkV.jpg" data-azat-style="opacity:0.63;" data-azb="az_image"><img alt="" src="http://i.imgur.com/GEomSkV.jpg" style="opacity: 0.63; width: 100%;"></div>
			</div>
		</div>
	</div>
</div>',
        'summary' => '',
        'format' => 'wysiwyg_full',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_page_attachments' => array(),
  'field_glazed_content_design' => array(),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'path' => array(
    'pathauto' => 1,
  ),
  'comment_count' => 0,
  'pathauto_perform_alias' => FALSE,
  'date' => '2015-12-10 13:46:43 +0000',
  'user_uuid' => 'c155b92b-6053-4816-8abf-401a4fcd0619',
);
  return $nodes;
}
