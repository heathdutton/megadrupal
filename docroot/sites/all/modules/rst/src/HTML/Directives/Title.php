<?php

namespace Drupal\rst\HTML\Directives;

use Drupal\rst\Parser;
use Drupal\rst\Directive;

use Drupal\rst\Nodes\RawNode;

/**
 * Add a meta title to the document
 *
 * .. title:: Page title
 */
class Title extends Directive
{
    public function getName()
    {
        return 'title';
    }

    public function process(Parser $parser, $node, $variable, $data, array $options)
    {
        $document = $parser->getDocument();

        $document->addHeaderNode(new RawNode('<title>'.htmlspecialchars($data).'</title>'));

        if ($node) {
            $document->addNode($node);
        }
    }
}
