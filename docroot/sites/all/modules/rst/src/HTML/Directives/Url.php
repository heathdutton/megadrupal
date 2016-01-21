<?php

namespace Drupal\rst\HTML\Directives;

use Drupal\rst\Directive;
use Drupal\rst\Parser;

/**
 * Sets the document URL
 */
class Url extends Directive
{
    public function getName()
    {
        return 'url';
    }

    public function processAction(Parser $parser, $variabe, $data, array $options)
    {
        $environment = $parser->getEnvironment();
        $environment->setUrl(trim($data));
    }
}
