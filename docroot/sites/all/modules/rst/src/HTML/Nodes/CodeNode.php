<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\CodeNode as Base;

class CodeNode extends Base
{
    public function render()
    {
        return "<pre><code class=\"".$this->language."\">".htmlspecialchars($this->value)."</code></pre>";
    }
}
