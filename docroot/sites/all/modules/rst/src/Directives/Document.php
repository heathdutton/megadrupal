<?php

namespace Drupal\rst\Directives;

use Drupal\rst\Parser;
use Drupal\rst\Directive;

use Drupal\rst\Nodes\DocumentNode;

/**
 * Tell that this is a document, in the case of LaTeX for instance,
 * this will mark the current document as one of the master document that
 * should be compiled
 */
class Document extends Directive
{
    public function getName()
    {
        return 'document';
    }

    public function processNode(Parser $parser, $variable, $data, array $options)
    {
        return new DocumentNode;
    }
}
