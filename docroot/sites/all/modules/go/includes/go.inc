<?php
/**
 * @file includes/go.inc
 *
 * Collection of important functions.
 */

/**
 * Helper function for quick cache get/set.
 *
 * @code
 *   $nid = $vid = 1;
 *
 *   $options['ttl'] = '+ 15 minutes';
 *   $options['cache_id'] = "node:{$nid}";
 *   $options['file'] = '/path/to/file.php;
 *   $options['reset'] = FALSE;
 *
 *   // Use case 1:
 *   $data = go_cache($options, function() use ($nid, $vid) {
 *     return node_load($nid, $vid);
 *   });
 *
 *   // Use case 2:
 *   $data = go_cache($options, 'node_load', array($nid, $vid));
 *
 * @endcode
 *
 * If GODEV constant is defined, go_cache() is by passed rebuild the cache if
 * $_GET['nocache'] is not empty.
 *
 * @param  [type] $options
 * @param  Closure|string $callback
 * @param  array  $arguments
 * @return mixed
 * @see GoCacheTestCase
 */
function go_cache($options, $callback, $arguments = array()) {
  // #####################
  // Get/Build arguments
  // #####################
  $bin = !empty($options['bin']) ? $options['bin'] : 'cache';
  $ttl = !empty($options['ttl']) ? $options['ttl'] : '+ 15 minutes';
  $reset = !empty($options['reset']) ? $options['reset'] : FALSE;
  $expire = strtotime($ttl);

  // No cache_id, can not fetch, can not write, this function is useless.
  if (empty($options['cache_id']) || !is_string($options['cache_id'])) {
    throw new Exception('Please provide a valid cache ID');
  }
  $cache_id = $options['cache_id'];

  // Allow dev to force rebuilding all caches on page
  if (defined('GODEV') && !empty($_GET['nocache'])) {
    $reset = TRUE;
  }

  // #####################
  // Fetch the cached data
  // #####################
  if (!$reset) {
    if ($cache = cache_get($cache_id, $bin)) {
      return $cache->data;
    }
  }

  // #####################
  // Build cache data
  // #####################
  if (is_a($callback, 'Closure')) {
    $return = $callback();
  }
  elseif (is_callable($callback)) {
    $return = call_user_func_array($callback, $arguments);
  }

  // #####################
  // Write the cache
  // #####################
  cache_set($cache_id, $return, $bin, $expire);

  return $return;
}

/**
 * Identity function, returns its argument unmodified.
 *
 * This is useful almost exclusively as a workaround to an oddity in the PHP
 * grammar -- this is a syntax error:
 *
 *    COUNTEREXAMPLE
 *    new Thing()->doStuff();
 *
 * ...but this works fine:
 *
 *    id(new Thing())->doStuff();
 *
 * @param   wild Anything.
 * @return  wild Unmodified argument.
 */
function go_id($x) {
  return $x;
}

/**
 * Group a list of objects by the result of some method, similar to how
 * GROUP BY works in an SQL query. This function simplifies grouping objects
 * by some property:
 *
 *    COUNTEREXAMPLE
 *    $animals_by_species = array();
 *    foreach ($animals as $animal) {
 *      $animals_by_species[$animal->getSpecies()][] = $animal;
 *    }
 *
 * This can be expressed more tersely with mgroup():
 *
 *    $animals_by_species = mgroup($animals, 'getSpecies');
 *
 * In either case, the result is a dictionary which maps species (e.g., like
 * "dog") to lists of animals with that property, so all the dogs are grouped
 * together and all the cats are grouped together, or whatever super
 * businessesey thing is actually happening in your problem domain.
 *
 * See also @{function:go_igroup}, which works the same way but operates on
 * array indexes.
 *
 * @param   list    List of objects to group by some property.
 * @param   string  Name of a method, like 'getType', to call on each object
 *                  in order to determine which group it should be placed into.
 * @param   ...     Zero or more additional method names, to subgroup the
 *                  groups.
 * @return  dict    Dictionary mapping distinct method returns to lists of
 *                  all objects which returned that value.
 */
function go_mgroup(array $list, $by /* , ... */) {
  $map = mpull($list, $by);

  $groups = array();
  foreach ($map as $group) {
    // Can't array_fill_keys() here because 'false' gets encoded wrong.
    $groups[$group] = array();
  }

  foreach ($map as $key => $group) {
    $groups[$group][$key] = $list[$key];
  }

  $args = func_get_args();
  $args = array_slice($args, 2);
  if ($args) {
    array_unshift($args, null);
    foreach ($groups as $group_key => $grouped) {
      $args[0] = $grouped;
      $groups[$group_key] = call_user_func_array('mgroup', $args);
    }
  }

  return $groups;
}

/**
 * Group a list of arrays by the value of some index. This function is the same
 * as @{function:go_mgroup}, except it operates on the values of array indexes
 * rather than the return values of method calls.
 *
 * @param   list    List of arrays to group by some index value.
 * @param   string  Name of an index to select from each array in order to
 *                  determine which group it should be placed into.
 * @param   ...     Zero or more additional indexes names, to subgroup the
 *                  groups.
 * @return  dict    Dictionary mapping distinct index values to lists of
 *                  all objects which had that value at the index.
 * @group   util
 */
