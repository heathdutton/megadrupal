<?php
namespace Drupal\go1_base\Twig;

class FunctionFetcher extends FilterFetcher {
  protected $config_id  = 'twig_functions';
  protected $config_key = 'twig_functions';
  protected $twig_base  = '\Twig_SimpleFunction';
  protected $wrapper    = '\Drupal\go1_base\Twig\Functions\Wrapper';
}
