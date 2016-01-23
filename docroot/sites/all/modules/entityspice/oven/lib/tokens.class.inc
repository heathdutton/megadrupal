<?php

class __OvenTokenizer {
  private $text;
  private $tokens;
  private $resultText;

  public function __construct($text, array $tokens) {
    $this->setText($text);
    $this->setTokens($tokens);
  }

  public function setText($text) {
    $this->text = $text;
    $this->resultText = $this->text;
  }

  public function setTokens(array $tokens) {
    $this->tokens = $tokens;
  }

  public function getText() {
    $this->ekran();
    return $this->resultText;
  }

  private function ekran() {
    $text = $this->text;
    foreach ($this->tokens as $token) {
      $with = $token->value();
      $what = OVEN_OPENING_TOKEN . $token->get('token') . OVEN_CLOSING_TOKEN;
      echo "\n\n\n_________________________________\n\n";
      echo "\nwhat:\t" . $what;
      echo "\nwith:\t" . $with;
      $text = str_replace($what, $with, $text);
    }

    $this->resultText = $text;
  }
}

class __OvenTokenInfoName {
  private static $tokens = [
    'name' => [
      'token' => 'PLACEHOLDER::files:info:name',
      'name' => 'name',
      'label' => 'Name',
      'description' => 'Name of module in .info file.',
      'validation' => 'name',
      'default policy' => 'machine',
    ],
    'description' => [
      'token' => 'PLACEHOLDER::files:info:description',
      'name' => 'description',
      'label' => 'Description',
      'description' => 'Description of module in .info file.',
      'validation' => 'text',
      'default policy' => 'empty',
    ],
    'package' => [
      'token' => 'PLACEHOLDER::files:info:package',
      'name' => 'package',
      'label' => 'Package',
      'description' => 'package to which this module belongs (In modules page) for .info file.',
      'validation' => 'name',
      'default policy' => '_oven_default_package',
    ],
    'machine_camel' => [
      'token' => 'PLACEHOLDER::name:machine_camel',
      'name' => 'machine_camel',
      'label' => 'MachineReadableCameCaseName',
      'description' => 'Must match the regular machine name!',
      'validation' => 'name',
      'default policy' => 'machine camel',
    ],
    'default_uri' => [
      'token' => 'PLACEHOLDER::path:default_uri',
      'name' => 'default_uri',
      'label' => 'Default URI',
      'description' => 'Default path from which the entity can be accessed. IMPORTANT: currently this must be a two part menu path, such as xxxx/yyyyyyyy/ with final slash.',
      'validation' => 'uri',
      'default policy' => '_oven_default_uri',
    ],
    'label' => [
      'token' => 'PLACEHOLDER::name:label',
      'name' => 'label',
      'label' => 'Label',
      'description' => 'Label of entity displayed in admin pages.',
      'validation' => 'label',
      'default policy' => 'machine',
    ],
    'camel_label' => [
      'token' => 'PLACEHOLDER::name:camel_label',
      'name' => 'camel_label',
      'label' => 'CamelLabel',
      'description' => 'Label of entity displayed in CamelCase.',
      'validation' => 'label',
      'default policy' => 'machine camel',
    ],
    'admin_bundle' => [
      'token' => 'PLACEHOLDER::path:admin_bundle',
      'name' => 'admin_bundle',
      'label' => 'Bundle administration page',
      'description' => 'This is a URI relative to admin/ path. Do not include admin/',
      'process callback' => [
        '_oven_arg_process_callback',
      ],
      'validation' => 'uri',
      'default policy' => '_oven_admin_bundle_uri',
    ],
    'delete_form_redirect' => [
      'token' => 'PLACEHOLDER::path:delete_form_redirect',
      'name' => 'delete_form_redirect',
      'label' => 'Redirect path from entity delete form',
      'description' => "When an entity is deleted, It's path will become invalid and user should be redirected to a new path.",
      'validation' => 'uri',
      'default policy' => '_oven_form_redirect_uri',
    ],
    'menu_label' => [
      'token' => 'PLACEHOLDER::name:menu_label',
      'name' => 'menu_label',
      'label' => 'Label in menu',
      'description' => '',
      'validation' => 'label',
      'default policy' => 'machine',
    ],
    'machine' => [
      'token' => 'PLACEHOLDER::name:machine',
      'name' => 'machine',
      'label' => 'Machine Name',
      'description' => "Machine readable name of module (and it's entity), must be only letters (small caps), numbers and underscores. It also shouldn't start with a number.",
      'validation' => 'machineName',
      'default policy' => 'null',
    ],
    'has_revision' => [
      'token' => 'PLACEHOLDER::cfg:has_revision',
      'name' => 'has_revision',
      'label' =>  'Has revision',
      'description' => 'Whether entity type supports revisioning or not (TRUE/FALSE).',
      'validation' => 'booleanText',
      'default policy' => 'boolean TRUE text',
    ],
    'has_bundle' => [
      'token' => 'PLACEHOLDER::cfg:has_bundle',
      'name' => 'has_bundle',
      'label' =>  'Has bundle',
      'description' => 'Whether entity type supports bundle or not (TRUE/FALSE).',
      'validation' => 'booleanText',
      'default policy' => 'boolean TRUE text',
    ],
  ];

