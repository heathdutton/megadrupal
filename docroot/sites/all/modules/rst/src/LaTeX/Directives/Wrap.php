<?php

namespace Drupal\rst\LaTeX\Directives;

use Drupal\rst\Parser;
use Drupal\rst\SubDirective;
use Drupal\rst\Nodes\WrapperNode;

/**
 * Wraps a sub document in a div with a given class
 */
class Wrap extends SubDirective
{
    protected $class;

    public function __construct($class)
    {
        $this->class = $class;
    }

    public function getName()
    {
        return $this->class;
    }

    public function processSub(Parser $parser, $document, $variable, $data, array $options)
    {
        return $document;
    }
}
