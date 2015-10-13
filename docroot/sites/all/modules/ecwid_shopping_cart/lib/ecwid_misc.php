<?php
function ecwid_page_url () {

    $port = ($_SERVER['SERVER_PORT'] == 80 ?  "http://" : "https://");

    $parts = parse_url($_SERVER['REQUEST_URI']);

    $queryParams = array();
    parse_str($parts['query'], $queryParams);
    unset($queryParams['_escaped_fragment_']);

    $queryString = http_build_query($queryParams);
    $url = $parts['path'] . '?' . $queryString;

    return $port . $_SERVER['HTTP_HOST'] . $url;
}

function ecwid_prepare_meta_description($description) {
    if (empty($description)) {
          return "empty";
    }

    $description = strip_tags($description);
    $description = html_entity_decode($description, ENT_NOQUOTES, 'UTF-8');
    $description = preg_replace("![\\s]+!", " ", $description);
    $description = trim($description, " \t\xA0\n\r"); // Space, tab, non-breaking space, newline, carriage return  
    $description = mb_substr($description, 0, 160);
    $description = htmlspecialchars($description, ENT_COMPAT, 'UTF-8');

    return $description;
}
