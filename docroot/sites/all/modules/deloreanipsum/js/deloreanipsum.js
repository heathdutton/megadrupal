(function ($) {
  Drupal.behaviors.deloreanipsum = {
    attach: function(context, settings) {
      $('.deloreanipsum:not(.deloreanipsum-processed)').each(function() {
        var textarea = this;
        var img = $('<img />');
        img.attr({
          src: Drupal.settings.deloreanipsum.imgpath
        });
        var a = $('<a />', {
          title: "DeLorean Ipsum",
          class: 'deloreanlink',
          href: '#',
          click: function() {
            var tmp = $('<textarea />');
            tmp.delorean();
            $(textarea).val($(textarea).val() + tmp.val());
            tmp = undefined;
            $(textarea).focus();
            width = $(textarea).width()-45;
            $(this).animate({"left": '+='+width+'px'}, '1000', 'swing', function() {
              $(this).animate({'left': '-='+width+'px'});
            });
          }
        }).append(img).insertAfter(textarea);
        $(textarea).addClass('deloreanipsum-processed');
      });
    }
  }
})(jQuery);

