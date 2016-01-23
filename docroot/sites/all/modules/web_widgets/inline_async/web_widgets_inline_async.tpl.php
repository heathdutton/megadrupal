<?php
/**
 * @file
 * Template for the code what to embed in external sites. Loads it asynchronously.
 */
?>
<script type="text/javascript">
(function(d, s) {
  var wid = '<?php print $wid; ?>',
  js, load = function(url, id) {
    if (d.getElementById(id)) {return;}
    js = d.createElement(s); js.src = url; js.id = id;
    d.getElementsByTagName('head')[0].appendChild(js);
  };
  var old = function(){};
  if (typeof window.onload == 'function') {
    old = window.onload;
  }
  window.onload = function(e) { old(e); load('<?php print $script_url ?>', 'script_'+ wid); };
}(document, 'script'));
</script>
<div id="<?php print $wid ?>"></div>