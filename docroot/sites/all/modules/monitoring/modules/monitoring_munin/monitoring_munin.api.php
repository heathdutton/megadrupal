<?php
/**
 * @file
 * Munin API.
 */

function hook_munin_default_multigraphs() {
    return array(
      // The array keys are multigraph titles.
      'My multigraph' => array(
        // Munin vertical label for units. This is required only if you want the
        // multigraph get created.
        'vlabel' => 'My units',
        // Sensor categories from which all sensors should belong to the defined
        // multigraph.
        'categories' => array(),
        // Individual sensors that should belong to the multigraph.
        'sensors' => array(),
      ),
    );
}
