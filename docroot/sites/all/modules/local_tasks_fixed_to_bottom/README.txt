
Local tasks fixed to bottom
===========================

The Local tasks fixed to bottom module moves the local tasks (node tabs) to
the bottom of the viewport.


Hooks
=====

On rare occasions it might be useful to manually override if this feature
should be enabled on certain pages. For this there's a custom hook
hook_local_tasks_fixed_to_bottom_disable().

Usage example:

/**
 * Implements hook_local_tasks_fixed_to_bottom_disable();
 */
function mymodule_local_tasks_fixed_to_bottom_disable($variables) {
  $arg = array(
    0 => arg(0),
    1 => arg(1),
    2 => arg(2),
  );

  if (isset($arg[0])) {
    if ($arg[0] === 'foo') {
      if (isset($arg[1]) && is_numeric($arg[1]) && isset($arg[2])) {
        $variables['disable'] = TRUE;
      }
    }
  }

  return $variables;
}
