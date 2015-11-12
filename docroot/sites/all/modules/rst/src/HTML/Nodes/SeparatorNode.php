<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\SeparatorNode as Base;

class SeparatorNode extends Base
{
    public function render()
    {
        return '<hr />';
    }
}
