(function ($) {
Drupal.behaviors.blockPurge = {
  attach: function (context) {
  jQuery.each(_cp, function (i, item) {
    var module = item.module;
    var delta = item.delta;
    var block_id = module + '-' + delta;
    var purge_btn = '<div class="cache-purge-btn" title="Purge Cache: ' + block_id +
    				'" id="cache-purge-btn-' + block_id +
    				'" onclick="cache_purge_block(\'' + module + '\', \'' + delta + '\', \'' + _cp_d + '\');"></div>';
    var id = '#block-' + block_id;
    var cl = '.block-' + block_id;
    jQuery(id + ', ' + cl).append(purge_btn);
   });
  }
};

})(jQuery);

function cache_purge_block(module, delta, debug) {
  var cssPrefix = false;
  var block_id = module + '-' + delta;
      
  if (jQuery.browser.webkit) {
    cssPrefix = "webkit";
  }
  if (jQuery.browser.safari) {
    cssPrefix = "webkit";
  }
  if (jQuery.browser.mozilla) {
    cssPrefix = "moz";
  }
  if (jQuery.browser.opera) {
    cssPrefix = "o";
  }
  if (jQuery.browser.msie) {
    cssPrefix = "ms";
  }

  if(cssPrefix != false) {
    var purge_btn = document.getElementById('cache-purge-btn-' + block_id), degrees = 0, speed = 5;
    var spinner = setInterval(function() {
      degrees += speed;
      purge_btn.setAttribute("style","-" + cssPrefix + "-transform:rotate(" + degrees + "deg)");
    },5);
  }
  jQuery.getJSON("/admin/config/development/cachepurger/block/" + module + "/" + delta, {}, function(data) {
    clearInterval(spinner);
    if (debug != 0) {
      alert(data.result);
    }
    jQuery('#cache-purge-btn-' + block_id).fadeOut('slow');
  });
}
