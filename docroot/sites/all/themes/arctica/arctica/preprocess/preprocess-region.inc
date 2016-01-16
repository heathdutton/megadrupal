<?php

// Add .sidebar class to sidebar regions
if ($vars['region'] == 'sidebar_first' OR $vars['region'] == 'sidebar_second') {
  $vars['classes_array'][] = 'sidebar';
}
