<?php

class millennium_handler_field_type extends views_handler_field {
  function render($values) {
    $biblio_data = unserialize($values->millennium_node_bib_biblio_data);
    return _millennium_human_string($biblio_data, 'type');
  }
}
