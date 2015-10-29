<?php

class millennium_handler_field_year extends views_handler_field {
  function render($values) {
    $biblio_data = unserialize($values->millennium_node_bib_biblio_data);
    $year = 0;
    if (isset($biblio_data["imprint_date"])) {
      $year = intval($biblio_data["imprint_date"]);
    }
    return ($year > 1400) ? "{$year}" : "";
  }
}