  private $tokenInfo;
  private $value;
  private $validator;
  private $name;

  public static function getTokens($with_machine_token) {
    $tokens = self::$tokens;
    if(!$with_machine_token) {
      unset($tokens['machine']);
    }

    return array_keys($tokens);
  }

  public static function getTokensObject($with_machine_token) {
    $names = self::getTokens($with_machine_token);
    $tokens = [];

    foreach ($names as $name) {
      $tokens[$name] = new __OvenTokenInfoName($name);
    }

    return $tokens;
  }

  public static function validateFor($name, $value) {
    if (!isset(__OvenTokenInfoName::$tokens[$name])) {
      throw new Exception('Unknown token: ' . $name);
    }
    else {
      $validator = &__OvenTokenInfoName::$tokens[$name]['validation'];
    }

    $validator = new __OvenValidator($value, $validator);
    return $validator->validate();
  }

  public function __construct($name, $value=NULL) {
    if (!isset(__OvenTokenInfoName::$tokens[$name])) {
      throw new Exception('Unknown token: ' . $name);
    }
    else {
      $this->tokenInfo = &__OvenTokenInfoName::$tokens[$name];
    }

    $this->name = $name;

    $validator = $this->tokenInfo['validation'];
    $this->validator = new __OvenValidator($value, $validator);

    $this->set($value);
  }

  public function set($value) {
    $this->value = $value;
    $this->validator->setValue($value);
  }

  public function value() {
    return $this->value;
  }

  public function get($name) {
    if(!isset($this->tokenInfo[$name])) {
      throw new Exception('Unknown token property: ' . $name);
    }
    return $this->tokenInfo[$name];
  }

  public function validate() {
    return $this->validator->validate();
  }

  public function defaultValue() {
    switch ($this->tokenInfo['default policy']) {
      case 'null':
        $def = NULL;
        break;

      case 'machine':
        $def = __OvenMachineName::get();
        break;

      case 'empty':
        $def = ' ';
        break;

      case 'machine camel':
        $def = _oven_get_camel_case(__OvenMachineName::get());
        break;

      case 'boolean TRUE text':
        $def = 'TRUE';
        break;

      case 'boolean FALSE text':
        $def = 'FALSE';
        break;

      default:
        $callback = $this->tokenInfo['default policy'];
        $def = $callback(__OvenMachineName::get());
        break;
    }

    return $def;
  }

  public function processValue() {

  }
}

class __OvenMachineName {
  private static $value = NULL;

  public static function get() {
    $v = __OvenMachineName::$value;
    if(!$v) {
      throw new __OvenMachineNameNotSet();
    }

    return $v;
  }

  public static function set($name) {
    __OvenMachineName::$value = $name;
  }
}

