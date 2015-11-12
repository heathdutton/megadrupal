<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\TitleNode as Base;

class TitleNode extends Base
{
    public function render()
    {
        return '<a id="'.$this->token.'"></a><h'.$this->level.'>'.$this->value.'</h'.$this->level.">";
    }
}
