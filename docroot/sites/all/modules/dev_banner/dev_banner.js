(function ($) {

  /**
   * Initialization stuff - executed when js is loaded
   */
  Drupal.behaviors.dev_bannerStartup = {
    attach: function (context, settings) {
    var banner = Drupal.settings.dev_banner.banner;
    var position = Drupal.settings.dev_banner.position;
    var sticky = '';
    if (Drupal.settings.dev_banner.sticky) {
      sticky = ' stickiness ';
    }

    $('body').prepend('<div id="dev-banner" class="' + position + sticky + '">' + banner + '</div>');
    }
  }
  
  /**
   * used on config page - change of position select
   */
 Drupal.behaviors.dev_bannerConfigChangePosition = {
    attach: function (context, settings) {
      $('#edit-dev-banner-position').change(
        function() {
          // show updating status
          $('.form-item-dev-banner-position div.description').css('display', 'inline');
          // get position value from select list
          var pos = $("#edit-dev-banner-position option:selected").val();
          $.ajax({
            type: "GET",
            url: Drupal.settings.basePath + 'dev_banner/refresh/' + pos, 
            async: false,
            dataType: "json",
            success: function(data) {
              var offset = 0;
              for (var idx in data) {
                if (offset < 999) {
                  //alert(offset + ' : ' + idx + ' => ' + data[idx]);
                  $(idx).html(data[idx]);
                }
                offset++;
              }
            }
          });
          $('.form-item-dev-banner-position div.description').css('display', 'none');
        }
      );
    }
  };
})(jQuery);
