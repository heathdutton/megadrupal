<?php
/**
 * @file
 * Redhen Campaign Page metadata controller classes
 */

/**
 * Controls metadata for Redhen Campaign Pages.
 *
 * Redhen utilizes custom property attributes to determine if a property
 * should be available as a filter on the listing page. These attributes are
 *   filter: TRUE to add as a filter.
 *   filter_operator: EFQ supported operators. Defaults to = or IN
 *     depending on value submitted
 *   field_type: textfield, select, etc.
 *   options list: callback that returns the options for this field.
 */
class RedhenCampaignPageMetadataController extends RedhenCampaignMetadataController {

  /**
   * Overrides entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['type'] = array(
      'label' => t('Type'),
      'description' => t('The type of campaign page.'),
      'type' => 'token',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer redhen campaign pages',
      'options list' => 'redhen_campaign_page_type_options_list',
      'required' => TRUE,
      'schema field' => 'type',
    );
    $properties['label']['label'] = t('Campaign Page Name');
    $properties['created']['description'] = t("The date the page was created.");
    $properties['updated']['description'] = t("The date the page was last updated.");
    $properties['campaign_page_type'] = array(
      'label' => t("Page Type"),
      'description' => t("Distinguish between Individual and Team pages."),
      'type' => 'int',
      'schema field' => 'campaign_page_type',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
    );

    // Entities.
    $properties['author']['description'] = t("The creator of the page.");

    $properties['campaign'] = array(
      'label' => t("Parent Campaign"),
      'type' => 'redhen_campaign',
      'description' => t("The campaign which this page is a part of."),
      'schema field' => 'campaign',
      'required' => TRUE,
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );
    $properties['team'] = array(
      'label' => t("Team Page"),
      'type' => 'redhen_campaign_page',
      'description' => t("The team which this page is a part of."),
      'schema field' => 'team',
      'required' => FALSE,
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['progress'] = array(
      'label' => t("Progress"),
      'type' => 'integer',
      'description' => t("Total of all donations to the Campaign through this Page."),
      'schema field' => 'progress',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'entity views field' => TRUE,
    );
    return $info;
  }

}
