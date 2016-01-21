<?php

namespace Drupal\rst\LaTeX\Nodes;

use Drupal\rst\Nodes\QuoteNode as Base;

class QuoteNode extends Base
{
    public function render()
    {
        return "\\begin{quotation}\n".$this->value."\n\\end{quotation}\n";
    }
}
