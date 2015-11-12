<?php

namespace Drupal\rst\HTML\Directives;

use Drupal\rst\Parser;
use Drupal\rst\SubDirective;
use Drupal\rst\Nodes\WrapperNode;

/**
 * Divs a sub document in a div with a given class
 */
class Div extends SubDirective
{
    public function getName()
    {
        return 'div';
    }

    public function processSub(Parser $parser, $document, $variable, $data, array $options)
    {
        return new WrapperNode($document, '<div class="'.$data.'">', '</div>');
    }
}