class __OvenValidator {
  // And class constants were PHP 5. .....4? 3? I don't have access to internet
  // right now :)
  const E_NO_ERROR = 0;
  const E_NONE_MACHINE = 2;
  const E_NONE_ALPHANUM = 4;
  const E_EMPTY = 8;
  const E_TOO_LONG = 16;
  const E_BAD_URI = 32;
  const E_NONE_LABEL = 64;
  const E_INVALID = 128;

  private $machineNameRegex = ['#[^a-z0-9_]+#', '#^[^a-z]+#'];
  private $nameRegex  = ['#[^a-zA-Z0-9_]+#'];
  private $labelRegex = ['#[^a-zA-Z0-9_ ]+#'];
  private $uriRegex   = ['#[^a-zA-Z0-9_/]+#', '#^[^/]^[^/]$#'];
  private $maxNameLen = 64;

  private $value;
  private $criteria = NULL;

  public function __construct($value=NULL, $default_criteria=NULL) {
    $this->setValue($value);
    if ($default_criteria) {
      $this->setDefaultCriteria($default_criteria);
    }
  }

  public function setValue($value) {
    $this->value = $value;
  }

  public function setDefaultCriteria($criteria) {
    $this->criteriaValidate($criteria, TRUE);
    $this->criteria = $criteria;
  }

  public function validate($criteria=NULL) {
    $criteria = $this->criteriaValidate($criteria ? $criteria : $this->criteria, TRUE);
    $ok = $this->{$criteria}();
    return $ok;
  }

  public function machineName() {
    $ok = $this->strLenRegErr($this->machineNameRegex, __OvenValidator::E_NONE_MACHINE);
    return $ok;
  }

  public function booleanText() {
    $ok = $this->value === 'TRUE' || $this->value === 'FALSE';
    return $ok ? $this::E_NO_ERROR : $this::E_INVALID;
  }

  public function name() {
    return $this->strLenRegErr($this->nameRegex, __OvenValidator::E_NONE_ALPHANUM);
  }

  public function label() {
    return $this->strLenRegErr($this->labelRegex, __OvenValidator::E_NONE_LABEL);
  }

  public function uri() {
    return $this->strRegErr($this->uriRegex, __OvenValidator::E_BAD_URI);
  }

  public function text() {
    return $this->strRegErr($this->labelRegex, __OvenValidator::E_NONE_LABEL);
  }

  private function strRegErr($regex, $regex_err) {
    if($err = $this->validateString()) {
      return $err;
    }
    if ($err = $this->validateLength()) {
      return $err;
    }
    if (!$this->validateRegex($regex)) {
      return $regex_err;
    }

    return __OvenValidator::E_NO_ERROR;
  }

  private function strLenRegErr($regex, $regex_err) {
    $err = $this->validateLength();
    return $err ? $err : $this->strRegErr($regex, $regex_err);
  }

  private function validateRegex(array $regex) {
    foreach ($regex as $r) {
      if(preg_match($r, $this->value)) {
        return FALSE;
      }
    }

    return TRUE;
  }

  private function validateString() {
    if(is_string($this->value) && !empty($this->value)) {
      return __OvenValidator::E_NO_ERROR;
    }

    return __OvenValidator::E_EMPTY;
  }

  private function validateLength() {
    if(strlen($this->value) < $this->maxNameLen) {
      return __OvenValidator::E_NO_ERROR;
    }

    return __OvenValidator::E_TOO_LONG;
  }

  private function criteriaValidate($criteria, $throw_exception) {
    $available = ['text', 'name', 'label', 'uri',  'machineName', 'booleanText'];
    $machine = ['machine', 'machine name', 'machinename', 'machine_name',
      'mname'];

    $criteria = in_array($criteria, $machine) ? 'machineName' : $criteria;
    $ok = in_array($criteria, $available);

    if(!$ok && $throw_exception) {
      throw new Exception('Unknown validation criteria');
    }

    return $ok ? $criteria : NULL;
  }
}
