<?php

namespace Drupal\rst\LaTeX\Nodes;

use Drupal\rst\Nodes\AnchorNode as Base;

class AnchorNode extends Base
{
    public function render()
    {
        return '\label{'.$this->value.'}';
    }
}
