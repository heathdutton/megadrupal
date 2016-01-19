(function($) {

  var original_add = jQuery.fn.addClass;
  jQuery.fn.addClass = function() {
    var result = original_add.apply(this, arguments);
    jQuery(this).trigger('classAdded', arguments);
    return result;
  }

  var original_removed = jQuery.fn.removeClass;
  jQuery.fn.removeClass = function() {
    var result = original_removed.apply(this, arguments);
    jQuery(this).trigger('classRemoved', arguments);
    return result;
  }

})(jQuery);