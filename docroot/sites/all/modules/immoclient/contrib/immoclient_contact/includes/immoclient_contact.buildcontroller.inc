<?php

/**
 * Extending the EntityAPIController for the contact entity.
 */
class ImmoclientContactEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    if(user_access('admin customer')){
      $build['edit_link'] = array(
        '#type' => 'markup',
        '#markup' =>l(t('Edit'), 'admin/contact/manage/' . $entity->id, array('attributes' => array('class' => 'admin-edit-link'))),
        '#prefix' => '<ul class="tabs primary"><li>',
        '#suffix' => '</li></ul>',
      );
    }
    $build['adressfreigabe'] = array(
      '#type' => 'markup',
      '#markup' =>'<b>'. t('Show address?').'</b>'.': '.immoclient_contact_bool($entity->adressfreigabe),
      '#prefix' => '<div class="contact_addr_fre">',
      '#suffix' => '</div>',
    );
    if($entity->anrede_brief !=''){
      $build['anrede_brief'] = array(
        '#type' => 'markup',
        '#markup' => $entity->anrede_brief.', ',
        '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_sal">',
        '#suffix' => '</div>',
      );
    }
    if($entity->position !=''){
      $build['position'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->position).', ',
        '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_pos">',
        '#suffix' => '</div>',
      );
    }
    $build['firma'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->firma),
      '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_firma">',
      '#suffix' => '</div>',
    );
    $build['anrede'] = array(
      '#type' => 'markup',
      '#markup' => immoclient_contact_bool_anrede($entity->anrede),
      '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_sal">',
      '#suffix' => '</div>',
    );
    $build['titel'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->titel),
      '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_title">',
      '#suffix' => '</div>',
    );
    $build['vorname'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->vorname),
      '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_prename">',
      '#suffix' => '</div>',
    );
    $build['name'] = array(
      '#type' => 'markup',
      '#markup' => $entity->name,
      '#contextual_links' => array('immoclient_contact'=>array('contact',array($entity->id))),
      '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_name">',
      '#suffix' => '</div>',
      
    );
    $build['strasse'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->strasse),
      '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_str">',
      '#suffix' => '</div>',
    );
    $build['hausnummer'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->hausnummer),
      '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_nbr">',
      '#suffix' => '</div>',
    );
    if($entity->land != ''){
      $build['land'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->land)."- ",
        '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_land">',
        '#suffix' => '</div>',
      );
    }
    if($entity->plz != ''){
      $build['plz'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->plz),
        '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_plz">',
        '#suffix' => '</div>',
      );
    }
    if($entity->ort != ''){
      $build['ort'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->ort),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_ort">',
        '#suffix' => '</div>',
      );
    }
    if($entity->postfach != ''){
    $build['postfach'] = array(
      '#type' => 'markup',
      '#markup' => "<b>".t("P.O.Box").":"."</b>".check_plain($entity->postfach),
      '#prefix' => '<div style="float:left; padding: 0 5px 0 0" class="contact_pstf">',
      '#suffix' => '</div>',
      );
    }
    if($entity->postf_plz != ''){
      $build['postf_plz'] = array(
        '#type' => 'markup',
        '#markup' =>"<b>". t("P.O.Box postal code").":"."</b>".check_plain($entity->postf_plz),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_pstf_plz">',
        '#suffix' => '</div>',
      );
    }
    
    if($entity->email_zentrale != ''){
      $build['email_zentrale'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Email firm").":"."</b>".check_plain($entity->email_zentrale),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    
    if($entity->email_direkt != ''){
      $build['email_direkt'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Email contact").":"."</b>".check_plain($entity->email_direkt),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    
    if($entity->tel_zentrale != ''){
      $build['tel_zentrale'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Telephone firm").":"."</b>".check_plain($entity->tel_zentrale),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    
    if($entity->tel_durchw != ''){
      $build['tel_durchw'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Telephone contact").":"."</b>".check_plain($entity->tel_durchw),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    if($entity->tel_fax != ''){
      $build['tel_fax'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Fax").":"."</b>".check_plain($entity->tel_fax),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    
    if($entity->tel_handy != ''){
      $build['tel_handy'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Smartphone").":"."</b>".check_plain($entity->tel_handy),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    
    if($entity->personennummer != ''){
      $build['personennummer'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Personennummer").":"."</b>".check_plain($entity->personennummer),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    if($entity->immobilientreuhaenderid != ''){
      $build['immobilientreuhaenderid'] = array(
        '#type' => 'markup',
        '#markup' => "<b>".t("Treuhaender ID").":"."</b>".check_plain($entity->immobilientreuhaenderid),
        '#prefix' => '<div style="padding: 0 5px 0 0" class="contact_emz">',
        '#suffix' => '</div>',
      );
    }
    return $build;
  }
  
}