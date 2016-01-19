<?php

/**
 * @file
 * Definition of SalsaEntitySupporterMyDonatePage class.
 */
/*
 * SalsaEntitySupporterMyDonatePage class.
 */
class SalsaEntitySupporterMyDonatePage extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $my_donate_page = entity_load_single('salsa_my_donate_page', $this->my_donate_page_KEY);

    $content['my_donate_form'] = drupal_get_form('salsa_my_donate_page_form', $this, $my_donate_page, $view_mode);

    if (isset($my_donate_page->Donation_Page_Footer)) {
      $content['Donation_Page_Footer'] = array(
        '#type' => 'item',
        '#markup' => $my_donate_page->Donation_Page_Footer,
      );
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  public function defaultLabel() {
    return $this->Page_Title;
  }

}

/*
 * SalsaEntityMyDonatePage class.
 */

class SalsaEntityMyDonatePage extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    global $user;
    if (!$user->uid) {
      $content['not_logged_in']['#markup'] = t('You need to log in or register to be able to collect donations.');
      $content['login_block'] = drupal_get_form('user_login_block');
    }
    else {
      // Check if the user already has a page.
      $supporter = salsa_profile_get_supporter($user);
      if ($supporter) {
        $supporter_my_donate_pages = entity_load('salsa_supporter_my_donate_page', FALSE, array('supporter_KEY' => $supporter->supporter_KEY, 'my_donate_page_KEY' => $this->{$this->idKey}));
      }
      if (!empty($supporter_my_donate_pages)) {
        $content['supporter_donate_pages']['#markup'] = salsa_my_donate_page_list($this);
      }
      else {
        $content['title']['#markup'] = '<h2>' . t('Create a new donate page') . '</h2>';
        $content['title']['#weight'] = -50;
        $content['my_donate_create_form'] = drupal_get_form('salsa_my_donate_page_create_form', $this);
      }
    }


    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

}
