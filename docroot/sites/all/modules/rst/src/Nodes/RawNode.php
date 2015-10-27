<?php

namespace Drupal\rst\Nodes;

class RawNode extends Node
{
    public function render()
    {
        return $this->value;
    }
}
