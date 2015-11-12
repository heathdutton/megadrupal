<?php

namespace Drupal\rst\LaTeX;

use Drupal\rst\Kernel as Base;

class Kernel extends Base
{
    function getName()
    {
        return 'LaTeX';
    }

    public function getDirectives()
    {
        $directives = parent::getDirectives();

        $directives = array_merge($directives, array(
            new Directives\LaTeXMain,
            new Directives\Image,
            new Directives\Meta,
            new Directives\Stylesheet,
            new Directives\Title,
            new Directives\Url,
            new Directives\Wrap('note')
        ));

        return $directives;
    }

    public function getFileExtension()
    {
        return 'tex';
    }
}
