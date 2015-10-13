<?php
/**
 * @file classes for tfa_totp
 */

/**
 * Class TfaTotp
 */
class Tfaduo extends TfaBasePlugin implements TfaValidationPluginInterface {
  protected $host;
  protected $skey;
  protected $ikey;

  public function __construct(array $context) {
    parent::__construct($context);
    $this->host = variable_get('tfa_duo_host', '');
    $this->skey = variable_get('tfa_duo_skey', '');
    $this->ikey = variable_get('tfa_duo_ikey', '');
  }

  /**
   * @copydoc TfaValidationPluginInterface::getForm()
   */
  public function getForm(array $form, array &$form_state) {
    global $base_url;
    $duo = new Duo();
    if (isset($_POST['sig_response'])) {
      $form_state['storage']['sig_response'] = $_POST['sig_response'];
      $form['duo'] = array('#type'=>'hidden','#value'=>$_POST['sig_response']);

       drupal_add_js("jQuery( document ).ready(function() {jQuery('#tfa-form').submit();});",'inline');
    } else {
      $account = user_load($this->context['uid']);
      $sig_request = $duo->signRequest($this->ikey, $this->skey, sha1(drupal_get_private_key()), $account->name);
      $js = drupal_get_path('module', 'tfa_duo') . '/Duo-Web-v1.min.js';
      $host = $this->host;
      drupal_add_js($js);
      $currentpath = $base_url . '/' . current_path();
      $form['directions'] = array('#type' => 'markup', '#markup' => '<h2>Please authenticate</h2>');
   drupal_add_js("
  Duo.init({
    'host': '$host',
    'sig_request': '$sig_request',
    'post_action': '$currentpath'
  });
    ",'inline'
      );
      $form['iframe'] = array('#markup'=>"<iframe id='duo_iframe' width='620' height='330' frameborder='0'></iframe>");
    }
    $form['submit'] = array('#prefix'=>'<div style="display:none">', '#type' => 'submit', '#value' => 'submit','#suffix'=>"</div>");

    return $form;
  }

  /**
   * @copydoc TfaValidationPluginInterface::getForm()
   */
  public function validateForm(array $form, array &$form_state) {

  }

  /**
   * @copydoc TfaBasePlugin::submitForm()
   */
  public function submitForm(array $form, array &$form_state) {
    $duo = new Duo();
    $from_duo =  $duo->verifyResponse($this->ikey, $this->skey, sha1(drupal_get_private_key()),$_POST['duo']);
    $account = user_load($this->context['uid']);
    if ($from_duo == $account->name) {
      return TRUE;
    }
    return FALSE;
  }



  /**
   * @copydoc TfaBasePlugin::ready()
   */

  public function ready() {
    if ($this->ikey == '') {
      return FALSE;
    }
    if (user_access('require duo')) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}


