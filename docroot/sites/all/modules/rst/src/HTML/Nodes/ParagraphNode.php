<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\ParagraphNode as Base;

class ParagraphNode extends Base
{
    public function render()
    {
        $text = $this->value;

        if (trim($text)) {
            return '<p>'.$text.'</p>';
        } else {
            return '';
        }
    }
}
