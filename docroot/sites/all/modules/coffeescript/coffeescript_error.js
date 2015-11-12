(function($){
  $(function(){
    var $error = $('<div class="messages error"><?php print $error; ?></div>');
    $error
     .css('display', 'block')
     .css('white-space', 'pre')
     .css('font-family', 'monospace')
     .css('font-size', '8pt')
     .css('line-height', '17px')
     .css('overflow', 'hidden');
    $('body').prepend($error);
  });
})(jQuery);
