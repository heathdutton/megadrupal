<?php

namespace Drupal\rst\HTML\Nodes;

use Drupal\rst\Nodes\ListNode as Base;

class ListNode extends Base
{
    protected function createElement($text, $prefix)
    {
        $class = '';
        if ($prefix == '-') {
            $class = ' class="dash"';
        }

        return '<li' . $class . '>' . $text . '</li>';
    }

    protected function createList($ordered)
    {
        $keyword = $ordered ? 'ol' : 'ul';

        return array('<'.$keyword.'>', '</'.$keyword.'>');
    }
}
