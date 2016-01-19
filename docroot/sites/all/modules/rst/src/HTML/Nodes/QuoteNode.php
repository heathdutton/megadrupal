<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\QuoteNode as Base;

class QuoteNode extends Base
{
    public function render()
    {
        return "<blockquote>".$this->value."</blockquote>";
    }
}
