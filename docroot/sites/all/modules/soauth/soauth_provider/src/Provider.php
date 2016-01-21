<?php

namespace Drupal\soauth_provider;

use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Common\Url;
use Drupal\soauth\Common\Field\SimpleField;
use Drupal\soauth\Common\Field\CompoundField;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\OAuth2\AbstractProvider;


/**
 * Class Provider
 * Custom SoAuth provider.
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Provider extends AbstractProvider {
  
  /**
   * Construct Provider
   */
  public function __construct() {
    parent::__construct();
  }
  
  /**
   * {@inheritdoc}
   */
  public function getScope($glue=' ') {
    return parent::getScope($glue);
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return '';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getFullName() {
    return '';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl() {
    return Url::fromUri('');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRequestTokenUrl() {
    return Url::fromUri('');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getUniformFieldMap() {
    return array(
      'id' => new SimpleField(''),
      'mail' => new SimpleField(''),
      'first_name' => new SimpleField(''),
      'last_name' => new SimpleField(''),
    );
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRawUserData($token) {
    
  }
  
  /**
   * {@inheritdoc}
   */
  public function setDefaults() {
    $this->getStorage()->set('scope', array(
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function buildApiEndpointUrl($path) {
    return Url::fromUri(''.$path);
  }
}
