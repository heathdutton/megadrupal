<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\ImageNode as Base;

class ImageNode extends Base
{
    public function render()
    {
        $attributes = '';
        foreach ($this->options as $key => $value) {
            $attributes .= ' '.$key . '="'.htmlspecialchars($value).'"';
        }

        return '<img src="'.$this->url.'" '.$attributes.' />';
    }
}
