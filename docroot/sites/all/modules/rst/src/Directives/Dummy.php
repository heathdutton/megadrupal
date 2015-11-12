<?php

namespace Drupal\rst\Directives;

use Drupal\rst\Directive;
use Drupal\rst\Parser;
use Drupal\rst\Nodes\DummyNode;

class Dummy extends Directive
{
    public function getName()
    {
        return 'dummy';
    }

    public function processNode(Parser $parser, $variable, $data, array $options)
    {
        return new DummyNode(array('data' => $data, 'options' => $options));
    }
}
