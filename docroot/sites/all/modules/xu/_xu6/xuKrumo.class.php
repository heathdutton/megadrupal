<?php
/**
 * Override the krumo class to properly handle line & file information for
 * calls via the xu module.
 *
 * @see class.krumo.php in the devel module
 */
class xuKrumo extends krumo {
  /**
   * Dump information about a variable
   *
   * @param mixed $data,...
   * @access public
   * @static
   */
  static Function dump($data) {
    // disabled ?
    //
    if (!krumo::_debug()) {
      return false;
    }

    // more arguments ?
    //
    if (func_num_args() > 1) {
      $_ = func_get_args();
      foreach($_ as $d) {
        krumo::dump($d);
      }
      return;
    }

    // the css ?
    //
    krumo::_css();

    // find caller

    // DEVEL: we added array_reverse() so the proper file+line number is found.
    // xu: we added xu as well
    $_ = array_reverse(debug_backtrace());
    while($d = array_pop($_)) {
      if ((strpos(@$d['function'], 'xu') === FALSE) &&
        (strpos(@$d['file'], 'debug') === FALSE) &&
        (strpos(@$d['file'], 'devel') === FALSE) &&
        (strpos(@$d['file'], 'krumo') === FALSE) &&
        @$d['class'] != 'krumo') {
        break;
      }
    }

    // the content
    //
    ?>
  <div class="krumo-root" dir="ltr">
    <ul class="krumo-node krumo-first">
      <?php echo krumo::_dump($data);?>
      <li class="krumo-footnote">
        <div class="krumo-version" style="white-space:nowrap;">
          <h6>Krumo version <?php echo krumo::version();?></h6> | <a
          href="http://krumo.sourceforge.net"
          target="_blank">http://krumo.sourceforge.net</a>
        </div>

        <?php if (isset($d['file'])) { ?>
        <span class="krumo-call" style="white-space:nowrap;">
			Called from <code><?php echo $d['file']?></code>,
				line <code><?php echo $d['line']?></code></span>
        <?php } ?>
        &nbsp;
      </li>
    </ul>
  </div>
  <?php
    // flee the hive
    //
    $_recursion_marker = krumo::_marker();
    if ($hive =& krumo::_hive($dummy)) {
      foreach($hive as $i=>$bee){
        if (is_object($bee)) {
          unset($hive[$i]->$_recursion_marker);
        } elseif(is_array($hive[$i])) {
          unset($hive[$i][$_recursion_marker]);
        }
      }
    }

    // PHP 4.x.x array reference bug...
    //
    if (is_array($data) && version_compare(PHP_VERSION, "5", "<")) {
      unset($GLOBALS[krumo::_marker()]);
    }
  }

}
