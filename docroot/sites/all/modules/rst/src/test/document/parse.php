<?php

include('../../autoload.php');

use Drupal\rst\Parser;

$parser = new Parser;
$document = $parser->parse(file_get_contents('document.rst'));

echo $document->renderDocument();
