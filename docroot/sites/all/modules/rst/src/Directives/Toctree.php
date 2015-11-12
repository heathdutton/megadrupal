<?php

namespace Drupal\rst\Directives;

use Drupal\rst\Directive;
use Drupal\rst\Parser;

class Toctree extends Directive
{
    public function getName()
    {
        return 'toctree';
    }

    public function process(Parser $parser, $node, $variable, $data, array $options)
    {
        $environment = $parser->getEnvironment();
        $kernel = $parser->getKernel();
        $files = array();

        foreach (explode("\n", $node->getValue()) as $file) {
            $file = trim($file);
            if ($file) {
                $environment->addDependency($file);
                $files[] = $file;
            }
        }

        $document = $parser->getDocument();
        $document->addNode($kernel->build('Nodes\TocNode', $files, $environment, $options));
    }

    public function wantCode()
    {
        return true;
    }
}
