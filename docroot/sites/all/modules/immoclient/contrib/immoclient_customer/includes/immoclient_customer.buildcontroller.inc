<?php
/**
 * Extending the EntityAPIController for the customer entity.
 */
class ImmoclientCustomerEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
 
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    if($entity->objekt_ref != ''){
      $build['objekt_ref'] = array(
        '#type' => 'markup',
        '#markup' => '<h3>'.t('Object').': '.check_plain($entity->objekt_ref).'-'.immoclient_customer_plainreference($entity->objekt_ref).'</h3>',
        '#prefix' => '<div class="customer_object_title">',
        '#suffix' => '</div>',
        '#weight' => 0,
      );
    }
    if(isset($entity->customer)){
      $build['customer'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'. immoclient_customer_custom($entity->customer).'</b>',
        '#prefix' => '<div class="customer_customer">',
        '#suffix' => '</div>',
        '#weight' => 1,
      );
    }
    $build['salutation'] = array(
      '#type' => 'markup',
      '#markup' => immoclient_customer_bool($entity->salutation),
      '#prefix' => '<div style="float:left; margin-right: 5px"class="customer_salutation">',
      '#suffix' => '</div>',
      '#weight' => 2,
    );
    if(isset($entity->prename)){
      $build['prename'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->prename),
        '#prefix' => '<div style="float:left; margin-right: 5px" class="customer_prename">',
        '#suffix' => '</div>',
        '#weight' => 3,
      );
    }
    if(isset($entity->name)){
      $build['name'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->name),
        '#prefix' => '<div class="customer_name">',
        '#suffix' => '</div>',
        '#weight' => 4,
      );
    }
    if(isset($entity->field_customer_strasse_hnr)){
        $customer = field_get_items('immoclient_customer', $entity, 'field_customer_strasse_hnr');
        $street = $customer [0]['value'];
        $build['field_customer_strasse_hnr'] = array(
         '#type' => 'markup',
        '#markup' => check_plain($street),
      '#prefix' => '<div class="customer_street">',
      '#suffix' => '</div>',
      '#weight' => 5,
    );   
    }
    if(isset($entity->field_customer_land)){
      $land = field_get_items('immoclient_customer', $entity, 'field_customer_land');
      $country = $land [0]['value'];
      $build['field_customer_land'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($country).' - ',
        '#prefix' => '<div style="float:left; margin-right: 5px"class="customer_country">',
        '#suffix' => '</div>',
        '#weight' => 6,
      );   
    }
    if(isset($entity->field_customer_plz)){
      $plz = field_get_items('immoclient_customer', $entity, 'field_customer_plz');
      $plc = $plz [0]['value'];
      $build['field_customer_plz'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($plc),
        '#prefix' => '<div style="float:left; margin-right: 5px" class="customer_plc">',
        '#suffix' => '</div>',
        '#weight' => 7,
      );   
    }
    if(isset($entity->field_customer_stadt)){
      $stadt = field_get_items('immoclient_customer', $entity, 'field_customer_stadt');
      $city = $stadt [0]['value'];
      $build['field_customer_stadt'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($city),
        '#prefix' => '<div class="customer_city">',
        '#suffix' => '</div>',
        '#weight' => 8,
      );   
    }
    if(isset($entity->field_customer_tel_privat)){
      $tel_priv = field_get_items('immoclient_customer', $entity, 'field_customer_tel_privat');
      $tel_private = $tel_priv[0]['value'];
      $build['field_customer_tel_privat'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Tel').'</b>'.': '.check_plain($tel_private),
        '#prefix' => '<div class="customer_comm">',
        '#suffix' => '</div>',
        '#weight' => 9,
      );   
    }
    if(isset($entity->field_customer_tel_mobil)){
      $tel_mobil = field_get_items('immoclient_customer', $entity, 'field_customer_tel_mobil');
      $tel_mobile = $tel_mobil [0]['value'];
      $build['field_customer_tel_mobil'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Mobil').'</b>'.': '.check_plain($tel_mobile),
        '#prefix' => '<div class="customer_comm">',
        '#suffix' => '</div>',
        '#weight' => 10,
    );   
    }
    else{
        $build['field_customer_tel_mobil'] = array(
          '#type' => 'markup',
          '#markup' => '',
    //    '#prefix' => '<div style="float:left; padding: 0 5px" class="pricedefault_kaufpreis">',
    //    '#suffix' => '</div>',
          '#weight' => 10,
    ); 
    }
    if(isset($entity->field_customer_fax)){
      $fax = field_get_items('immoclient_customer', $entity, 'field_customer_fax');
      $faxe = $fax [0]['value'];
      $build['field_customer_fax'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Fax').'</b>'.': '.check_plain($faxe),
        '#prefix' => '<div class="customer_comm">',
        '#suffix' => '</div>',
        '#weight' => 11,
        );   
    }
    if(isset($entity->field_customer_email)){
      $email = field_get_items('immoclient_customer', $entity, 'field_customer_email');
      $mail = $email [0]['value'];
      $build['field_fax'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.t('Email').'</b>'.': '.'<a href="mailto:'.$mail.'">'.$mail.'</a>',
        '#prefix' => '<div class="customer_comm">',
        '#suffix' => '</div>',
        '#weight' => 12,
      );   
    }   
return $build;
  }
  
}