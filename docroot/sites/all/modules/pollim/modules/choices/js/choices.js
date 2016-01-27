
(function ($) {
  Drupal.behaviors.choices = {
    attach: function (context) {
      $('.entity-pollim input.choice', context).click(function() {
        var $this = $(this);
        var $container = $this.closest('.entity-pollim');
        var entityId = $this.attr('entityid');
        var url = Drupal.settings['basePath'] + "pollim/ajax/vote/" + entityId;
        var value = $this.val();
        $.cookie('pi-' + entityId, value, { expires: 31, path: '/' });
        $.post(url, {data: value}, function(response) {
          Drupal.detachBehaviors($container);
          $container.replaceWith($(response));
          processRadioButtons($('body'));
        });
      });
      processRadioButtons(context);
    }
  };
  
  var processRadioButtons = function(context) {
    $('.entity-pollim input.choice', context).each(function() {
        var $this = $(this);
        if ($this.attr('entityid')) {
          $this.css('visibility', 'visible');
          var $container = $this.closest('.entity-pollim');
          var cookieVal = $.cookie('pi-' + $this.attr('entityid'));
          
          if (cookieVal) {
            $container.addClass('voted');
            $this.attr('disabled', 'disabled');
            $('input.choice[value=' +cookieVal+ ']', $this.closest('.entity-pollim')).attr('checked', 'checked');
          }
        }
    });
  }
  
      
})(jQuery);
