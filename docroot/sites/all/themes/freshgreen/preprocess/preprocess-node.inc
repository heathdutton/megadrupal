<?php

function fresh_green_alpha_preprocess_node(&$vars) {
  unset($vars['content']['links']['node']);
}