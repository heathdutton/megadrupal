<?php
/**
 * @file
 * Redhen Campaign entity classes
 */

/**
 * The class used for campaign entities.
 */
class RedhenCampaign extends Entity {

  // @codingStandardsIgnoreStart
  public
    $label = '',
    $campaign_id = NULL,
    $author_uid = NULL,
    $type = '',
    $created = '',
    $updated = '',
    $progress = 0,
    $promote = TRUE,
    $sticky = FALSE;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'redhen_campaign');
  }

  /**
   * Override buildContent() to add campaign properties.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {

    $wrapper = entity_metadata_wrapper('redhen_campaign', $this);
    $content = array(
      'label' => array(
        '#theme' => 'redhen_property_field',
        '#label_hidden' => TRUE,
        '#items' => array(
          array(
            '#markup' => $wrapper->label(),
          ),
        ),
        '#classes' => 'field field-label-inline clearfix redhen_campaign_title',
        '#attributes' => '',
      ),
      'join' => array(
        '#theme' => 'redhen_property_field',
        '#label_hidden' => TRUE,
        '#items' => array(),
        '#classes' => 'field field-label-inline clearfix redhen_campaign_join_link',
        '#attributes' => '',
      ),
    );
    if (user_is_anonymous()) {
      $destination = url('redhen_campaign_page/add', array('query' => array('campaign' => $this->identifier())));
      $content['join']['#items'][] = array(
        '#markup' => l(t('Join Now'), 'user/register', array('query' => array('destination' => $destination))),
        '#prefix' => '<div class="join">',
        '#suffix' => '</div>',
      );
    }
    else {
      $content['join']['#items'][] = array(
        '#markup' => l(t('Join Now'), 'redhen_campaign_page/add', array('query' => array('campaign' => $this->identifier()))),
        '#prefix' => '<div class="join">',
        '#suffix' => '</div>',
      );
    }
    return entity_get_controller($this->entityType)
      ->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Specify URI.
   */
  protected function defaultUri() {
    return array('path' => 'redhen_campaign/' . $this->internalIdentifier());
  }

}
