<?php

namespace Drupal\rst\References;

use Drupal\rst\Reference;
use Drupal\rst\Environment;

class Doc extends Reference
{
    protected $name;

    public function __construct($name = 'doc')
    {
        $this->name = $name;
    }

    public function getName()
    {
        return $this->name;
    }

    public function resolve(Environment $environment, $data)
    {
        $metas = $environment->getMetas();
        $file = $environment->canonicalUrl($data);

        if ($metas) {
            $entry = $metas->get($file);
            $entry['url'] = $environment->relativeUrl('/'.$entry['url']);
        } else {
            $entry = array(
                'title' => '(unresolved)',
                'url' => '#'
            );
        }

        return $entry;
    }

    public function found(Environment $environment, $data)
    {
        $environment->addDependency($data);
    }
}
