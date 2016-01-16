(function($) {
  $.fn.placeholder = function(params) { 
    var $this = $(this);
    $this.val(params['value']);
    $this.focus(function(){
      if(this.value == Drupal.t(params['value'])) {
        this.value='';
      }
    }).blur(function(){
      if(this.value == '') {
        this.value = Drupal.t(params['value']);
      }
    });
  };
  $.fn.clearMinHeight = function() {
    $(this).css('min-height', '0px');
  }
})(jQuery);
