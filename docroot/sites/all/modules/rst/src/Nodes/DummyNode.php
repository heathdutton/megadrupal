<?php

namespace Drupal\rst\Nodes;

class DummyNode extends Node
{
    public $data;

    public function __construct($data)
    {
        $this->data = $data;
    }

    public function render()
    {
        return '';
    }
}
