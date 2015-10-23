<?php
/**
 * @file
 */

namespace Drupal\cakemail_relay;
use Drupal\cakemail_api\APIInterface;

/**
 * Class CakemailTemplate
 */
class Template {

  /**
   * @var string
   */
  private $template;

  /**
   * @param APIInterface $api
   * @param $template_id
   * @param $user_key
   * @param null $client_id
   */
  public function __construct(APIInterface $api, $template_id, $user_key, $client_id = NULL) {
    $args = array(
      'user_key' => $user_key,
      'template_id' => $template_id,
    );
    if (!empty($client_id)) {
      $args['client_id'] = $client_id;
    }
    $this->template = $api->TemplateV2->GetTemplate($args);
  }


  /**
   * @param array $variables
   *   An associative array of variables to use when applying the template. Keys
   *   are the variable name used in the template, without the enclosing
   *   brackets ('[' and ']').
   * @return string
   */
  public function apply($variables = array()) {
    $replacements = array();
    foreach ($variables as $key => $value) {
      // Ensure variables are available in full lower/upper-case.
      $replacements[drupal_strtolower($key)] = $replacements[drupal_strtoupper($key)] = $value;
    }
    $tokens = array_keys($replacements);
    $values = array_values($replacements);
    return str_replace($tokens, $values, $this->content);
  }

  function __get($name) {
    if (isset($this->template->{$name})) {
      return $this->template->{$name};
    }
    $trace = debug_backtrace();
    trigger_error(
      'Undefined property via __get(): ' . $name .
      ' in ' . $trace[0]['file'] .
      ' on line ' . $trace[0]['line'],
      E_USER_NOTICE);
    return null;
  }
}