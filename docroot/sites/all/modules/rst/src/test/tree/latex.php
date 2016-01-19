<?php

include('../../autoload.php');

use Drupal\rst\Builder;
use Drupal\rst\LaTeX\Kernel;

try
{
    // Build the 'input' files to the 'output' directory
    $builder = new Builder(new Kernel);
    $builder->copy('css', 'css');
    $builder->build('input', 'output');
}
catch (\Exception $exception)
{
    echo "\n";
    echo "Error: ".$exception->getMessage()."\n";
}
