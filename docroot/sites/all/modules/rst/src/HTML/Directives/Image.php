<?php

namespace Drupal\rst\HTML\Directives;

use Drupal\rst\Parser;
use Drupal\rst\Directive;

use Drupal\rst\HTML\Nodes\ImageNode;

/**
 * Renders an image, example :
 *
 * .. image:: image.jpg
 *      :width: 100
 *      :title: An image
 *
 */
class Image extends Directive
{
    public function getName()
    {
        return 'image';
    }

    public function processNode(Parser $parser, $variable, $data, array $options)
    {
        $environment = $parser->getEnvironment();
        $url = $environment->relativeUrl($data);

        return new ImageNode($url, $options);
    }
}
