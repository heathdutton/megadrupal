<?php

namespace Drupal\rst\LaTeX\Directives;

use Drupal\rst\LaTeX\Nodes\LaTeXMainNode;
use Drupal\rst\Parser;
use Drupal\rst\Directive;

/**
 * Marks the document as LaTeX main
 */
class LaTeXMain extends Directive
{
    public function getName()
    {
        return 'latex-main';
    }

    public function processNode(Parser $parser, $variable, $data, array $options)
    {
        return new LaTeXMainNode;
    }
}
