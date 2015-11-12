<?php
/**
 * @file
 * Redhen Campaign metadata controller classes
 */

/**
 * Controls metadata for Redhen Campaigns.
 *
 * Redhen utilizes custom property attributes to determine if a property
 * should be available as a filter on the listing page. These attributes are
 *   filter: TRUE to add as a filter.
 *   filter_operator: EFQ supported operators. Defaults to = or IN
 *     depending on value submitted
 *   field_type: textfield, select, etc.
 *   options list: callback that returns the options for this field.
 */
class RedhenCampaignMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['type'] = array(
      'label' => t('Type'),
      'description' => t('The type of campaign.'),
      'type' => 'token',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer redhen campaigns',
      'options list' => 'redhen_campaign_type_options_list',
      'required' => TRUE,
      'schema field' => 'type',
    );
    $properties['label'] = array(
      'label' => t('Campaign Name'),
      'filter' => TRUE,
      'field_type' => 'textfield',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'filter_operator' => 'CONTAINS',
    ) + $properties['label'];
    $properties['created'] = array(
      'label' => t("Created"),
      'description' => t("The date the campaign was created."),
      'type' => 'date',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set',
    );
    $properties['updated'] = array(
      'label' => t("Updated"),
      'description' => t("The date the campaign was last updated."),
      'type' => 'date',
      'schema field' => 'updated',
      'setter callback' => 'entity_property_verbatim_set',
    );
    $properties['progress'] = array(
      'label' => t("Progress"),
      'type' => 'integer',
      'description' => t("Total of all donations to the Campaign."),
      'schema field' => 'progress',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'entity views field' => TRUE,
    );
    $properties['author'] = array(
      'label' => t("Creator"),
      'type' => 'user',
      'description' => t("The creator of the campaign."),
      'schema field' => 'author_uid',
      'required' => TRUE,
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );

    return $info;
  }

}
