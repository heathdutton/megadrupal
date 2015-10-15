/**
 * Implements hook_block_view_MODULE_DELTA_alter().
 */
function <?php print $basename; ?>_block_view_MODULE_DELTA_alter(&\$data, \$block) {
  ${1:// This code will only run for a specific block. For example, if MODULE_DELTA
  // in the function definition above is set to "mymodule_somedelta", the code
  // will only run on the "somedelta" block provided by the "mymodule" module.

  // Change the title of the "somedelta" block provided by the "mymodule"
  // module.
  \$data['subject'] = t('New title of the block');}
}

$2