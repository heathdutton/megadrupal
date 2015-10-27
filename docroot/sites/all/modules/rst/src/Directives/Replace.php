<?php

namespace Drupal\rst\Directives;

use Drupal\rst\Span;
use Drupal\rst\Parser;
use Drupal\rst\Directive;

/**
 * The Replace directive will set the variables for the spans
 *
 * .. |test| replace:: The Test String!
 */
class Replace extends Directive
{
    public function getName()
    {
        return 'replace';
    }

    public function processNode(Parser $parser, $variable, $data, array $options)
    {
        return $parser->createSpan($data);
    }
}
