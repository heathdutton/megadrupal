<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\AnchorNode as Base;

class AnchorNode extends Base
{
    public function render()
    {
        return '<a id="'.$this->value.'"></a>';
    }
}
