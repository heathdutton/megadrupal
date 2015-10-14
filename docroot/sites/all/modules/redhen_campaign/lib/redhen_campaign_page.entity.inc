<?php
/**
 * @file
 * Redhen Campaign Page entity classes
 */

/**
 * The class used for campaign page entities.
 */
class RedhenCampaignPage extends Entity {

  // @codingStandardsIgnoreStart
  public
    $page_id = NULL,
    $author_uid = NULL,
    $type = '',
    $created = '',
    $updated = '',
    $campaign = NULL,
    $team = NULL,
    $progress = 0,
    $campaign_page_type = NULL;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'redhen_campaign_page');
  }

  /**
   * Override buildContent() to add campaign page properties.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {

    $wrapper = entity_metadata_wrapper('redhen_campaign_page', $this);
    $team = $wrapper->team->value();
    $owner = user_load($this->author_uid);
    $owner_contact = redhen_contact_load_by_user($owner);
    switch ($view_mode) {
      case 'snippet':
        $owner_view_mode = 'snippet';
        break;

      case 'teaser':
        $owner_view_mode = 'redhen_campaign_page_owner_teaser';
        break;

      case 'full':
      default:
        $owner_view_mode = 'redhen_campaign_page_owner';
        break;
    }
    $uri = $this->uri();
    $content = array(
      'label' => array(
        '#theme' => 'redhen_property_field',
        '#label_hidden' => TRUE,
        '#items' => array(
          array(
            '#markup' => $wrapper->label(),
          ),
        ),
        '#classes' => 'field field-label-inline clearfix redhen_campaign_page_title',
        '#attributes' => '',
      ),
      'owner' => $owner_contact ? redhen_contact_view($owner_contact, $owner_view_mode, NULL, TRUE) : NULL,
      'campaign' => array(
        '#theme' => 'redhen_property_field',
        //'#label' => t('Campaign'),
        '#label_hidden' => TRUE,
        '#items' => array(
          array(
            '#markup' => l($wrapper->campaign->label(), $wrapper->campaign->url->value()),
          ),
        ),
        '#classes' => 'field field-label-inline clearfix redhen_campaign_page_campaign',
        '#attributes' => '',
      ),
      // @todo may want to check that donations are enabled before adding this:
      'donate' => array(
        '#theme' => 'redhen_property_field',
        '#label_hidden' => TRUE,
        '#items' => array(
          array(
            '#markup' => l(t('Donate'), 'redhen_campaign/' . $this->campaign . '/donate', array(
              'query' => array(
                'campaign_page' => $this->page_id,
                'destination' => $uri['path'],
              ),
            )),
          ),
        ),
        '#classes' => 'field field-label-inline clearfix redhen_campaign_page_donate',
        '#attributes' => '',
      ),
    );
    if (!empty($team)) {
      $content['team'] = array(
        '#theme' => 'redhen_property_field',
        '#label' => t('Team'),
        '#items' => array(
          array(
            '#markup' => l($team->label, $wrapper->team->url->value()),
          ),
        ),
        '#classes' => 'field field-label-inline clearfix redhen_campaign_page_team',
        '#attributes' => '',
      );
    }
    return entity_get_controller($this->entityType)
      ->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Set the parent campaign for a campaign page.
   *
   * @param RedhenCampaign $campaign
   *   The RedhenCampaign to use as the parent
   *
   * @return bool
   *   Return FALSE if we are unable to set the parent campaign
   */
  public function setParentCampaign(RedhenCampaign $campaign) {
    $this->campaign = $campaign->campaign_id;
    return $this->save();
  }

  /**
   * Set the team campaign page for a campaign page.
   *
   * @param RedhenCampaignPage $team
   *   The RedhenCampaignPage to use as the parent
   *
   * @return bool
   *   Return FALSE if we are unable to set the team page campaign
   */
  public function setTeam(RedhenCampaignPage $team) {
    $this->team = $team->page_id;
    return $this->save();
  }

  /**
   * Specify URI.
   */
  protected function defaultUri() {
    return array('path' => 'redhen_campaign/page/' . $this->internalIdentifier());
  }

}
