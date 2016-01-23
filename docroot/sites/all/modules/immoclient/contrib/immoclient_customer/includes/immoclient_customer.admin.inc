<?php

/*
 * change admin default ui
 */
class ImmoclientCustomerUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage customers and sellers.';
    return $items;
  }
  
  /**
   * Overrides EntityDefaultUIController::overviewForm().
   */
  public function overviewForm($form, &$form_state) {
    $form['filter'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Filter'),
      '#theme' => 'exposed_filters__reply',
    );

    $form['filter']['status'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('clearfix')),
    );
    $form['filter']['status']['filters'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('filters')),
    );
    $form['filter']['status']['filters']['name'] = array(
      '#type' => 'textfield',
      //'#title' => t('Name'),
      '#default_value' => isset($_GET['name'])? $_GET['name'] : '', 
      '#attributes' => array('placeholder' => t('Name or object reference')),
      '#size' => 25,
    );
    $form['filter']['status']['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['filter']['status']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Filter'),
    );
    $form['filter']['status']['actions']['reset']['#markup'] = l(t('Reset'), $_GET['q']);

    $form['immoclient_customer'] = $this->overviewTable();
    $form['pager'] = array('#theme' => 'pager');

    return $form;
  }
  
  /**
   * Overrides EntityDefaultUIController::overviewFormSubmit().
   */
  public function overviewFormSubmit($form, &$form_state) {
    $args = array();
    if ($form_state['values']['name'] != '') {
      $args['name'] = $form_state['values']['name'];
    }
    $form_state['redirect'] = array(
      'path' => $_GET['q'],
      'options' => array(
        'query' => $args,
      ),
    );
  }
  
  /**
   * Overrides EntityDefaultUIController::overviewTable()
   */
  public function overviewTable($conditions = array()) {
  
    $header = array(
      array('data' => t('ID'), 'field' => 'id', 'sort' => 'DESC'),
      array('data' => t('Name'), 'field' => 'ic.name'),
      array('data' => t('Object'), 'field' => 'ic.objekt_ref'),
      array('data' => t('Status Customer'),'field' => 'ic.customer'),
      array('data' => t('Operations')),
    );

    $query = db_select('immoclient_customer', 'ic')
      ->extend('PagerDefault')->limit(30)
      ->extend('TableSort')->orderByHeader($header)
      ->fields('ic', array('id'));
    
    if (isset($_GET['name'])) {
      $db_or = db_or();
      $db_or->condition('ic.objekt_ref', '%' . db_like($_GET['name']) . '%', 'LIKE');
      $db_or->condition('ic.name', '%' . db_like($_GET['name']) . '%', 'LIKE');
      $query->condition($db_or);     
    }

    $ids = $query->execute()->fetchCol();
    
    $rows = array();
    $clients = immoclient_customer_load_multiple($ids);
    
    foreach($clients as $client){
      $parent_entity = entity_load_single('immoclient_customer', $client->id);
      if ($parent_entity) {
        $parent_link = l($parent_entity->name.', '.$parent_entity->prename, 'customer/' . $client->id);
      }
      else {
        $parent_link = t('Missing: @id', array('@id' => 'customer/' . $client->id));
      }
      $obj_ref = t('No object');
      $nnid = '';
      if($parent_entity->objekt_ref != ''){
        $nnid = immoclient_customer_search_nid($parent_entity->objekt_ref);
          if($nnid != NULL){
          $obj_ref = l($parent_entity->objekt_ref,
                  'node/'.$nnid,
                  array('attributes' => array('target'=>'_blank')));
          }
      }
      $status = '';
      if($parent_entity->customer != ''){
        $status = immoclient_customer_options_admin($parent_entity->customer);
      }
      $row = array();
      $row[] = $client->id;
      $row[] = $parent_link;
      $row[] = $obj_ref;
      $row[] = $status;
      if($parent_entity->customer == 0){
        $operations = array(
          'edit' => array(
            'title' => t('Edit'), 
            'href' => 'admin/customer/manage/' . $client->id, 
            'query' => drupal_get_destination()
            ),
          'delete' => array(
            'title' => t('Delete'), 
            'href' => 'admin/customer/manage/' . $client->id.'/delete', 
            'query' => drupal_get_destination()
            ),
        );
      }
      else{
        $base_url = base_path();
        $operations = array(
          'unedit' => array(
            'title' => t('No operations allowed, please manipulate the object'),
            'href' => $base_url.'node/'.$nnid,
            'fragment'=>'overlay=node/'.$nnid.'/edit#kontakt-hor',
            ),
        );
      }
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
      '#empty' => t('No clients available.'),
    );

    return $render;
  }

}
