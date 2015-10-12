<?php

/**
 * @file search-block-form.tpl.php
 */

  /* remove ' this site' - probably should try more surefire way using </label> */
  $search["search_block_form"]= str_replace("Search this site:", "", $search["search_block_form"]);

  print $search["search_block_form"];
  print $search["submit"];
  print $search["hidden"]; 