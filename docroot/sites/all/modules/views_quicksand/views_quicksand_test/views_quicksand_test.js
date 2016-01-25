(function ($) {

  $(document).bind('afterQuicksandAnimation', function(event) {
    var time = new Date();
    console.log(time.getHours() + '-' + time.getMinutes() + '-' +
    time.getSeconds() + '-' + time.getMilliseconds() + ': after');
  });

  $(document).bind('beforeQuicksandAnimation', function(event) {
    var time = new Date();
    console.log(time.getHours() + '-' + time.getMinutes() + '-' +
    time.getSeconds() + '-' + time.getMilliseconds() + ': before');
  });

})(jQuery);
