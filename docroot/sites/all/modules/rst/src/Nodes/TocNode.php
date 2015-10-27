<?php

namespace Drupal\rst\Nodes;

use Drupal\rst\Environment;

abstract class TocNode extends Node
{
    protected $files;
    protected $environment;
    protected $options;

    public function __construct(array $files, Environment $environment, array $options)
    {
        $this->files = $files;
        $this->environment = $environment;
        $this->options = $options;
    }

    public function getFiles()
    {
        return $this->files;
    }
}