function go_igroup(array $list, $by /* , ... */) {
  $map = ipull($list, $by);

  $groups = array();
  foreach ($map as $group) {
    $groups[$group] = array();
  }

  foreach ($map as $key => $group) {
    $groups[$group][$key] = $list[$key];
  }

  $args = func_get_args();
  $args = array_slice($args, 2);
  if ($args) {
    array_unshift($args, null);
    foreach ($groups as $group_key => $grouped) {
      $args[0] = $grouped;
      $groups[$group_key] = call_user_func_array('igroup', $args);
    }
  }

  return $groups;
}

/**
 * Sort a list of objects by the return value of some method. In PHP, this is
 * often vastly more efficient than ##usort()## and similar.
 *
 *    // Sort a list of Duck objects by name.
 *    $sorted = msort($ducks, 'getName');
 *
 * It is usually significantly more efficient to define an ordering method
 * on objects and call ##msort()## than to write a comparator. It is often more
 * convenient, as well.
 *
 * NOTE: This method does not take the list by reference; it returns a new list.
 *
 * @param   list    List of objects to sort by some property.
 * @param   string  Name of a method to call on each object; the return values
 *                  will be used to sort the list.
 * @return  list    Objects ordered by the return values of the method calls.
 * @group   util
 */
function go_msort(array $list, $method) {
  $surrogate = mpull($list, $method);

  asort($surrogate);

  $result = array();
  foreach ($surrogate as $key => $value) {
    $result[$key] = $list[$key];
  }

  return $result;
}

/**
 * Sort a list of arrays by the value of some index. This method is identical to
 * @{function:go_msort}, but operates on a list of arrays instead of a list of
 * objects.
 *
 * @param   list    List of arrays to sort by some index value.
 * @param   string  Index to access on each object; the return values
 *                  will be used to sort the list.
 * @return  list    Arrays ordered by the index values.
 * @group   util
 */
function go_isort(array $list, $index) {
  $surrogate = ipull($list, $index);

  asort($surrogate);

  $result = array();
  foreach ($surrogate as $key => $value) {
    $result[$key] = $list[$key];
  }

  return $result;
}

/**
 * Checks if all values of array are instances of the passed class.
 * Throws InvalidArgumentException if it isn't true for any value.
 *
 * @param  array
 * @param  string  Name of the class or 'array' to check arrays.
 * @return array   Returns passed array.
 * @group   util
 */
function go_assert_instances_of(array $arr, $class) {
  $is_array = !strcasecmp($class, 'array');

  foreach ($arr as $key => $object) {
    if ($is_array) {
      if (!is_array($object)) {
        $given = gettype($object);
        throw new InvalidArgumentException(
          "Array item with key '{$key}' must be of type array, ".
          "{$given} given.");
      }

    } else if (!($object instanceof $class)) {
      $given = gettype($object);
      if (is_object($object)) {
        $given = 'instance of '.get_class($object);
      }
      throw new InvalidArgumentException(
        "Array item with key '{$key}' must be an instance of {$class}, ".
        "{$given} given.");
    }
  }

  return $arr;
}

/**
 * Invokes the "new" operator with a vector of arguments. There is no way to
 * call_user_func_array() on a class constructor, so you can instead use this
 * function:
 *
 *   $obj = newv($class_name, $argv);
 *
 * That is, these two statements are equivalent:
 *
 *   $pancake = new Pancake('Blueberry', 'Maple Syrup', true);
 *   $pancake = newv('Pancake', array('Blueberry', 'Maple Syrup', true));
 *
 * DO NOT solve this problem in other, more creative ways! Three popular
 * alternatives are:
 *
 *   - Build a fake serialized object and unserialize it.
 *   - Invoke the constructor twice.
 *   - just use eval() lol
 *
 * These are really bad solutions to the problem because they can have side
 * effects (e.g., __wakeup()) and give you an object in an otherwise impossible
 * state. Please endeavor to keep your objects in possible states.
 *
 * If you own the classes you're doing this for, you should consider whether
 * or not restructuring your code (for instance, by creating static
 * construction methods) might make it cleaner before using newv(). Static
 * constructors can be invoked with call_user_func_array(), and may give your
 * class a cleaner and more descriptive API.
 *
 * @param  string  The name of a class.
 * @param  list    Array of arguments to pass to its constructor.
 * @return obj     A new object of the specified class, constructed by passing
 *                 the argument vector to its constructor.
 * @group util
 */
function go_newv($class_name, array $argv) {
  $reflector = new ReflectionClass($class_name);
  if ($argv) {
    return $reflector->newInstanceArgs($argv);
  } else {
    return $reflector->newInstance();
  }
}

function go_is_windows() {
  // We can also use PHP_OS, but that's kind of sketchy because it returns
  // "WINNT" for Windows 7 and "Darwin" for Mac OS X. Practically, testing for
  // DIRECTORY_SEPARATOR is more straightforward.
  return (DIRECTORY_SEPARATOR != '/');
}
