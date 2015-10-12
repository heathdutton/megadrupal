(function ($) {


  Array.prototype.item_delete = function(value) {
    var index = this.indexOf(value);
    if (index != -1) {
      this.splice(index, 1);
    }
  };


  Array.prototype.item_insert_unique = function(value) {
    var index = this.indexOf(value);
    if (index == -1) {
      this.push(value);
    }
  };


  jQuery.fn.extend({
    disableSelection: function() {
      this.each(function() {
        this.onselectstart = function() {return false;};
        this.unselectable = 'on';
        jQuery(this).css({
          '-moz-user-select': 'none',
          '-webkit-user-select': 'none',
          '-ie-user-select': 'none',
          'outline': 'none'
        });
      });
    }
  });


})(jQuery);