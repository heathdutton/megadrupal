<?php

namespace Drupal\rst\LaTeX\Directives;

use Drupal\rst\Parser;
use Drupal\rst\Directive;

use Drupal\rst\Nodes\RawNode;

/**
 * Adds a stylesheet to a document, example:
 *
 * .. stylesheet:: style.css
 */
class Stylesheet extends Directive
{
    public function getName()
    {
        return 'stylesheet';
    }

    public function process(Parser $parser, $node, $variable, $data, array $options)
    {
    }
}
