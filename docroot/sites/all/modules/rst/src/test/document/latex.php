<?php

include('../../autoload.php');

use Drupal\rst\Parser;
use Drupal\rst\LaTeX\Kernel;

$parser = new Parser(null, new Kernel);
$document = $parser->parse(file_get_contents('document.rst'));

echo $document->renderDocument();
