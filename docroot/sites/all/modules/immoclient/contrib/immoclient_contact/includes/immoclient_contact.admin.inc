<?php

/*
 * change admin default ui
 */
class ImmoclientContactUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage contacts.';
    return $items;
  }
  /**
   * Overrides EntityDefaultUIController::overviewTable()
   */
  public function overviewTable($conditions = array()) {
  
    $header = array(
      array('data' => t('ID'), 'field' => 'id', 'sort' => 'DESC'),
      array('data' => t('Name'), 'field' => 'ico.name'),
      array('data' => t('Operations')),
    );

    $query = db_select('immoclient_contact', 'ico')
      ->extend('PagerDefault')->limit(30)
      ->extend('TableSort')->orderByHeader($header)
      ->fields('ico', array('id'));

    $ids = $query->execute()->fetchCol();
    
    $rows = array();
    $clients = immoclient_contact_load_multiple($ids);
    
    foreach($clients as $client){
      $parent_entity = entity_load_single('immoclient_contact', $client->id);
      if ($parent_entity) {
        $parent_link = l($parent_entity->name.', '.$parent_entity->vorname, 'contact/' . $client->id);
      }
      else {
        $parent_link = t('Missing: @id', array('@id' => 'contact/' . $client->id));
      }
      $row = array();
      $row[] = $client->id;
      $row[] = $parent_link;
      $operations = array(
        'edit' => array('title' => t('Edit'), 'href' => 'admin/contact/manage/' . $client->id, 'query' => drupal_get_destination()),
        'delete' => array('title' => t('Delete'), 'href' => 'admin/contact/manage/' . $client->id.'/delete', 'query' => drupal_get_destination()),
      );
      if(user_access('admin customer')){
      $row[] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
      }
      else{
        $row[] = '';
      }
      $rows[] = $row;
    }
    
    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No contacts available.'),
    );

    return $render;
  }

}