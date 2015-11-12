<?php

namespace Drupal\rst\LaTeX\Nodes;

use Drupal\rst\Nodes\ParagraphNode as Base;

class ParagraphNode extends Base
{
    public function render()
    {
        $text = $this->value;

        if (trim($text)) {
            return $text."\n";
        } else {
            return '';
        }
    }
}
